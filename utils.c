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
