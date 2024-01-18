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

    matrix send_counts; initializeMatrix(&send_counts, 1, size);
    matrix displacements; initializeMatrix(&displacements, 1, size);
    matrix local_data;

    matrix data;

    const int n_rows_per_process = n_rows / size;
    const int n_rows_remainder = n_rows % size;
    int sum = 0;
    for (int i = 0; i < size; i++) {
        if (i < n_rows_remainder) {
            send_counts.data[i] = (n_rows_per_process + 1) * n_cols;
            displacements.data[i] = sum;
            sum += send_counts.data[i];
        } else {
            send_counts.data[i] = n_rows_per_process * n_cols;
            displacements.data[i] = sum;
            sum += send_counts.data[i];
        }
    }
    if (rank == root) {
        initializeMatrix(&data, n_rows, n_cols);
        initializeMatrix(&local_data, send_counts.data[rank] / n_cols, n_cols);
        for (int i = 0; i < n_rows; i++) {
            for (int j = 0; j < n_cols; j++) {
                data.data[i * n_cols + j] = i * n_cols + j;
            }
        }
        print(&data);
    } else {
        initializeMatrix(&local_data, send_counts.data[rank] / n_cols, n_cols);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == root) {
        printBreakLine();
        printf("Send counts:\n");
        print(&send_counts);
        printf("Displacements:\n");
        print(&displacements);
        printBreakLine();
    }

    MPI_Scatterv(data.data, send_counts.data, displacements.data, MPI_INT, local_data.data, send_counts.data[rank], MPI_INT, root, MPI_COMM_WORLD);
    for (int i = 0; i < local_data.n_row * local_data.n_col; i++) {
        local_data.data[i] *= rank;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gatherv(local_data.data, send_counts.data[rank], MPI_INT, data.data, send_counts.data, displacements.data, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root) {
        printBreakLine();
        printf("After scatterv and gatherv:\n");
        print(&data);
    }
    MPI_Finalize();
    return 0;
}