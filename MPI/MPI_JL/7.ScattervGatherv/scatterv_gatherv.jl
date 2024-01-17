using MPI;

# scatter and gather

function distributeJob(
    job_number::IntType, worker_number::IntType
)::AbstractVector{IntType} where {IntType <: Integer}
    job_per_worker::IntType, remainder::IntType = divrem(job_number, worker_number);
    job_per_worker_array::AbstractVector{<:IntType} = fill(job_per_worker, worker_number);
    job_per_worker_array[1: remainder] .+= 1;
    return job_per_worker_array;
end

MPI.Init();

mpi_communicator::MPI.Comm = MPI.COMM_WORLD;
mpi_rank::Int = MPI.Comm_rank(mpi_communicator);
mpi_size::Int = MPI.Comm_size(mpi_communicator);
const mpi_root::Int = 0;

if mpi_rank == mpi_root
    m, n = 4, 7;

    data = Float64[i for i=1:m, j=1:n];
    output = similar(data);

    # julia matrix stored by column-major
    m_counts = fill(m, mpi_size);
    n_counts = distributeJob(n, mpi_size);

    sizes = vcat(m_counts', n_counts');
    size_ubuf = MPI.UBuffer(sizes, 2); # UBuffer for broadcast
    counts = vec(prod(sizes, dims=1));
    data_vbuf = MPI.VBuffer(data, counts); # VBuffer for scatter
    output_vbuf = MPI.VBuffer(output, counts); # VBuffer for gather
else
    size_ubuf = MPI.UBuffer(nothing);
    output_vbuf = data_vbuf = MPI.VBuffer(nothing);
end

if mpi_rank == mpi_root
    println("Original matrix")
    println("================")
    @show data sizes counts
    println()
    println("Each rank")
    println("================")
end
MPI.Barrier(mpi_communicator);

local_size = MPI.Scatter(size_ubuf, NTuple{2, Int}, mpi_root, mpi_communicator);
local_data = MPI.Scatterv!(data_vbuf, zeros(Float64, local_size), mpi_root, mpi_communicator);

for i_rank = 0: mpi_size - 1
    if mpi_rank == i_rank
        @show mpi_rank local_data
    end
    MPI.Barrier(mpi_communicator);
end

local_data .+= 1;

MPI.Gatherv!(local_data, output_vbuf, mpi_root, mpi_communicator);

if mpi_rank == mpi_root
    println()
    println("Gathered matrix")
    println("================")
    @show output
end

MPI.Finalize();