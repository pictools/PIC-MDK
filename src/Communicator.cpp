#include "Communicator.h"

#include "Exception.h"


namespace picmdk {


namespace {

class CommunicatorImplementation: public Communicator {
public:

    CommunicatorImplementation(MPI_Comm communicator);

    // Getters
    virtual int getRank() const;
    virtual int getNumProcesses() const;
    virtual MPI_Comm getMPICommunicator() const;

    // Communication operations. The names and semantics are the same as
    // for the corresponding MPI operations except the communicator parameter.
    // Error are reported via exceptions.
    virtual void barrier();
    virtual void send(void *buf, int count, MPI_Datatype datatype,
        int dest, int tag);
    virtual void recv(void *buf, int count, MPI_Datatype datatype,
        int source, int tag, MPI_Status *status);
    virtual void sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
        int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype,
        int source, int recvtag, MPI_Status *status);
    virtual void bcast(void *buffer, int count, MPI_Datatype datatype, int root);
    virtual void gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
        void *recvbuf, int recvcount, MPI_Datatype recvtype, int root);
    virtual void reduce(void *sendbuf, void *recvbuf, int count,
        MPI_Datatype datatype, MPI_Op op, int root);
    virtual void allreduce(void *sendbuf, void *recvbuf, int count,
        MPI_Datatype datatype, MPI_Op op);

protected:

    MPI_Comm communicator;
    int rank, numProcesses;

    // Preferrably should be thrown in case std::bad_alloc is caught
    class MPIException : public NamedException {
    public:
        MPIException(const std::string& expression):
            NamedException("Error code returned at call " + expression, "MPI exception") {}
        virtual ~MPIException() throw() {}
    };

};


#define PICMDK_MPI_CHECK(expression) \
    do { \
        if (expression) \
            PICMDK_THROW(MPIException, (#expression)); \
    } while (0)

CommunicatorImplementation::CommunicatorImplementation(MPI_Comm _communicator)
{
    MPI_Comm_dup(_communicator, &communicator);
    PICMDK_MPI_CHECK(MPI_Comm_size(communicator, &numProcesses));
    PICMDK_MPI_CHECK(MPI_Comm_rank(communicator, &rank));
}

int CommunicatorImplementation::getRank() const
{
    return rank;
}

int CommunicatorImplementation::getNumProcesses() const
{
    return numProcesses;
}

MPI_Comm CommunicatorImplementation::getMPICommunicator() const
{
    return communicator;
}

void CommunicatorImplementation::barrier()
{
    PICMDK_MPI_CHECK(MPI_Barrier(communicator));
}

void CommunicatorImplementation::send(void *buf, int count,
    MPI_Datatype datatype, int dest, int tag)
{
    PICMDK_MPI_CHECK(MPI_Send(buf, count, datatype, dest, tag, communicator));
}

void CommunicatorImplementation::recv(void *buf, int count,
    MPI_Datatype datatype, int source, int tag, MPI_Status *status)
{
    PICMDK_MPI_CHECK(MPI_Recv(buf, count, datatype, source, tag, communicator, status));
}

void CommunicatorImplementation::sendrecv(void *sendbuf, int sendcount,
    MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount,
    MPI_Datatype recvtype, int source, int recvtag, MPI_Status *status)
{
    PICMDK_MPI_CHECK(MPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf,
        recvcount, recvtype, source, recvtag, communicator, status));
}

void CommunicatorImplementation::bcast(void *buffer, int count,
    MPI_Datatype datatype, int root)
{
    PICMDK_MPI_CHECK(MPI_Bcast(buffer, count, datatype, root, communicator));
}

void CommunicatorImplementation::gather(void *sendbuf, int sendcount,
    MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root)
{
    PICMDK_MPI_CHECK(MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount,
        recvtype, root, communicator));
}

void CommunicatorImplementation::reduce(void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op, int root)
{
    PICMDK_MPI_CHECK(MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, communicator));
}


void CommunicatorImplementation::allreduce(void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op)
{
    PICMDK_MPI_CHECK(MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, communicator));
}


#undef PICMDK_MPI_CHECK

} // anonymous namespace


::std::auto_ptr<Communicator> Communicator::create()
{
    return ::std::auto_ptr<Communicator>(new CommunicatorImplementation(MPI_COMM_WORLD));
}

::std::auto_ptr<Communicator> Communicator::create(MPI_Comm communicator)
{
    return ::std::auto_ptr<Communicator>(new CommunicatorImplementation(communicator));
}

Communicator::~Communicator()
{
}


} // namespace MDK
