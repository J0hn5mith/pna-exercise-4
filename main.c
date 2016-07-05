#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


#define DIMENSION 20
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
    for (int column = 0; column < DIMENSION; column++) {
        if(column >= step){
            float d = get(matrix, row, column, DIMENSION) - l * get(matrix, step, column, DIMENSION);
            new_row[column] = d;
        } else { // Set all other values to 0 in same iteration
            new_row[column] = 0;
        }
    }
}

void broadcast_row(float* data, struct Node node){
    MPI_Bcast(
            (void*) data,
            DIMENSION + 1,
            MPI_FLOAT,
            node.rank,
            MPI_COMM_WORLD
            );
}

void broadcast_row_by_rank(float* data, int sender_rank, MPI_Request* request){
    MPI_Ibcast(
            data,
            DIMENSION + 1,
            MPI_FLOAT,
            sender_rank,
            MPI_COMM_WORLD,
            request
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
int main(int argc, char *argv[])
{
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;

    struct Node node = init();

    float* matrix = init_matrix(node);
    float* u = generate_unit_matrix(DIMENSION);
    copy_matrix(matrix, u, DIMENSION);
    float* l = generate_unit_matrix(DIMENSION);
    float* buffer = (float*)malloc(((DIMENSION+1) * (DIMENSION + 1))*sizeof(float));

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();
    for (int step = 0; step < DIMENSION; ++step) {
        for (int row = step + 1; row < DIMENSION; ++row) {
            float* row_buffer = buffer + row * (DIMENSION + 1);
            if(row % node.world_size == node.rank){
                process_row(step, row, u, row_buffer);
            }

            broadcast_row_by_rank(row_buffer, row%node.world_size, &request);
            MPI_Wait(&request, &status);
            update_values(l, u, step, row, row_buffer, DIMENSION);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    double  duration = end_time - start_time;

    MPI_Finalize();
    float* check = mul_matrix(l, u, DIMENSION);


    if(!compare_matrix(matrix, check, DIMENSION)){
        return 1;
    }

    if(node.rank == 0){
        printf("Duration: %f\n", duration);
        printf("Successfully finished job\n");
    }

    return 0;
}


