#ifndef PICMDK_COMMUNICATOR_H
#define PICMDK_COMMUNICATOR_H


#include "MPIWrapper.h"

#include <memory>


namespace picmdk {


// Interface to MPI-like communications between processes.
// This class does not aim at providing a wide set of operations or full coverage of MPI.
// Instead, it provides only the most basic operations used by InterData so that it is
// compatible with custom MPI communicators passed in constructor.
// For most modules capabilities of InterData are likely to be sufficient and 
// explicit communication is not needed. However, modules can use this interface as well.
class Communicator {
public:

    // Create a communicator which uses MPI_COMM_WORLD if MPI is enabled or simulates MPI otherwise
    static ::std::auto_ptr<Communicator> create();
    // Create a communicator which uses the given MPI communicator
    static ::std::auto_ptr<Communicator> create(MPI_Comm communicator);

    virtual ~Communicator();

    // Getters
    virtual int getRank() const = 0;
    virtual int getNumProcesses() const = 0;
    virtual MPI_Comm getMPICommunicator() const = 0;

    // Communication operations. The names and semantics are the same as
    // for the corresponding MPI operations except the communicator parameter.
    // Error are reported via exceptions.
    virtual void barrier() = 0;
    virtual void send(void *buf, int count, MPI_Datatype datatype,
        int dest, int tag) = 0;
    virtual void recv(void *buf, int count, MPI_Datatype datatype,
        int source, int tag, MPI_Status *status) = 0;
    virtual void sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype,
        int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype,
        int source, int recvtag, MPI_Status *status) = 0;
    virtual void bcast(void *buffer, int count, MPI_Datatype datatype, int root) = 0;
    virtual void gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
        void *recvbuf, int recvcount, MPI_Datatype recvtype, int root) = 0;
    virtual void reduce(void *sendbuf, void *recvbuf, int count,
        MPI_Datatype datatype, MPI_Op op, int root) = 0;
    virtual void allreduce(void *sendbuf, void *recvbuf, int count,
        MPI_Datatype datatype, MPI_Op op) = 0;

};


} // namespace picmdk


#endif
