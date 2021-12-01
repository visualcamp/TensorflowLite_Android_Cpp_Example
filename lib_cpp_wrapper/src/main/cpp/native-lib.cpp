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
Java_com_example_lib_1cpp_1wrapper_Detector_stringFromJNI
    (JNIEnv* env, jobject thiz)
{
  std::string hello = "Hello from C++";
  seeso::Log.d(hello);
  return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_lib_1cpp_1wrapper_Detector_nativeDetector
    (JNIEnv* env, jobject thiz)
{
  static_assert(sizeof(jlong) >= sizeof(intptr_t), "intptr_t must be representable with jlong");
  return reinterpret_cast<jlong>(new seeso::Detector());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_lib_1cpp_1wrapper_Detector_nativeDetectorDelete
    (JNIEnv* env, jobject thiz, jlong obj)
{
  delete detector(obj);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lib_1cpp_1wrapper_Detector_nativeDetectorLoadModel
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