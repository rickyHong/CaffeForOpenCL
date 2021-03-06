#ifndef CAFFE_UTIL_OPENCL_DEFINITIONS_H_
#define CAFFE_UTIL_OPENCL_DEFINITIONS_H_

#define OPENCL_WPT 4
#define OPENCL_RTS 8
#define OPENCL_BLOCK_SIZE 16
#define PS 1

#define OPENCL_BLOCK_SIZE_X 16
#define OPENCL_BLOCK_SIZE_Y 16
#define OPENCL_BLOCK_SIZE_1D_X 256
#define OPENCL_BLOCK_SIZE_1D_Y 1

#define OPENCL_LOCAL_SIZE 64

#define OPENCL_NUM_INPUT_QUEUES 1
#define OPENCL_NUM_COMMAND_QUEUES 1
#define OPENCL_NUM_OUTPUT_QUEUES 1

#endif  // CAFFE_UTIL_OPENCL_DEFINITIONS_H_
