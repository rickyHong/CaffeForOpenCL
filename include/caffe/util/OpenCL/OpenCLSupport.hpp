#ifdef USE_OPENCL

#ifndef __OPENCL_SUPPORT_HPP__
#define __OPENCL_SUPPORT_HPP__

#include <CL/cl.h>
#include <clBLAS.h>
#include <glog/logging.h>

#include <caffe/util/OpenCL/OpenCLBuffer.hpp>
#include <caffe/util/OpenCL/OpenCLDevice.hpp>
#include <caffe/util/OpenCL/OpenCLManager.hpp>
#include <caffe/util/OpenCL/OpenCLMemory.hpp>
#include <caffe/util/OpenCL/OpenCLPlatform.hpp>

#include <iostream>  // NOLINT(*)
#include <map>
#include <sstream>
#include <string>
#include <typeindex>
#include <typeinfo>

#include "sys/time.h"
#include "sys/types.h"
#include "time.h"

using namespace std;  // NOLINT(*)
using kernelMapType = map<pair<string, type_index>, string>;

#ifndef OPENCL_OPT_LEVEL
#define OPENCL_OPT_LEVEL 1
#endif

#ifndef CAFFE_OPENCL_VERSION
#define CAFFE_OPENCL_VERSION "0.0"
#endif

#if defined(CL_API_SUFFIX__VERSION_2_0)
#define OPENCL_VERSION 2.0
#define OPENCL_VERSION_2_0
#define OPENCL_VERSION_1_2
#define OPENCL_VERSION_1_1
#define OPENCL_VERSION_1_0
#elif defined(CL_API_SUFFIX__VERSION_1_2)
#define OPENCL_VERSION 1.2
#define OPENCL_VERSION_1_2
#define OPENCL_VERSION_1_1
#define OPENCL_VERSION_1_0
#elif defined(CL_API_SUFFIX__VERSION_1_1)
#define OPENCL_VERSION 1.1
#define OPENCL_VERSION_1_1
#define OPENCL_VERSION_1_0
#elif defined(CL_API_SUFFIX__VERSION_1_0)
#define OPENCL_VERSION 1.0
#define OPENCL_VERSION_1_0
#else
#define OPENCL_VERSION 0.0
#endif

#define CL_PTR_BIT 63

#define CL_CHECK(code) \
  ({ \
    bool ret = false;\
    if ( code != CL_SUCCESS ) { \
      std::ostringstream message;\
      message  <<  "["  <<  __FILE__  <<  " > "\
               <<  __func__  <<  "():"  <<  __LINE__  <<  "]";\
      message  <<  " failed: "  <<  caffe::OpenCL::what(code)\
               <<  " : "  <<  code; \
      std::cerr  <<  message.str()  <<  std::endl; \
      ret = false;\
    } else { \
      ret = true;\
    }\
    ret;\
  })\

#define BOOL_CHECK(code) \
  ({ \
    bool ret = false;\
    if ( code != true ) { \
      std::ostringstream message;\
      message  <<  "["  <<  __FILE__  <<  " > " \
               <<  __func__  <<  "():"  <<  __LINE__  <<  "]";\
      message  <<  " failed."; \
      std::cerr  <<  message.str()  <<  std::endl; \
      ret = false;\
    } else { \
      ret = true;\
    }\
    ret;\
  })\

#define CL_SET_KERNEL_ARG\
  cl_int err;\
  unsigned int idx = 0;\
  std::vector<cl_mem> sb;\
  std::map<const void*, std::pair<void*, size_t> > bm;

#define CL_SET_TYPE_KERNEL_ARG(type, variable, kernel) \
  DLOG(INFO)<<"CL_SET_TYPE_KERNEL_ARG["<<idx<<"] = " <<variable;\
  if ( !clSetKernelTypeArg(variable, idx, kernel) ) return false;

#define CL_SET_ARRAY_KERNEL_ARG(variable, kernel) \
  DLOG(INFO)<<"CL_SET_ARRAY_KERNEL_ARG["<<idx<<"] = "<<*variable;\
  if ( !clSetKernelArrayArg(*variable, idx, sb, bm, kernel) ) return false;

#define CL_SET_KERNEL_ARG_END\
  clReleaseSubBuffers(sb);\
  clReleaseBufferMap(bm);\

#define _format(message, ...)  "%s[%6d] in %s :" message, __FILE__, __LINE__, __func__,  ##__VA_ARGS__  // NOLINT(*)

typedef struct clProfileResult_t {
    struct {
        cl_ulong t_bgn;
        cl_ulong t_end;
        struct {
            uint64_t ns;
            double us;
            double ms;
            double s;
        } time;
    } queued;

    struct {
        cl_ulong t_bgn;
        cl_ulong t_end;
        struct {
            uint64_t ns;
            double us;
            double ms;
            double s;
        } time;
    } submit;

    struct {
        cl_ulong t_bgn;
        cl_ulong t_end;
        struct {
            uint64_t ns;
            double us;
            double ms;
            double s;
        } time;

        struct {
            uint64_t Flop;
            double Flops;
            double MFlops;
            double GFlops;
            double TFlops;
        } compute;

        struct {
            uint64_t Byte;
            double Bytes;
            double MBytes;
            double GBytes;
            double TBytes;
        } io;
    } kernel;

    struct {
        cl_ulong t_bgn;
        cl_ulong t_end;
        struct {
            uint64_t ns;
            double us;
            double ms;
            double s;
        } time;
    } call;
} clProfileResult;

namespace caffe {

namespace OpenCL {

bool clMalloc(void** virtualPtr, size_t);
bool clGetBuffer(void** virtualPtr, size_t);
bool clBufferSetAvailable(void* virtualPtr, size_t size);

bool clFree(void* virtualPtr);
template<typename T> bool clMemset(
    void* gpuPtr,
    const T alpha,
    const size_t Bytes);
bool clMemcpy(void* dst, const void* src, size_t Bytes, int type);
bool clIsVirtualMemory(const void* p);
bool clMakeLogical(const void* ptr_virtual, const void** ptr_logical);
bool clMakeLogical2(
    const void* ptr_virtual,
    const void** ptr_logical,
    std::vector<cl_mem>& subBuffers,  // NOLINT(*)
    std::map<const void*, std::pair<void*, size_t> >& bufferMap);  // NOLINT(*)
template<typename T> bool clSetKernelTypeArg(T variable, unsigned int& idx, // NOLINT(*)
    cl_kernel* kernel);
bool clSetKernelArrayArg(const void* ptr_virtual, unsigned int& idx, // NOLINT(*)
    std::vector<cl_mem>& subBuffers,  // NOLINT(*)
    std::map<const void*, std::pair<void*, size_t> >& bufferMap,  // NOLINT(*)
    cl_kernel* kernel);
bool clReleaseSubBuffers(std::vector<cl_mem>& subBuffers);  // NOLINT(*)
bool clReleaseBufferMap(
    std::map<const void*, std::pair<void*, size_t> >& bufferMap);  // NOLINT(*)

size_t clGetMemoryOffset(const void* ptr_virtual);
size_t clGetMemorySize(const void* ptr_virtual);
void* clGetMemoryBase(const void* ptr_virtual);
bool clGetMemoryObject(const void* ptr_virtual, OpenCLMemory** clMem);

template<typename T> std::string clGetKernelName(std::string name);
template<typename T> int getAlignedSize(
    const int dataSize,
    const int blockSize);
template<typename T> bool parameterSearch(
    const size_t* globalSize,
    const size_t* localLimit,
    size_t* localSize);
template<typename T> bool parameterSearch2D(
    const size_t* globalSize,
    const size_t* localLimit,
    size_t* localSize);

template<typename T> bool clsign(
    const int n,
    const void* array_GPU_x,
    void* array_GPU_y);
template<typename T> bool clsgnbit(
    const int n,
    const void* array_GPU_x,
    void* array_GPU_y);
template<typename T> bool clabs(
    const int n,
    const void* array_GPU_x,
    void* array_GPU_y);
template<typename T> bool cldiv(
    const int n,
    const void* array_GPU_x,
    const void* array_GPU_y,
    void* array_GPU_z);
template<typename T> bool clmul(
    const int n,
    const void* array_GPU_x,
    const void* array_GPU_y,
    void* array_GPU_z);
template<typename T> bool clsub(
    const int n,
    const T* array_GPU_x,
    const T* array_GPU_y,
    T* array_GPU_z);
template<typename T> bool cladd(
    const int n,
    const T* array_GPU_x,
    const T* array_GPU_y,
    T* array_GPU_z);
template<typename T> bool cladd_scalar(const int N, const T alpha, T* Y);
template<typename T> bool clpowx(
    const int n,
    const T* array_GPU_x,
    const T alpha,
    T* array_GPU_z);
template<typename T> bool clexp(
    const int n,
    const T* array_GPU_x,
    T* array_GPU_y);
template<typename T> bool clgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const T* B,
    const T beta,
    T* C,
    cl_event* event);
template<typename T> bool clgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const T* B,
    const size_t idx_offset_B,
    const T beta,
    T* C,
    const size_t idx_offset_C,
    cl_event* event);
template<typename T> bool cl_group_gemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const int gm,
    const int gn,
    const int gk,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const T* B,
    const size_t idx_offset_B,
    const T beta,
    T* C,
    const size_t idx_offset_C,
    cl_event* event);
template<typename T> bool cl_group_gemm_3D(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const int g,
    const int gm,
    const int gn,
    const int gk,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const T* B,
    const size_t idx_offset_B,
    const T beta,
    T* C,
    const size_t idx_offset_C,
    cl_event* event);
template<typename T> bool clgemv(
    const clblasTranspose TransA,
    const int m,
    const int n,
    const T alpha,
    const T* A,
    const size_t step_A,
    const T* x,
    const size_t step_x,
    const T beta,
    T* y,
    const size_t step_y);
template<typename T> bool clgemv(
    const clblasTranspose TransA,
    const int m,
    const int n,
    const T alpha,
    const T* A,
    const T* x,
    const T beta,
    T* y);

/* clBLAS wrapper functions */
template<typename T> bool clBLASasum(const int n, const void* gpuPtr, T* y);
template<typename T> bool clBLASscal(
    const int n,
    const float alpha,
    const void* array_GPU_x,
    void* array_GPU_y);
template<typename T> bool clBLASdot(
    const int n,
    const T* x,
    const int incx,
    const T* y,
    const int incy,
    T* out);
template<typename T> bool clBLASgemv(
    const clblasTranspose TransA,
    const int m,
    const int n,
    const T alpha,
    const T* A,
    const T* x,
    const T beta,
    T* y);
template<typename T> bool clBLASgemv(
    const clblasTranspose TransA,
    const int m,
    const int n,
    const T alpha,
    const T* A,
    const int step_A,
    const T* x,
    const int step_x,
    const T beta,
    T* y,
    const int step_y);
template<typename T> bool clBLASgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const T* x,
    const T beta,
    T* y,
    cl_event* event);
template<typename T> bool clBLASgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const T* x,
    const size_t idx_offset_x,
    const T beta,
    T* y,
    const size_t idx_offset_y);
template<typename T> bool clBLASgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const T* x,
    const size_t idx_offset_x,
    const T beta,
    T* y,
    const size_t idx_offset_y,
    cl_event* event);
template<typename T> bool clBLASgemm(
    const clblasTranspose TransA,
    const clblasTranspose TransB,
    const int m,
    const int n,
    const int k,
    const T alpha,
    const T* A,
    const size_t idx_offset_A,
    const size_t lda,
    const T* x,
    const size_t idx_offset_x,
    const size_t ldx,
    const T beta,
    T* y,
    const size_t idx_offset_y,
    const size_t ldy,
    cl_event* event);
template<typename T> bool clBLASaxpy(
    const int N,
    const T alpha,
    const T* X,
    const int incr_x,
    T* Y,
    const int incr_y);
bool cl_caffe_gpu_rng_uniform(const int n, unsigned int* r);
template<typename T> bool cl_caffe_gpu_rng_uniform(
    const int n,
    const T a,
    const T b,
    T* r);
template<typename T> bool cl_caffe_gpu_rng_gaussian(
    const int n,
    const T mu,
    const T sigma,
    T* r);
template<typename T1, typename T2> bool cl_caffe_gpu_rng_bernoulli(
    const int n,
    const T1 p,
    T2* r);

const char* what(cl_int value);
void clProfile(cl_event event, clProfileResult* profile);
void clLogProfile(
    clProfileResult profile,
    std::string file,
    std::string src,
    std::string function,
    std::string type);

static const int COPY_CPU_TO_CPU = 0;
static const int COPY_CPU_TO_GPU = 1;
static const int COPY_GPU_TO_CPU = 2;
static const int COPY_GPU_TO_GPU = 3;
static const int COPY_DEFAULT = 4;

static std::map<std::pair<std::string, std::type_index>, std::string> mapKernelName; // NOLINT(*)
static std::map<const void*, const void*> mapMemoryToDevice;
static std::map<const void*, size_t> mapMemoryToOffset;
static std::map<const void*, size_t> mapMemoryToSize;
static std::map<const void*, void*> mapMemoryToBase;

}  // namespace OpenCL

}  // namespace caffe

class OpenCLSupportException: public std::exception {
 public:
    explicit OpenCLSupportException(std::string message) {
      message_ = message;
    }
    virtual ~OpenCLSupportException() throw () {  // NOLINT(*)
    }

    virtual const char* what() const throw () {  // NOLINT(*)
      return message_.c_str();
    }

 protected:
 private:
    std::string message_;
};

#endif  // __OPENCL_SUPPORT_HPP__
#endif  // USE_OPENCL
