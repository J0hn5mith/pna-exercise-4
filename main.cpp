#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <mpi.h>
#include <ctime>
#include "matrix.hpp"


#define DIMENSION 5
struct Node{
    int rank;
    int world_size;
};

struct Node init(){
    MPI_Init(NULL, NULL);
    int world_size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    struct Node node;
    node.rank = rank;
    node.world_size = world_size;

    return node;
}


float*  receive_matrix(){
}

float* broadcast_matrix(float* matrix){
    return matrix;
}

float* init_matrix(struct Node* node){
    if(node->rank == 0){
        float* matrix = generate_matrix(DIMENSION);
        return broadcast_matrix(matrix);
    } else {
        return receive_matrix();
    }
}


float* process_row(int step, int row, float* matrix, float* new_row){
    float l  = set(matrix, row, step, get(matrix, row,step, DIMENSION)/get(matrix, step,step, DIMENSION), DIMENSION);
    new_row[DIMENSION] = l;
    for (int column = 0; column < DIMENSION; column++) {
        if(column >= row){
            float d = get(matrix, row, column, DIMENSION) - l * get(matrix, step, column, DIMENSION);
            new_row[column] = d;
        } else { // Set all other values to 0 in same iteration
            new_row[column] = 0;
        }
    }
}

void receive(){

}


int main(int argc, char *argv[])
{
    struct Node node = init();
    float* matrix = generate_matrix(DIMENSION);
    float* u = generate_unit_matrix(DIMENSION);
    copy_matrix(matrix, u, DIMENSION);
    float* l = generate_unit_matrix(DIMENSION);
    float* row_buffer = (float*)malloc((DIMENSION + 1)*sizeof(float));

    for (int step = 0; step < DIMENSION; ++step) {
        for (int row = step + 1; row < DIMENSION; ++row) {
            if(row % node.world_size == node.rank){
                process_row(step, row, u, row_buffer);
                update_values(l, u, step, row, row_buffer, DIMENSION);
            }
        }
        receive();
    }
    return 0;
    delete[] matrix;
    delete[] l;
    delete[] u;
}


