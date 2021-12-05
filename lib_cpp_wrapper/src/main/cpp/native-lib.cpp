#include <jni.h>

#include <cstdint>

#include <algorithm>
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

#include "detector.h"
#include "log.h"

template<typename T>
void detector(T) = delete;

inline seeso::Detector* detector(jlong obj) {
  return reinterpret_cast<seeso::Detector*>(obj);
}

extern "C" JNIEXPORT jlong JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeDetector
    (JNIEnv* env, jobject thiz)
{
  static_assert(sizeof(jlong) >= sizeof(intptr_t), "intptr_t must be representable with jlong");
  return reinterpret_cast<jlong>(new seeso::Detector());
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeDetectorDelete
    (JNIEnv* env, jobject thiz, jlong obj)
{
  delete detector(obj);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeDetectorLoadModel
    (JNIEnv* env, jobject thiz, jlong obj, jbyteArray buffer, jlong buffer_size)
{
  jboolean isCopy;
  jbyte* b = env->GetByteArrayElements(buffer, &isCopy);
  const auto success = detector(obj)->loadModel(reinterpret_cast<const char*>(b), buffer_size);
  env->ReleaseByteArrayElements(buffer, b, 0);

  return success;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeBuildInterpreter
    (JNIEnv* env, jobject thiz, jlong obj)
{
  return detector(obj)->buildInterpreter();
}

extern "C"
JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeResetInterpreter
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->resetInterpreter();
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeSetUseCPU
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->setUseCPU();
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeSetUseGPU
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->setUseGpu();
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeSetUseNnApi
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->setUseNnApi();
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeSetUseXNNPack
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->setUseXNNPack();
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeSetNumThreads
    (JNIEnv* env, jobject thiz, jlong obj, jint num)
{
  detector(obj)->setNumThreads(num);
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeAddImage
    (JNIEnv* env, jobject thiz, jlong obj, jbyteArray data)
{
  static const int index = 0; // Image is index 0

  const auto input_size = detector(obj)->input_bytes(index);
  jbyte *buffers = env->GetByteArrayElements(data, NULL);

  cv::Mat srgb(300, 300, CV_8UC4, buffers);
  cv::Mat rgb;
  cv::cvtColor(srgb, rgb, cv::COLOR_RGBA2RGB);

  detector(obj)->setInput(index, rgb.data, input_size);
  env->ReleaseByteArrayElements(data, buffers, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeInvoke
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->invoke();
}

extern "C" JNIEXPORT jfloatArray JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeGetOutput
    (JNIEnv* env, jobject thiz, jlong obj, jint index)
{
  const auto output_size = detector(obj)->output_bytes(index) / sizeof(float);

  std::vector<float> output_cpp_type(output_size);
  detector(obj)->copy_output(output_cpp_type.data(), index);

  std::vector<jfloat> output_jni_type(output_size);
  std::transform(output_cpp_type.begin(), output_cpp_type.end(), output_jni_type.begin(), [](auto val) {
    return static_cast<jfloat>(val);
  });

  jfloatArray output = env->NewFloatArray(output_size);
  env->SetFloatArrayRegion(output, 0, output_size, output_jni_type.data());

  return output;
}