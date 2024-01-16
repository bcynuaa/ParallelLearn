#include <stdio.h>
#include <mpi.h>

struct Particle {
    double mass;
}typedef Particle;

void combine(Particle* p1, Particle* p2, Particle* combined_p) {
    combined_p->mass = p1->mass + p2->mass;
    return;
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process id
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get number of processes

    Particle p;
    p.mass = rank + 1;

    printf("Process %d has mass %f\n", rank, p.mass);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        Particle total_p;
        MPI_Reduce(&p, &total_p, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("Total mass: %f\n", total_p.mass);
    } else {
        MPI_Reduce(&p, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double arr[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = rank + i;
    }
    double sum_arr[5];

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(&arr, &sum_arr, 5, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total array: ");
        for (int i = 0; i < 5; i++) {
            printf("%f ", sum_arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize(); // Finalize MPI environment
    return 0;
}