#include "container.h"
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int root = 0;

    const int n_rows = 10;
    const int n_cols = 4;

    if (rank == root) {
        matrix data;
        initializeMatrix(&data, n_rows, n_cols);
        print(&data);
        printBreakLine();
    }

    MPI_Finalize();
    return 0;
}