#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

#define DIMENSION 10
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


float* generate_matrix(){
    std::srand(std::time(0));
    float* matrix = (float*)malloc(DIMENSION*DIMENSION*sizeof(float));
    for (int i = 0; i < DIMENSION*DIMENSION; ++i) {
        matrix[i] = (float)(std::rand()%10);
    }

    return (float*)matrix;
}

float*  receive_matrix(){
}

float* broadcast_matrix(float* matrix){
    return matrix;
}

float* init_matrix(struct Node* node){
    if(node->rank == 0){
        float* matrix = generate_matrix();
        return broadcast_matrix(matrix);
    } else {
        return receive_matrix();
    }
}

float* generate_unit_matrix(){
    float* matrix = (float*)malloc(DIMENSION*DIMENSION*sizeof(float));
    int row = -1;
    for (int i = 0; i < DIMENSION*DIMENSION; ++i) {
        int column = i % DIMENSION;
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

float  set(float* matrix, int row, int column, float value){
    matrix[DIMENSION*row + column] = value;
    return value;
}

float get(float* matrix, int row, int column){
    return matrix[DIMENSION*row + column];
}

void process_row(int step, int row, float* matrix){
    float new_row[DIMENSION] = {0};
    float l  = set(matrix, row, step, get(matrix, row,step)/get(matrix, step,step));
    for (int column = row; column < DIMENSION; column++) {
        new_row[column] = get(matrix, row, column) - l * get(matrix, step, column);
    }
}

void receive(){

}


int main(int argc, char *argv[])
{
    struct Node node = init();
    float* matrix = generate_matrix();
    float* l = matrix;
    float* u = generate_unit_matrix();

    for (int step = 0; step < DIMENSION; ++step) {
        for (int row = 0; row < DIMENSION; ++row) {
            if(row % node.world_size == node.rank){
                process_row(step, row, u);
            }
        }
        receive();
    }
    return 0;
    //delete[] matrix;
    //delete[] l;
    //delete[] u;
}


