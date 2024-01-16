using MPI;
MPI.Init();

# matrix's initial space seems not support in MPI

const mpi_communicator = MPI.COMM_WORLD;
const mpi_rank = MPI.Comm_rank(mpi_communicator);
const mpi_size = MPI.Comm_size(mpi_communicator);
const mpi_root = 0;
const n = 10;

func(row, col) = row + col;

if mpi_rank == mpi_root
    arr_s = [zeros(n) for i in 1: mpi_size];
    for i = 1: n
        arr_s[1][i] = func(0, i);
    end
else
    arr = zeros(n);
end

if mpi_rank == mpi_root
    print("Before: \n")
    for i = 1: mpi_size
        print(arr_s[i], "\n");
    end
    println("="^100);
else
    for i = 1: n
        arr[i] = func(mpi_rank, i);
    end
end

if mpi_rank != mpi_root
    MPI.Isend(arr, mpi_communicator; dest = 0, tag = mpi_rank+32);
else
    for i = 1: mpi_size-1
        MPI.Irecv!(arr_s[i+1], mpi_communicator; source = i, tag = i+32);
    end
end

MPI.Barrier(mpi_communicator);

if mpi_rank == mpi_root
    print("After: \n")
    for i = 1: mpi_size
        print(arr_s[i], "\n");
    end
    println("="^100);
end

MPI.Finalize();