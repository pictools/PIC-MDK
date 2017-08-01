#ifndef PICMDK_MPIWRAPPER_H
#define PICMDK_MPIWRAPPER_H

////
#define PICMDK_USE_MPI
#ifdef PICMDK_USE_MPI

#include <mpi.h>

#else

#include <cstring>

// Stubs for employed MPI types and constants
typedef int MPI_Comm;
typedef int MPI_Status;
const MPI_Comm MPI_COMM_WORLD = 0;
enum MPI_Datatype { MPI_MPI_DATATYPE_NULL, MPI_CHAR, MPI_INT, MPI_FLOAT, MPI_DOUBLE };
enum MPI_Op {
    MPI_OP_NULL, MPI_MAX, MPI_MIN, MPI_SUM, MPI_PROD, MPI_LAND,
    MPI_BAND, MPI_LOR, MPI_BOR, MPI_LXOR, MPI_BXOR, MPI_MINLOC, MPI_MAXLOC, MPI_REPLACE
};


int MPI_Comm_rank(MPI_Comm comm, int* rank);
int MPI_Comm_size(MPI_Comm comm, int* size);
void MPI_Comm_dup(MPI_Comm comm, MPI_Comm* newcomm);
int MPI_Barrier(MPI_Comm comm);
int MPI_Send(const void *buf, int count, MPI_Datatype datatype,
    int dest, int tag, MPI_Comm comm);
int MPI_Recv(void *buf, int count,
    MPI_Datatype datatype, int source, int tag, MPI_Status *status, MPI_Comm comm);
int MPI_Sendrecv(const void *sendbuf, int sendcount,
    MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount,
    MPI_Datatype recvtype, int source, int recvtag, MPI_Status *status, MPI_Comm comm);
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Reduce(const void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int MPI_Allreduce(const void *sendbuf, void *recvbuf,
    int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);


#endif


#endif
