#include "container.h"
#include <mpi.h>

int function(int row, int col) {
    return (row+1) * (col+1);
}

int main() {
    int mpi_rank, mpi_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Status status;
    const int root = 0;

    const int row_number = 10;
    matrix mat;

    if (mpi_rank == root) initializeMatrix(&mat, mpi_size, row_number);
    else initializeMatrix(&mat, 1, row_number);

    if (mpi_rank == root) {
        printf("Before, matrix is:\n");
        print(&mat);
        printBreakLine();
    }

    for (int i = 0; i < row_number; i++) {
        setItem(&mat, 0, i, function(mpi_rank, i));
    }

    if (mpi_rank != root) MPI_Send(&mat.data[0], row_number, MPI_INT, root, 0, MPI_COMM_WORLD);
    else {
        for (int i = 1; i < mpi_size; i++) {
            MPI_Recv(&mat.data[i * row_number], row_number, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        printf("After, matrix is:\n");
        print(&mat);
    }

    MPI_Finalize();
    return 0;
}