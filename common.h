#pragma once

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "mc_runtime.h"
#include "mccl.h"

#define MCCHECK(cmd)                                                           \
  do {                                                                         \
    mcError_t err = cmd;                                                       \
    if (err != mcSuccess) {                                                    \
      printf("Failed: MACA error %s:%d '%s'\n", __FILE__, __LINE__,            \
             mcGetErrorString(err));                                           \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define MCCLCHECK(cmd)                                                         \
  do {                                                                         \
    mcclResult_t res = cmd;                                                    \
    if (res != mcclSuccess) {                                                  \
      printf("Failed, MCCL error %s:%d '%s'\n", __FILE__, __LINE__,            \
             mcclGetErrorString(res));                                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

typedef enum {
  DATA_INT8 = 0,
  DATA_INT32 = 1,
  DATA_FLOAT16 = 2,
  DATA_BFLOAT16 = 3,
  DATA_FLOAT32 = 4,
  DATA_NUM_TYPES = 5,
} DataType;

typedef enum {
  OP_SUM = 0,
  OP_NONE = 1,
} RedOp;

typedef enum {
  COUNT_FROM_SEND = 0,
  COUNT_FROM_RECV = 1,
  COUNT_FROM_RECV_DIV_NRANKS = 2,
} CountMode;

typedef struct sizeInfo {
  // sizeBytes is the per-rank payload after any operator-specific alignment.
  size_t sizeBytes; // effective payload bytes per rank
  size_t sendBytes;
  size_t recvBytes;
  size_t count; // elements passed to collective
} sizeInfo_t;

typedef struct testArgs {
  int nranks;
  // Size sweep configuration (bytes).
  size_t minBytes;
  size_t maxBytes;
  size_t sizeBytes;
  size_t sendBytes;
  size_t recvBytes;
  size_t maxSendBytes;
  size_t maxRecvBytes;
  size_t count;
  DataType type;
  const char *typeName;
  int all_dtypes;
  RedOp op;
  const char *opName;
  int root;
  int warmup_iters;
  int iters;
  // agg_iters: number of collectives aggregated per timing window.
  int agg_iters;
  int check;
  int checkSamples;
  int blocking;
  // 0=oop, 1=ip, 2=oop+ip
  int inplace_mode; // 0=oop, 1=ip, 2=oop+ip
  double stepFactor;
  size_t stepBytes;
} testArgs_t;

typedef struct threadArgs {
  int rank;
  int nranks;
  size_t sendBytes;
  size_t recvBytes;
  size_t count;
  int in_place;
  size_t in_place_offset;
  int send_is_alias;
  int check_phase;
  mcclUniqueId commId;
  mcclComm_t *comms;
  mcclComm_t comm;
  mcStream_t stream;
  mcEvent_t start;
  mcEvent_t stop;
  void *base_sendbuff;
  void *base_recvbuff;
  void *sendbuff;
  void *recvbuff;
  int errors;
  double avg_us;
  double algbw;
  double busbw;
  const sizeInfo_t *sizes;
  int nSizes;
  double *time_us;
  double *algbw_out;
  double *busbw_out;
  int *errors_out;
  int *errors_by_rank;
  pthread_barrier_t *barrier;
  const struct testEngine *engine;
  const testArgs_t *test;
} threadArgs_t;

typedef struct testEngine {
  const char *name;
  size_t defaultSizeBytes;
  int supportsInplace;
  DataType defaultType;
  const char *defaultTypeName;
  RedOp defaultOp;
  const char *defaultOpName;
  int defaultRoot;
  CountMode countMode;
  // Compute per-rank send/recv byte sizes from logical countBytes and nranks.
  void (*getBuffSize)(size_t *sendBytes, size_t *recvBytes, size_t countBytes,
                      int nranks, size_t typeSize);
  // Initialize device buffers (including expected data) for this rank.
  void (*initData)(threadArgs_t *args, int root, DataType type, RedOp op,
                   int in_place);
  // Enqueue one collective operation into the stream.
  void (*runTest)(threadArgs_t *args, int root, DataType type, RedOp op,
                  const void *sendbuff, void *recvbuff, mcStream_t stream);
  // Return byte offset into recv buffer for in-place send region.
  size_t (*getInplaceOffset)(size_t sendBytes, size_t recvBytes, int nranks,
                             int rank);
  // Compute algorithmic and bus bandwidth from timing.
  void (*getBw)(size_t sendBytes, size_t recvBytes, int nranks, double timeSec,
                double *algbw, double *busbw);
  // Optional data validation hook; return number of errors for this rank.
  int (*checkData)(threadArgs_t *args, int root, DataType type, RedOp op,
                   int in_place);
} testEngine_t;

extern testEngine_t mcclTestEngine;

int get_device_count();
int apply_env_override(int devicecount);
size_t dtypeSize(DataType dt);
const char *dtypeName(DataType dt);
mcclDataType_t toMcclType(DataType dt);
int dtypeSupported(DataType dt);
const DataType *getAllDtypes(int *count);
int parseDataType(const char *name, DataType *out);
void dtypeGetTolerances(DataType dt, float *atol, float *rtol);
size_t get_type_size(DataType type);
void parseArgs(int argc, char **argv, testArgs_t *args,
               const testEngine_t *engine);
void setupArgsForSize(testArgs_t *args, const testEngine_t *engine,
                      size_t sizeBytes);
int build_samples(const threadArgs_t *args, size_t *out, int max);
float get_check_alpha();
float get_check_atol();
float get_check_rtol();
