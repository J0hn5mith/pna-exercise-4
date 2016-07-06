#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>


#define DIMENSION 3
#define SIZE DIMENSION*DIMENSION
#define R_DIMENSION  (DIMENSION + 1)
#define R_SIZE R_DIMENSION*R_DIMENSION

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

void broadcast_matrix_by_rank(float* data, int sender_rank){
    MPI_Bcast(
            data,
            DIMENSION * (DIMENSION + 1),
            MPI_FLOAT,
            sender_rank,
            MPI_COMM_WORLD
            );
}

void init(struct Node* node, float** m, float** l, float** u, float** rb){
    *m = init_matrix(*node);
    *rb  = (float*)malloc(R_SIZE*sizeof(float));
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
    float*  matrix, *u, *l, *receive_buffer = NULL;
    init(&node, &matrix, &l, &u, &receive_buffer);

    MPI_Barrier(MPI_COMM_WORLD);
    start_timer();
    for (int step = 0; step < DIMENSION; ++step) {

        int remaining_rows = DIMENSION - step - 1;
        int block_size = ceil(remaining_rows/node.world_size);
        int start = step + 1 + block_size * node.rank;

        if(remaining_rows%node.world_size != 0 && node.rank + 1 == node.world_size){
            block_size = remaining_rows%node.world_size;
        }
        int end = start +  block_size;
        for (int row = start; row < end; ++row) {
            process_row(step, row, l, u);
        }

        /*for (int worker = 0; worker < node.world_size; ++worker) {*/
        /*float* active_buffer;*/
        /*if(worker == node.rank){ // send values*/
        /*active_buffer = buffer;*/
        /*} else { //receive values*/
        /*active_buffer = receive_buffer;*/
        /*}*/

        /*broadcast_matrix_by_rank(active_buffer, worker);*/

        /*if(worker != node.rank){ // send values*/
        /*int block_size = ceil(remaining_rows/node.world_size);*/
        /*int start = step + 1 + block_size * worker;*/
        /*if(remaining_rows%node.world_size != 0 && worker + 1 == node.world_size){*/
        /*block_size = remaining_rows%node.world_size;*/
        /*}*/
        /*int end = start +  block_size;*/
        /*for (int row = start; row < end; ++row) {*/
        /*int buffer_row = row/node.world_size;*/
        /*float* row_buffer = active_buffer + buffer_row * (DIMENSION + 1);*/
        /*update_values(l, u, step, row, row_buffer, DIMENSION);*/
        /*}*/
        /*for (int row = step + 1; row < DIMENSION; ++row) {*/
        /*if(row % node.world_size == worker){*/
        /*int buffer_row = row/node.world_size;*/
        /*float* row_buffer = receive_buffer + buffer_row * (DIMENSION + 1);*/
        /*update_values(l, u, step, row, row_buffer, DIMENSION);*/
        /*}*/
        /*}*/
        /*}*/
        /*}*/
        /*}*/
}

MPI_Barrier(MPI_COMM_WORLD);
stop_timer();
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
