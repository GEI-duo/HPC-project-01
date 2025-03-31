#!/bin/bash

SIZE=(100 1000 2000)

chmod u+x ./heat_serial
for (( i = 0; i < ${#SIZE[@]}; i++ )); do
    ./heat_serial ${SIZE[i]} ${SIZE[i]} tests/seq-${SIZE[i]}-${SIZE[i]}.bmp
done

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
END="\e[0m"

echo "========================================="
echo "TESTS"
echo "-----------------------------------------"

for test_file in ./Test/*_final.out; do
    filename=$(basename -- $test_file)
    solved_file="./res/$filename"
    
    if ! test -f "$solved_file"; then
        echo -e "${YELLOW}NOTF${END} $filename"
        continue
    fi

    cmp --silent $solved_file $test_file

    if [[ $? -eq 0 ]]; then
        echo -e "${GREEN}PASS${END} $filename"
    else
        echo -e "${RED}FAIL${END} $filename"
    fi
done

echo "========================================="