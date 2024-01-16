import MPI;
MPI.Init();

const mpi_communicator::MPI.Comm = MPI.COMM_WORLD;
const mpi_rank::Int = MPI.Comm_rank(mpi_communicator);
const mpi_size::Int = MPI.Comm_size(mpi_communicator);
const root::Int = 0;

struct Particle
    mass::Float64
    Particle(mass)::Particle = new(Float64(mass));
end

function combine(p1::Particle, p2::Particle)::Particle
    return Particle(p1.mass + p2.mass);
end

particle = Particle(mpi_rank+1);
print("I'm particle $mpi_rank and my mass is $(particle.mass)\n");

MPI.Barrier(mpi_communicator);

combined_particle = MPI.Reduce(particle, combine, root, mpi_communicator);

if mpi_rank == root
    print("I'm the master and the combined particle has mass $(combined_particle.mass)\n");
end

MPI.Finalize();