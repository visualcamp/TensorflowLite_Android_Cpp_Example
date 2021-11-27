#include <jni.h>
#include <string>

#include "log.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_tensorflowlite_1android_1cpp_1example_MainActivity_stringFromJNI(
    JNIEnv* env,
    jobject /* this */) {
  std::string hello = "Hello from C++";

  seeso::Log.d(hello);

  return env->NewStringUTF(hello.c_str());
}