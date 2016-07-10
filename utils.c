#include <mpi.h>
#include <stdio.h>

#include "utils.h"


void start_timer(struct Timer* timer){
    timer->start_time = MPI_Wtime();
}

void stop_timer(struct Timer* timer){
    timer->end_time = MPI_Wtime();
    timer->duration += timer->end_time - timer->start_time;
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
