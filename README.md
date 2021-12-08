# Android sample project for using Tensorflow Lite C++

## Overview
Modified version of [TFLite Object Detection Demo App](https://github.com/tensorflow/examples/tree/master/lite/examples/object_detection/android)
* Use Tensorflow Lite C++ API and OpenCV C++ instead of Java API
* Additional support for NNAPI, GPU, XNN

## Introduction
Prebuilt libraries including TensorFlow Lite and OpenCV is needed.  
See our [tech blog](https://blog.seeso.io/running-tensorflow-lite-c-in-android-3ff01d42be07) for full instructions.

## Notes
* Initializing can take several seconds depending on your devices.
* Initializing with non-CPU delegates may fail if the hardware doesn't support them
