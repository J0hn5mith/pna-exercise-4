#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>


#define DIMENSION 2000
#define SIZE DIMENSION*DIMENSION

struct Node{
    int rank;
    int world_size;
};

struct Node init_mpi(){
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

void broadcast_matrix(float* matrix){
    MPI_Bcast(
            (void*) matrix,
            SIZE,
            MPI_FLOAT,
            0,
            MPI_COMM_WORLD
            );
}

float* init_matrix(struct Node node){
    float* matrix = generate_matrix(DIMENSION);
    broadcast_matrix(matrix);
    return matrix;
}

void process_row(int step, int row, float* l, float* u){
    float l_value  = get(u, row, step, DIMENSION)/get(u, step, step, DIMENSION);
    set(l, row, step, l_value, DIMENSION);
    for (int column = step; column < DIMENSION; column++) {
        float u_value = get(u, row, column, DIMENSION) - l_value\
                        * get(u, step, column, DIMENSION);
        set(u,row, column, u_value, DIMENSION);
    }
}

void broadcast_rows(float* matrix, int sender, int start_row, int num_rows){
    matrix += start_row * DIMENSION;
    MPI_Bcast(
            matrix,
            num_rows * DIMENSION,
            MPI_FLOAT,
            sender,
            MPI_COMM_WORLD
            );
}

void init(struct Node* node, float** m, float** l, float** u){
    *m = init_matrix(*node);
    *u = generate_unit_matrix(DIMENSION);
    copy_matrix(*m, *u, DIMENSION);
    *l = generate_unit_matrix(DIMENSION);
}

static double start_time;
static double end_time;
static double  duration;
void start_timer(){
    start_time = MPI_Wtime();
}

void stop_timer(){
    end_time = MPI_Wtime();
    duration = end_time - start_time;
}

int main(int argc, char *argv[]) {
    struct Node node = init_mpi();
    float*  matrix, *u, *l = NULL;
    init(&node, &matrix, &l, &u);

    MPI_Barrier(MPI_COMM_WORLD);
    start_timer();
    for (int step = 0; step < DIMENSION; ++step) {
        int remaining_rows = DIMENSION - step - 1;

        // Calculate Own values
        if(node.rank < remaining_rows ){
            int block_size = floor((float)remaining_rows/(float)node.world_size);
            int block_start = remaining_rows - block_size * (node.world_size - node.rank);
            if(block_size == 0 || node.rank < remaining_rows%block_size){
                block_size++;
                block_start = step + 1 + block_size * node.rank;
            }
            int end = block_start +  block_size;
            for (int row = block_start; row < end; ++row) {
                /*printf("Worker: %d; Start: %d; Size: %d; End: %d \n", node.rank, block_start, block_size, end);*/
                /*printf("Rank\n");*/
                process_row(step, row, l, u);
            }
        }

        // Communicate u values
        for (int worker = 0; worker < node.world_size; ++worker) {
            if(worker < remaining_rows ){
                int block_size = floor((float)remaining_rows/(float)node.world_size);
                int block_start = remaining_rows - block_size * (node.world_size - worker);
                if(block_size == 0 || worker < remaining_rows%block_size){
                    block_size++;
                    block_start = step + 1 + block_size * worker;
                }
                broadcast_rows(u, worker, block_start, block_size);
                broadcast_rows(l, worker, block_start, block_size);
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    stop_timer();
    MPI_Finalize();

    float* check = mul_matrix(l, u, DIMENSION);

    if(!compare_matrix(matrix, check, DIMENSION)){
        printf("Error!!\n");
        return 1;
    }

    if(node.rank == 0){
        printf("Duration: %f\n", duration);
        printf("Successfully finished job\n");
    }
    free(matrix);
    free(l);
    free(u);

    return 0;
}
