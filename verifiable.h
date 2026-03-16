#pragma once

#include "common.h"

void verifiable_init_allreduce(threadArgs_t *args, int root, DataType type,
                               int in_place);
int verifiable_check_allreduce(threadArgs_t *args, int root, DataType type);
void verifiable_init_reduce(threadArgs_t *args, int root, DataType type,
                            int in_place);
int verifiable_check_reduce(threadArgs_t *args, int root, DataType type);
void verifiable_init_allgather(threadArgs_t *args, int root, DataType type,
                               int in_place);
int verifiable_check_allgather(threadArgs_t *args, int root, DataType type);
void verifiable_init_gather(threadArgs_t *args, int root, DataType type,
                            int in_place);
int verifiable_check_gather(threadArgs_t *args, int root, DataType type);
void verifiable_init_broadcast(threadArgs_t *args, int root, DataType type,
                               int in_place);
int verifiable_check_broadcast(threadArgs_t *args, int root, DataType type);
void verifiable_init_reducescatter(threadArgs_t *args, int root, DataType type,
                                   int in_place);
int verifiable_check_reducescatter(threadArgs_t *args, int root, DataType type);
void verifiable_init_scatter(threadArgs_t *args, int root, DataType type,
                             int in_place);
int verifiable_check_scatter(threadArgs_t *args, int root, DataType type);
void verifiable_init_sendrecv(threadArgs_t *args, int root, DataType type,
                              int in_place);
int verifiable_check_sendrecv(threadArgs_t *args, int root, DataType type);
void verifiable_init_alltoall(threadArgs_t *args, int root, DataType type,
                              int in_place);
int verifiable_check_alltoall(threadArgs_t *args, int root, DataType type);
