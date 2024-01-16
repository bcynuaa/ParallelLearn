#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int mpi_rank, mpi_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Status status;
    // printf("Hello world from process %d of %d\n", mpi_rank, mpi_size);
    if (mpi_rank == 0) {
        int send_data = 99+mpi_rank;
        // MPI_Send(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        for (int i = 1; i < mpi_size; i++) {
            MPI_Send(&send_data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int recv_data = 0;
        printf("Before recv_data = %d\n", recv_data);
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("After recv_data = %d\n", recv_data);
    }
    MPI_Finalize();
    return 0;
}