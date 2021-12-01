//
// Created by YongGyu Lee on 2021/12/01.
//

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/delegates/gpu/delegate.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/kernels/register.h"

namespace seeso {

class Detector {
 public:
  Detector() = default;

  bool loadModel(const char* buffer, std::size_t buffer_size);

  void setLabels(const char* labels, std::size_t labels_size);

  bool buildInterpreter();

  void resetInterpreter();

  void setNumThreads(int num);

  void setUseCPU();

  void setUseNnApi();

  void setUseGpu();

  void setInput(std::size_t index, const void* data, std::size_t data_size);

  /**
   * Invoke the model
   * @return Inference time in milliseconds
   */
  int invoke();

  struct Recognition {
    const std::string id;
    const std::string title;
    const float confidence;
    const float location[4];
  };

  const Recognition getResult() const;


 private:
  enum BUILD_TYPE {
    kCPU,
    kGPU,
    kNNAPI,
  };

  /// 'delegate' must outlive the interpreter.
  std::unique_ptr<TfLiteDelegate> nnapi_delegate_;

  std::unique_ptr<TfLiteDelegate, decltype(&TfLiteGpuDelegateV2Delete)>
      gpu_delegate_{nullptr, &TfLiteGpuDelegateV2Delete};
  TfLiteGpuDelegateOptionsV2 gpu_option = TfLiteGpuDelegateOptionsV2Default();

  std::unique_ptr<tflite::FlatBufferModel> model_;
  std::unique_ptr<tflite::Interpreter> interpreter_;
  tflite::ops::builtin::BuiltinOpResolver resolver_;

  int num_thread_ = 2;
  BUILD_TYPE build_type_ = kCPU;

  std::vector<std::string> labels_;
};

} // namespace seeso

#endif // DETECTOR_H_
