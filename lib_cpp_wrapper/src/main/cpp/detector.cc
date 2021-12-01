//
// Created by YongGyu Lee on 2021/12/01.
//

#include <cstddef>

#include <sstream>
#include <string>

#include "detector.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/error_reporter.h"
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/kernels/register.h"

#include "log.h"

inline void dummy() {};

#define RETURN_IF_TF_FAIL(expr, ret)  \
    if ((expr) != kTfLiteOk) {        \
      LOG_ERROR_HERE;                 \
      return ret;                     \
    }

#define RETURN_FALSE_IF_TF_FAIL(expr) RETURN_IF_TF_FAIL(expr, false)

namespace seeso {

//class SeeSoErrorReporter : public tflite::ErrorReporter {
// public:
//  int Report(const char* format, va_list args) override {
//    return 0;
//  }
//};

std::string GetTensorInfo(const TfLiteTensor* tensor) {
  std::stringstream ss;

  ss << tensor->name << " "
     << TfLiteTypeGetName(tensor->type) << " ";


  if (tensor->dims->size == 0) {
    ss << "None";
  } else {
    ss << tensor->dims->data[0];
    for (int i = 1; i < tensor->dims->size; ++i)
      ss << 'x' << tensor->dims->data[i];
  }

  return ss.str();
}

void PrintModelInfo(const tflite::Interpreter* interpreter) {
  std::stringstream ss;

  ss << "Input Tensor: \n";
  for (const auto id : interpreter->inputs())
    ss << "\t" << GetTensorInfo(interpreter->tensor(id)) << '\n';

  ss << "\nOutput Tensor: \n";
  for (const auto id : interpreter->outputs())
    ss << "\t" << GetTensorInfo(interpreter->tensor(id)) << '\n';

  Log.v(ss.str());
}

bool Detector::loadModel(const char* buffer, std::size_t buffer_size) {
  model_ = tflite::FlatBufferModel::BuildFromBuffer(buffer, buffer_size);

  tflite::ops::builtin::BuiltinOpResolver resolver;
  RETURN_FALSE_IF_TF_FAIL(tflite::InterpreterBuilder(*model_, resolver)(&interpreter_))

  RETURN_FALSE_IF_TF_FAIL(interpreter_->AllocateTensors())

  PrintModelInfo(interpreter_.get());
  return true;
}

} // namespace seeso
