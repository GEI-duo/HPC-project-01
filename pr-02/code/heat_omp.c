#include "heat.h"
#include <omp.h>


void print_grid(double *grid, int nx, int ny)
{
    int i, j;
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            printf("%.2f ", grid[i * ny + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void initialize_grid(double *grid, int nx, int ny, int temp_source)
{
    int i, j;
    
    // TODO: Check if for loops should be reverse order due to cache
    #pragma omp parallel for private(j) collapse(2)
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            if (i == j)
                grid[i * ny + j] = 1500.0;
            else if (i == nx - 1 - j)
                grid[i * ny + j] = 1500.0;
            else
                grid[i * ny + j] = 0.0;
        }
    }
}

void solve_heat_equation(double *grid, double *new_grid, int steps, double r, int nx, int ny)
{
    int step, i, j;
    double *temp;

    for (step = 0; step < steps; step++)
    {
        // Compute the new grid
        #pragma omp parallel for private(j) collapse(2)
        for (i = 1; i < nx - 1; i++)
        {
            for (j = 1; j < ny - 1; j++)
            {
                new_grid[i * ny + j] = grid[i * ny + j] + r * (grid[(i + 1) * ny + j] + grid[(i - 1) * ny + j] - 2 * grid[i * ny + j]) + r * (grid[i * ny + j + 1] + grid[i * ny + j - 1] - 2 * grid[i * ny + j]);
            }
        }

        // Apply boundary conditions (Dirichlet: u=0 on boundaries)
        #pragma omp parallel for
        for (i = 0; i < nx; i++)
        {
            new_grid[0 * ny + i] = 0.0;
            new_grid[ny * (nx - 1) + i] = 0.0;
        }
        #pragma omp parallel for
        for (j = 0; j < ny; j++)
        {
            new_grid[0 + j * nx] = 0.0;
            new_grid[(ny - 1) + j * nx] = 0.0;
        } // Swap the grids

        temp = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

void write_bmp_header(FILE *file, int width, int height)
{
    unsigned char header[BMP_HEADER_SIZE] = {0};

    int file_size = BMP_HEADER_SIZE + 3 * width * height;
    header[0] = 'B';
    header[1] = 'M';
    header[2] = file_size & 0xFF;
    header[3] = (file_size >> 8) & 0xFF;
    header[4] = (file_size >> 16) & 0xFF;
    header[5] = (file_size >> 24) & 0xFF;
    header[10] = BMP_HEADER_SIZE;

    header[14] = 40; // Info header size
    header[18] = width & 0xFF;
    header[19] = (width >> 8) & 0xFF;
    header[20] = (width >> 16) & 0xFF;
    header[21] = (width >> 24) & 0xFF;
    header[22] = height & 0xFF;
    header[23] = (height >> 8) & 0xFF;
    header[24] = (height >> 16) & 0xFF;
    header[25] = (height >> 24) & 0xFF;
    header[26] = 1;  // Planes
    header[28] = 24; // Bits per pixel

    fwrite(header, 1, BMP_HEADER_SIZE, file);
}

void get_color(double value, unsigned char *r, unsigned char *g, unsigned char *b)
{

    if (value >= 500.0)
    {
        *r = 255;
        *g = 0;
        *b = 0; // Red
    }
    else if (value >= 100.0)
    {
        *r = 255;
        *g = 128;
        *b = 0; // Orange
    }
    else if (value >= 50.0)
    {
        *r = 171;
        *g = 71;
        *b = 188; // Lilac
    }
    else if (value >= 25)
    {
        *r = 255;
        *g = 255;
        *b = 0; // Yellow
    }
    else if (value >= 1)
    {
        *r = 0;
        *g = 0;
        *b = 255; // Blue
    }
    else if (value >= 0.1)
    {
        *r = 5;
        *g = 248;
        *b = 252; // Cyan
    }
    else
    {
        *r = 255;
        *g = 255;
        *b = 255; // white
    }
}

void write_grid(FILE *file, double *grid, int nx, int ny)
{
    int row_stride = ny * 3;
    int padding = (4 - (row_stride % 4)) % 4;
    int padded_row_size = row_stride + padding;
    int total_size = nx * padded_row_size;
    int i, j, p;

    unsigned char *pixel_data = malloc(total_size);
    if (!pixel_data) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    #pragma omp parallel for private(j, p)
    for (i = 0; i < nx; i++) {
        int row_index = nx - 1 - i; // BMP is bottom-to-top
        int row_offset = i * padded_row_size;

        for (j = 0; j < ny; j++) {
            unsigned char r, g, b;
            get_color(grid[row_index * ny + j], &r, &g, &b);

            int pixel_offset = row_offset + j * 3;
            pixel_data[pixel_offset + 0] = b;
            pixel_data[pixel_offset + 1] = g;
            pixel_data[pixel_offset + 2] = r;
        }

        // Zero out padding at the end of the row
        for (p = 0; p < padding; p++) {
            pixel_data[row_offset + row_stride + p] = 0;
        }
    }

    fwrite(pixel_data, 1, total_size, file);
    free(pixel_data);
}

// Main function
int main(int argc, char *argv[])
{
    double time_begin, elapsed_time;
    char car;
    double r;   // constant of the heat equation
    int nx, ny; // Grid size in x-direction and y-direction
    int steps;  // Number of time steps
    // double DT;
    if (argc != 4)
    {
        printf("Command line wrong\n");
        printf("Command line should be: heat_serial size steps name_output_file.bmp. \n");
        printf("Try again!!!!\n");
        return 1;
    }
    nx = ny = atoi(argv[1]);
    r = ALPHA * DT / (DX * DY);
    steps = atoi(argv[2]);
    time_begin = omp_get_wtime();
    // Allocate memory for the grid
    double *grid = (double *)calloc(nx * ny, sizeof(double));
    double *new_grid = (double *)calloc(nx * ny, sizeof(double));

    // Initialize the grid
    initialize_grid(grid, nx, ny, T);

    // Solve heat equation
    solve_heat_equation(grid, new_grid, steps, r, nx, ny);
    // Write grid into a bmp file
    FILE *file = fopen(argv[3], "wb");
    if (!file)
    {
        printf("Error opening the output file.\n");
        return 1;
    }

    write_bmp_header(file, nx, ny);
    write_grid(file, grid, nx, ny);

    fclose(file);
    // Function to visualize the values of the temperature. Use only for debugging
    print_grid(grid, nx, ny);
    //  Free allocated memory
    free(grid);
    free(new_grid);
    elapsed_time = omp_get_wtime() - time_begin;
    printf("The Execution Time=%fs with a matrix size of %dx%d and %d steps\n", elapsed_time, nx, nx, steps);
    return 0;
}
