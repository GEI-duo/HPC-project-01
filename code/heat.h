#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BMP_HEADER_SIZE 54
#define ALPHA 0.01 // Thermal diffusivity
#define L 0.2      // Length (m) of the square domain
#define DX 0.02    // grid spacing in x-direction
#define DY 0.02    // grid spacing in y-direction
#define DT 0.0005  // Time step
#define T 1500     // Temperature on ºk of the heat source

/**
 * @brief Function to print the grid (optional, for debugging or visualization)
 */
void print_grid(double *grid, int nx, int ny);

/**
 * @brief Function to initialize the grid
 */
void initialize_grid(double *grid, int nx, int ny, int temp_source);

/**
 * @brief Function to solve the heat equation
 */
void solve_heat_equation(double *grid, double *new_grid, int steps, double r, int nx, int ny);

/**
 * @brief Function to write the grid into a BMP file
 */
void write_bmp_header(FILE *file, int width, int height);

/**
 * @brief Function to write the grid into a BMP file
 */
void write_grid(FILE *file, double *grid, int nx, int ny);
