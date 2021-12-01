//
// Created by YongGyu Lee on 2021/12/01.
//

#include <cassert>
#include <cstddef>
#include <cstring>

#include <chrono>
#include <sstream>
#include <string>

#include "detector.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/delegates/gpu/delegate.h"
#include "tensorflow/lite/delegates/nnapi/nnapi_delegate.h"
#include "tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h"
#include "tensorflow/lite/error_reporter.h"
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/model_builder.h"

#include "log.h"

inline void dummy() {};

#define RETURN_IF_TF_FAIL(expr, ret)  \
    if ((expr) != kTfLiteOk) {        \
      LOG_ERROR_HERE;                 \
      return ret;                     \
    }

#define RETURN_FALSE_IF_TF_FAIL(expr) RETURN_IF_TF_FAIL(expr, false)

namespace seeso {

namespace {

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

} // anonymous namespace

bool Detector::loadModel(const char* buffer, std::size_t buffer_size) {

//  tflite::StatefulNnApiDelegate test(nullptr);

  model_ = tflite::FlatBufferModel::BuildFromBuffer(buffer, buffer_size);
  return model_ != nullptr;
}

void Detector::setLabels(const char* labels, std::size_t labels_size) {

}

void Detector::setNumThreads(int num) {
  num_thread_ = num;
  interpreter_->SetNumThreads(num_thread_);
}

void Detector::setUseCPU() {
  build_type_ = kCPU;
}

void Detector::setUseNnApi() {
  nnapi_delegate_ = std::make_unique<tflite::StatefulNnApiDelegate>();
  build_type_ = kCPU;
}

void Detector::setUseGpu() {
  gpu_delegate_.reset(TfLiteGpuDelegateV2Create(&gpu_option));
  build_type_ = kGPU;
}

void Detector::resetInterpreter() {
  interpreter_.reset();
}

bool Detector::buildInterpreter() {
  RETURN_FALSE_IF_TF_FAIL(tflite::InterpreterBuilder(*model_, resolver_)(&interpreter_))
  RETURN_FALSE_IF_TF_FAIL(interpreter_->AllocateTensors())

  PrintModelInfo(interpreter_.get());

  return true;
}

void Detector::setInput(std::size_t index, const void* data, std::size_t data_size) {
  std::memcpy(interpreter_->input_tensor(index)->data.data, data, data_size);
}

int Detector::invoke() {
  namespace chrono = std::chrono;
  using clock = chrono::steady_clock;

  const auto t1 = clock::now();
  {
    const auto status = interpreter_->Invoke();
    assert(status == kTfLiteOk);
  }
  const auto t2 = clock::now();

  return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
}

const Detector::Recognition Detector::getResult() const {
  return {};
}

} // namespace seeso
