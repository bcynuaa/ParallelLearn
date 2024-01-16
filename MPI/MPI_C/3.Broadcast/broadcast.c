#include "container.h"
#include <mpi.h>

int main(int argc, char *argv[]) {
    int mpi_rank, mpi_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    const int root = 0;

    matrix m;
    initializeMatrix(&m, 1, 10);

    if (mpi_rank == root) {
        for (int i = 0; i < m.n_col; i++) {
            setItem(&m, 0, i, i);
        }
    }
    printf("Processor %d, Before broadcast:\n", mpi_rank);
    print(&m);
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (mpi_rank == root) {
        printBreakLine(); // print 50 '='
    }
    else;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(m.data, m.n_col, MPI_INT, root, MPI_COMM_WORLD);

    printf("Processor %d, After broadcast:\n", mpi_rank);
    print(&m);

    MPI_Finalize();
    return 0;
}