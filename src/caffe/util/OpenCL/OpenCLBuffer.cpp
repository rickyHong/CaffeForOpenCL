#ifdef USE_OPENCL

#include <CL/cl.h>
#include <glog/logging.h>
#include <stdio.h>
#include <string.h>

#include <caffe/util/OpenCL/OpenCLBuffer.hpp>
#include <caffe/util/OpenCL/OpenCLSupport.hpp>

#include <exception>
#include <iostream>  // NOLINT(*)

namespace caffe {

  OpenCLBuffer::OpenCLBuffer() {
    available_ = true;
  }

  OpenCLBuffer::OpenCLBuffer(size_t size)
      : OpenCLMemory::OpenCLMemory(size) {
    available_ = true;
  }

  OpenCLBuffer::~OpenCLBuffer() {
  }

  bool OpenCLBuffer::isAvailable() {
    return available_;
  }

  void OpenCLBuffer::setAvailable() {
    available_ = true;
  }

  void OpenCLBuffer::setUnavailable() {
    available_ = false;
  }

}  // namespace caffe

#endif  // USE_OPENCL
