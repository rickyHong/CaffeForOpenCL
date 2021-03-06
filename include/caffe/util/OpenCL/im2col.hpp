#ifndef __OPENCL_IM2COL_LAYER_HPP__
#define __OPENCL_IM2COL_LAYER_HPP__

#include <CL/cl.h>
#include <glog/logging.h>

#include <caffe/util/OpenCL/OpenCLDevice.hpp>
#include <caffe/util/OpenCL/OpenCLManager.hpp>
#include <caffe/util/OpenCL/OpenCLPlatform.hpp>
#include <caffe/util/OpenCL/OpenCLSupport.hpp>

#include <iostream>   // NOLINT(*)
#include <sstream>    // NOLINT(*)
#include <string>

namespace caffe {

namespace OpenCL {

template<typename T> bool clim2col_gpu(
    const int num_kernels,
    const T* data_im,
    const int height,
    const int width,
    const int kernel_h,
    const int kernel_w,
    const int pad_h,
    const int pad_w,
    const int stride_h,
    const int stride_w,
    const int height_col,
    const int width_col,
    T* data_col);
template<typename T> bool clim2col_gpu(
    const int n,
    const T* data_im,
    const size_t data_im_step,
    const int height,
    const int width,
    const int kernel_h,
    const int kernel_w,
    const int pad_h,
    const int pad_w,
    const int stride_h,
    const int stride_w,
    const int height_col,
    const int width_col,
    T* data_col,
    const size_t data_col_step);
template<typename T> bool clim2col_group_gpu(
    const int num_kernels,
    const T* data_im,
    const int bottom_step,
    const int num_images,
    const int num_channels,
    const int height,
    const int width,
    const int kernel_h,
    const int kernel_w,
    const int pad_h,
    const int pad_w,
    const int stride_h,
    const int stride_w,
    const int height_col,
    const int width_col,
    T* data_col,
    const int top_step);
template<typename T> bool clim2col_group_gpu(
    const T* data_im,
    const int* mask,
    const int num_images,
    const int num_channels,
    const int height,
    const int width,
    const int kernel_h,
    const int kernel_w,
    const int height_out,
    const int width_out,
    T* data_col);

template<typename T> bool clcol2im_gpu(
    const int n,
    const T* data_col,
    const int height,
    const int width,
    const int channels,
    const int patch_h,
    const int patch_w,
    const int pad_h,
    const int pad_w,
    const int stride_h,
    const int stride_w,
    const int height_col,
    const int width_col,
    T* data_im);
template<typename T> bool clcol2im_gpu(
    const int n,
    const T* data_col,
    const int top_step,
    const int col_number,
    const int height,
    const int width,
    const int channels,
    const int patch_h,
    const int patch_w,
    const int pad_h,
    const int pad_w,
    const int stride_h,
    const int stride_w,
    const int height_col,
    const int width_col,
    T* data_im,
    const int bottom_step);

}  // namespace OpenCL

}  // namespace caffe

#endif  // __OPENCL_IM2COL_LAYER_HPP__
