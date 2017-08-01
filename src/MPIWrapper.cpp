#include "MPIWrapper.h"

/* This is a simple wrapper over MPI_get_rank. */
#ifndef PICMDK_USE_MPI

#include <cstring>


size_t getSize(MPI_Datatype type)
{
    switch (type) {
        case MPI_MPI_DATATYPE_NULL: return 0;
        case MPI_CHAR: return sizeof(char);
        case MPI_INT: return sizeof(int);
        case MPI_FLOAT: return sizeof(float);
        case MPI_DOUBLE: return sizeof(double);
        default: return -1;
    }
}


int MPI_Comm_rank(MPI_Comm comm, int* rank)
{
    *rank = 0;
    return 0;
}

int MPI_Comm_size(MPI_Comm comm, int* size)
{
    *size = 1;
    return 0;
}

void MPI_Comm_dup(MPI_Comm comm, MPI_Comm* newcomm)
{
    *newcomm = comm;
}

int MPI_Barrier(MPI_Comm comm)
{
    return 0;
}

int MPI_Send(const void *buf, int count, MPI_Datatype datatype,
    int dest, int tag, MPI_Comm comm)
{
    /// TODO
    return 1;
}

int MPI_Recv(void *buf, int count,
    MPI_Datatype datatype, int source, int tag, MPI_Status *status, MPI_Comm comm)
{
    /// TODO
    return 1;
}

int MPI_Sendrecv(const void *sendbuf, int sendcount,
    MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount,
    MPI_Datatype recvtype, int source, int recvtag, MPI_Status *status, MPI_Comm comm)
{
    /// TODO
    return 1;
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    /// TODO
    return 1;
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
    if ((!root) && (sendcount * getSize(sendtype) == recvcount * getSize(recvtype)))
    {
        memcpy(recvbuf, sendbuf, sendcount * getSize(sendtype));
        return 0;
    }
    else
        return 1;
}

int MPI_Reduce(const void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    memcpy(recvbuf, sendbuf, count * getSize(datatype));
    return 0;
}

inline int MPI_Allreduce(const void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_Reduce(sendbuf, recvbuf, count, datatype, op, 0, comm);
}


#endif
