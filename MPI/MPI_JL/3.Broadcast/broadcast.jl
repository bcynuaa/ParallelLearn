import MPI;
MPI.Init();

const mpi_communicator::MPI.Comm = MPI.COMM_WORLD;
const mpi_rank::Int = MPI.Comm_rank(mpi_communicator);
const mpi_size::Int = MPI.Comm_size(mpi_communicator);
const root_rank::Int = 0;

const n = 10;

if mpi_rank == root_rank
    A = Vector(LinRange(1, n, n)) .|> round;
else
    A = Array{Float64}(undef, n);
end

print("Process $mpi_rank: A = $A\n");

if mpi_rank == root_rank
    print("="^100 * "\n");
end

MPI.Bcast!(A, root_rank, mpi_communicator);

print("Process $mpi_rank: A = $A\n");

MPI.Finalize();