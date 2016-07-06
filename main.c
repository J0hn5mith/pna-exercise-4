#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>


#define DIMENSION 3
#define SIZE DIMENSION*DIMENSION
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


void process_row(int step, int row, float* matrix, float* new_row){
    float l  = get(matrix, row, step, DIMENSION)/get(matrix, step, step, DIMENSION);
    new_row[DIMENSION] = l;
    new_row[DIMENSION] = l;
    for (int column = step; column < DIMENSION; column++) {
        new_row[column] = get(matrix, row, column, DIMENSION) - l\
                          * get(matrix, step, column, DIMENSION);
    }
}

void broadcast_matrix_by_rank(float* data, int sender_rank){
    MPI_Bcast(
            data,
            DIMENSION * (DIMENSION + 1),
            MPI_FLOAT,
            sender_rank,
            MPI_COMM_WORLD
            );
}

void verbose_mpi_status(MPI_Status status){
    if(status.MPI_ERROR == MPI_SUCCESS){
        printf("Success\n");
    } else if(status.MPI_ERROR == MPI_ERR_REQUEST){
        printf("MPI_ERR_REQUEST\n");
    } else if(status.MPI_ERROR == MPI_ERR_ARG){
        printf("MPI_ERR_ARG\n");
    } else {
        printf("Unknown\n");
    }
}

int main(int argc, char *argv[]) {
    struct Node node = init();

    float* matrix = init_matrix(node);
    float* u = generate_unit_matrix(DIMENSION);
    copy_matrix(matrix, u, DIMENSION);
    float* l = generate_unit_matrix(DIMENSION);
    float* buffer = (float*)malloc(((DIMENSION+1) * (DIMENSION + 1))*sizeof(float));
    float* receive_buffer = (float*)malloc(((DIMENSION+1) * (DIMENSION + 1))*sizeof(float));

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();
    for (int step = 0; step < DIMENSION; ++step) {

        int remaining_rows = DIMENSION - step - 1;
        int block_size = ceil(remaining_rows/node.world_size);
        int start = step + 1 + block_size * node.rank;
        if(remaining_rows%node.world_size != 0 && node.rank + 1 == node.world_size){
            block_size = remaining_rows%node.world_size;
        }
        int end = start +  block_size;
        /*printf("Start: %d\nSize: %d\nRemaining Rows:%d\n", start, block_size, remaining_rows);*/
        for (int row = start; row < end; ++row) {
            int buffer_row = row/node.world_size;
            float* row_buffer = buffer + buffer_row * (DIMENSION + 1);
            process_row(step, row, u, row_buffer);
            update_values(l, u, step, row, row_buffer, DIMENSION);
        }

        for (int worker = 0; worker < node.world_size; ++worker) {
            float* active_buffer;
            if(worker == node.rank){ // send values
                active_buffer = buffer;
            } else { //receive values
                active_buffer = receive_buffer;
            }

            broadcast_matrix_by_rank(active_buffer, worker);

            if(worker != node.rank){ // send values
                int block_size = ceil(remaining_rows/node.world_size);
                int start = step + 1 + block_size * worker;
                if(remaining_rows%node.world_size != 0 && worker + 1 == node.world_size){
                    block_size = remaining_rows%node.world_size;
                }
                int end = start +  block_size;
                for (int row = start; row < end; ++row) {
                    int buffer_row = row/node.world_size;
                    float* row_buffer = active_buffer + buffer_row * (DIMENSION + 1);
                    update_values(l, u, step, row, row_buffer, DIMENSION);
                }
                /*for (int row = step + 1; row < DIMENSION; ++row) {*/
                /*if(row % node.world_size == worker){*/
                /*int buffer_row = row/node.world_size;*/
                /*float* row_buffer = receive_buffer + buffer_row * (DIMENSION + 1);*/
                /*update_values(l, u, step, row, row_buffer, DIMENSION);*/
                /*}*/
                /*}*/
                /*}*/
        }
    }
}
MPI_Barrier(MPI_COMM_WORLD);
double end_time = MPI_Wtime();
double  duration = end_time - start_time;

MPI_Finalize();

float* check = mul_matrix(l, u, DIMENSION);
print_matrix(u, DIMENSION);

if(!compare_matrix(matrix, check, DIMENSION)){
    printf("Error!!\n");
    return 1;
}

if(node.rank == 0){
    printf("Duration: %f\n", duration);
    printf("Successfully finished job\n");
}

return 0;
}
