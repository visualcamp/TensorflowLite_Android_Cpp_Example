//
// Created by YongGyu Lee on 2021/12/01.
//

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <cstddef>

#include <memory>

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/delegates/gpu/gl_delegate.h"
#include "tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h"
#include "tensorflow/lite/delegates/nnapi/nnapi_delegate.h"

namespace seeso {

class Detector {
 public:
  Detector() = default;

  bool loadModel(const char* buffer, std::size_t buffer_size);

 private:
  std::unique_ptr<tflite::FlatBufferModel> model_;
  std::unique_ptr<tflite::Interpreter> interpreter_;


};

} // namespace seeso

#endif // DETECTOR_H_
