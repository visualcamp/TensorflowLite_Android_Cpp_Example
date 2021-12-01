#include <jni.h>

#include <cstdint>

#include <string>

#include "detector.h"
#include "log.h"


template<typename T>
void detector(T) = delete;

inline seeso::Detector* detector(jlong obj) {
  return reinterpret_cast<seeso::Detector*>(obj);
}

extern "C" JNIEXPORT jstring JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_stringFromJNI
    (JNIEnv* env, jobject thiz)
{
  std::string hello = "Hello from C++";
  seeso::Log.d(hello);
  return env->NewStringUTF(hello.c_str());
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

  if (isCopy) {
    env->ReleaseByteArrayElements(buffer, b, 0);
  }

  return success;
}

extern "C" JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeBuildInterpreter
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->buildInterpreter();
}

extern "C"
JNIEXPORT void JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeResetInterpreter
    (JNIEnv* env, jobject thiz, jlong obj)
{
  detector(obj)->resetInterpreter();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_io_seeso_lib_1cpp_1wrapper_Detector_nativeIsProcessing
    (JNIEnv* env, jobject thiz, jlong obj)
{
  return true;
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