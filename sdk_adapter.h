#pragma once

#if defined(SDK_MACA) == defined(SDK_HPCC)
#error "Define exactly one of SDK_MACA or SDK_HPCC"
#endif

#if defined(SDK_MACA)
#include "mc_runtime.h"
#include "mccl.h"

#define DEVCHECK(cmd)                                                         \
  do {                                                                        \
    mcError_t err = cmd;                                                      \
    if (err != mcSuccess) {                                                   \
      printf("Failed: MACA error %s:%d '%s'\n", __FILE__, __LINE__,           \
             mcGetErrorString(err));                                          \
      exit(EXIT_FAILURE);                                                     \
    }                                                                         \
  } while (0)

#define CCLCHECK(cmd)                                                         \
  do {                                                                        \
    mcclResult_t res = cmd;                                                   \
    if (res != mcclSuccess) {                                                 \
      printf("Failed, MCCL error %s:%d '%s'\n", __FILE__, __LINE__,           \
             mcclGetErrorString(res));                                        \
      exit(EXIT_FAILURE);                                                     \
    }                                                                         \
  } while (0)

typedef mcStream_t rtStream_t;
typedef mcEvent_t rtEvent_t;
typedef mcclUniqueId cclUniqueId_t;
typedef mcclComm_t cclComm_t;
typedef mcclDataType_t cclDataType_t;
typedef mcclRedOp_t cclRedOp_t;

#define cclInt8 mcclInt8
#define cclInt32 mcclInt32
#define cclHalf mcclHalf
#define cclFloat16 mcclFloat16
#define cclFloat mcclFloat
#define cclSum mcclSum

#define rtGetDeviceCount mcGetDeviceCount
#define rtSetDevice mcSetDevice
#define rtStreamCreate mcStreamCreate
#define rtStreamDestroy mcStreamDestroy
#define rtStreamSynchronize mcStreamSynchronize
#define rtEventCreate mcEventCreate
#define rtEventDestroy mcEventDestroy
#define rtEventRecord mcEventRecord
#define rtEventSynchronize mcEventSynchronize
#define rtEventElapsedTime mcEventElapsedTime
#define rtMalloc mcMalloc
#define rtFree mcFree
#define rtMemcpy mcMemcpy
#define rtMemset mcMemset
#define rtMemcpyHostToDevice mcMemcpyHostToDevice
#define rtMemcpyDeviceToHost mcMemcpyDeviceToHost

#define cclGetUniqueId mcclGetUniqueId
#define cclCommInitRank mcclCommInitRank
#define cclCommDestroy mcclCommDestroy
#define cclGetErrorString mcclGetErrorString
#define cclAllReduce mcclAllReduce
#define cclAllGather mcclAllGather
#define cclBroadcast mcclBroadcast
#define cclReduce mcclReduce
#define cclReduceScatter mcclReduceScatter
#define cclGroupStart mcclGroupStart
#define cclGroupEnd mcclGroupEnd
#define cclSend mcclSend
#define cclRecv mcclRecv
#define cclAllToAll mcclAllToAll
#define cclAllToAllv mcclAllToAllv
#define cclAllToAlld mcclAllToAlld

#elif defined(SDK_HPCC)
#include "hc_runtime.h"
#include "hccl.h"

#define DEVCHECK(cmd)                                                         \
  do {                                                                        \
    hcError_t err = cmd;                                                      \
    if (err != hcSuccess) {                                                   \
      printf("Failed: HPCC error %s:%d '%s'\n", __FILE__, __LINE__,           \
             hcGetErrorString(err));                                          \
      exit(EXIT_FAILURE);                                                     \
    }                                                                         \
  } while (0)

#define CCLCHECK(cmd)                                                         \
  do {                                                                        \
    hcclResult_t res = cmd;                                                   \
    if (res != hcclSuccess) {                                                 \
      printf("Failed, HCCL error %s:%d '%s'\n", __FILE__, __LINE__,           \
             hcclGetErrorString(res));                                        \
      exit(EXIT_FAILURE);                                                     \
    }                                                                         \
  } while (0)

typedef hcStream_t rtStream_t;
typedef hcEvent_t rtEvent_t;
typedef hcclUniqueId cclUniqueId_t;
typedef hcclComm_t cclComm_t;
typedef hcclDataType_t cclDataType_t;
typedef hcclRedOp_t cclRedOp_t;

#define cclInt8 hcclInt8
#define cclInt32 hcclInt32
#define cclHalf hcclHalf
#define cclFloat16 hcclFloat16
#define cclFloat hcclFloat
#define cclSum hcclSum

#define rtGetDeviceCount hcGetDeviceCount
#define rtSetDevice hcSetDevice
#define rtStreamCreate hcStreamCreate
#define rtStreamDestroy hcStreamDestroy
#define rtStreamSynchronize hcStreamSynchronize
#define rtEventCreate hcEventCreate
#define rtEventDestroy hcEventDestroy
#define rtEventRecord hcEventRecord
#define rtEventSynchronize hcEventSynchronize
#define rtEventElapsedTime hcEventElapsedTime
#define rtMalloc hcMalloc
#define rtFree hcFree
#define rtMemcpy hcMemcpy
#define rtMemset hcMemset
#define rtMemcpyHostToDevice hcMemcpyHostToDevice
#define rtMemcpyDeviceToHost hcMemcpyDeviceToHost

#define cclGetUniqueId hcclGetUniqueId
#define cclCommInitRank hcclCommInitRank
#define cclCommDestroy hcclCommDestroy
#define cclGetErrorString hcclGetErrorString
#define cclAllReduce hcclAllReduce
#define cclAllGather hcclAllGather
#define cclBroadcast hcclBroadcast
#define cclReduce hcclReduce
#define cclReduceScatter hcclReduceScatter
#define cclGroupStart hcclGroupStart
#define cclGroupEnd hcclGroupEnd
#define cclSend hcclSend
#define cclRecv hcclRecv
#define cclAllToAll hcclAllToAll
#define cclAllToAllv hcclAllToAllv
#define cclAllToAlld hcclAllToAlld
#endif
