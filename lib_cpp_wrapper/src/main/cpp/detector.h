//
// Created by YongGyu Lee on 2021/12/01.
//

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <cstddef>

#include <memory>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/delegates/gpu/delegate.h"
#include "tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/kernels/register.h"

namespace seeso {

class Detector {
 public:
  Detector() = default;

  bool loadModel(const char* buffer, std::size_t buffer_size);

  bool buildInterpreter();

  void resetInterpreter();

  void setNumThreads(int num);

  void setUseCPU();

  void setUseNnApi();

  void setUseGpu();

  void setUseXNNPack();

  void setInput(std::size_t index, const void* data, std::size_t data_size);

  /**
   * Invoke the model
   * @return Inference time in milliseconds
   */
  int invoke();

  std::size_t input_bytes(std::size_t index) const;

  std::size_t output_bytes(std::size_t index) const;

  void copy_output(void* dst, std::size_t index) const;




 private:
  enum BUILD_TYPE {
    kCPU,
    kGPU,
    kNNAPI,
    kXNNPack,
  };

  /// 'delegate' must outlive the interpreter.

  // NNAPI
  std::unique_ptr<TfLiteDelegate> nnapi_delegate_;

  // GPU
  TfLiteGpuDelegateOptionsV2 gpu_options_ = TfLiteGpuDelegateOptionsV2Default();
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteGpuDelegateV2Delete)>
      gpu_delegate_{nullptr, &TfLiteGpuDelegateV2Delete};

  // XNNPack
  TfLiteXNNPackDelegateOptions xnn_options_ = TfLiteXNNPackDelegateOptionsDefault();
  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteXNNPackDelegateDelete)>
    xnn_delegate_{nullptr, &TfLiteXNNPackDelegateDelete};

  std::unique_ptr<tflite::FlatBufferModel> model_;
  tflite::ops::builtin::BuiltinOpResolver resolver_;
  std::unique_ptr<tflite::Interpreter> interpreter_;

  int num_thread_ = 2;
  BUILD_TYPE build_type_ = kCPU;
};

} // namespace seeso

#endif // DETECTOR_H_
