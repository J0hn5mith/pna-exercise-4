#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void print_row(float* row, int dimension){
    for (int i = 0; i < dimension; ++i) {
        printf("%f ", row[i]);
    }
    printf("\n");
}

void print_matrix(float* matrix, int dimension){
    printf("=================\n");
    for (int i = 0; i < dimension; ++i) {
        int offset = i * dimension;
        print_row(matrix + offset, dimension);
    }
    printf("=================\n");
}

float* generate_matrix(int dimension){
    //srand(time(0));
    srand(1);
    float* matrix = (float*)malloc(dimension*dimension*sizeof(float));
    for (int i = 0; i < dimension*dimension; ++i) {
        matrix[i] = (float)(rand()%9 + 1);
    }

    return (float*)matrix;
}

float* generate_unit_matrix(int dimension){
    float* matrix = (float*)malloc(dimension*dimension*sizeof(float));
    int row = -1;
    for (int i = 0; i < dimension*dimension; ++i) {
        int column = i % dimension;
        if(column == 0){
            row++;
        }
        if (column == row){
            matrix[i] = 1;
        } else {
            matrix[i] = 0;
        }
    }
    return (float*)matrix;
}

float set(float* matrix, int row, int column, float value, int dimension){
    matrix[dimension*row + column] = value;
    return value;
}


void set_lines(float* matrix, int start_row, int num_rows, float* values, int dimension){
    int offset = dimension*start_row;
    float* start = matrix + offset;
    memcpy(start, values, num_rows * dimension*sizeof(float));
}

void set_line(float* matrix, int row, float* values, int dimension){
    set_lines(matrix, row, 1, values, dimension);
}

float get(float* matrix, int row, int column, int dimension){
    return matrix[dimension*row + column];
}

void update_values(float* l, float* u, int step, int row, float* new_values, int dimension){
    set_line(u, row, new_values, dimension);
    set(l, row, step, new_values[dimension], dimension);
}

void copy_matrix(float* original, float* copy, int dimension){
    memcpy(copy, original, dimension*dimension*sizeof(float));
}

float* mul_matrix(float* left, float* right, int dimension){
    float* result  = (float*)malloc(dimension*dimension*sizeof(float));
    for (int i = 0; i < dimension; ++i) {
        for (int ii = 0; ii < dimension; ++ii) {
            float sum = 0;
            for (int iii = 0; iii < dimension; ++iii) {
                sum += get(left, i, iii, dimension) * get(right, iii, ii, dimension);
            }
            set(result, i, ii, sum, dimension);
        }
    }

    return result;
}

int  compare_matrix(float* left, float* right, int dimension){
    for (int i = 0; i < dimension; ++i) {
        for (int ii = 0; ii < dimension; ++ii) {
            float difference = fabsf(get(left, i, ii, dimension)  - get(right, i, ii, dimension));
            if(difference > 0.1){
                printf("ERROR: %f (%d %d)\n",difference, i, ii);
                return 0;
            }
        }
    }
    return 1;
}
