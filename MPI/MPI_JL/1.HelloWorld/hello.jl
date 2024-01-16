using MPI;
MPI.Init();

communicator::MPI.Comm = MPI.COMM_WORLD;
rank::Int = MPI.Comm_rank(communicator);
size::Int = MPI.Comm_size(communicator);

print("Helo world, this is process $rank of $size\n");

MPI.Finalize();