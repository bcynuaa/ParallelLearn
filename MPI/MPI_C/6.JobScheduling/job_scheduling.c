#include "container.h"
#include <mpi.h>

int f(int x) {
    return x + 100;
}

int main(int argc, char *argv[]) {
    int mpi_rank, mpi_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    const int mpi_root = 0;
    const int mpi_workers_number = mpi_size - 1;

    const int n_rows = 10;
    const int n_cols = 5;

    matrix original_data, job_schedule, receive;
    int job_number = n_rows * n_cols;

    if (mpi_rank == mpi_root) {
        initializeMatrix(&original_data, n_rows, n_cols);
        for (int i_row = 0; i_row < n_rows; i_row++) {
            for (int i_col = 0; i_col < n_cols; i_col++) {
                setItem(&original_data, i_row, i_col, i_row * n_cols + i_col);
            }
        }
        print(&original_data);
        printBreakLine();
        initializeMatrix(&job_schedule, mpi_workers_number, 2);
        const int n_jobs = job_number;
        const int n_jobs_per_worker = n_jobs / mpi_workers_number;
        const int n_jobs_last_worker = n_jobs - n_jobs_per_worker * mpi_workers_number;
        for (int i_worker = 1; i_worker <= mpi_workers_number; i_worker++) {
            // job_start, job_number
            if (i_worker <= n_jobs_last_worker) {
                setItem(&job_schedule, i_worker - 1, 0, (i_worker - 1) * (n_jobs_per_worker + 1));
                setItem(&job_schedule, i_worker - 1, 1, n_jobs_per_worker + 1);
            }
            else {
                setItem(&job_schedule, i_worker - 1, 0, (i_worker - 1) * n_jobs_per_worker + n_jobs_last_worker);
                setItem(&job_schedule, i_worker - 1, 1, n_jobs_per_worker);
            }
            MPI_Send(&job_schedule.data[(i_worker-1)*2+1], 1, MPI_INT, i_worker, 0, MPI_COMM_WORLD);
        }
        print(&job_schedule);
    }
    else {
        MPI_Recv(&job_number, 1, MPI_INT, mpi_root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        initializeMatrix(&receive, job_number, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %d: job_number = %d\n", mpi_rank, job_number);
    MPI_Barrier(MPI_COMM_WORLD);
    // send job to workers
    if (mpi_rank == mpi_root) {
        int i_job_start, i_job_number;
        for (int i_worker = 1; i_worker <= mpi_workers_number; i_worker++) {
            getItem(&job_schedule, i_worker - 1, 0, &i_job_start);
            getItem(&job_schedule, i_worker - 1, 1, &i_job_number);
            MPI_Send(&original_data.data[i_job_start], i_job_number, MPI_INT, i_worker, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&receive.data[0], job_number, MPI_INT, mpi_root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i_job = 0; i_job < job_number; i_job++) {
            setItem(&receive, i_job, 0, f(receive.data[i_job]));
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // receive job from workers
    if (mpi_rank == mpi_root) {
        int i_job_start, i_job_number;
        for (int i_worker = 1; i_worker <= mpi_workers_number; i_worker++) {
            getItem(&job_schedule, i_worker - 1, 0, &i_job_start);
            getItem(&job_schedule, i_worker - 1, 1, &i_job_number);
            MPI_Recv(&original_data.data[i_job_start], i_job_number, MPI_INT, i_worker, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else {
        MPI_Send(&receive.data[0], job_number, MPI_INT, mpi_root, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (mpi_rank == mpi_root) {
        printBreakLine();
        print(&original_data);
    }

    MPI_Finalize();
}