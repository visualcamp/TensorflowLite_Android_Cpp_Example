package com.example.lib_cpp_wrapper;

import android.content.Context;
import android.content.res.AssetFileDescriptor;

import androidx.annotation.NonNull;

import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;

public class Detector {
  
  
  
  public Detector() {
    nativeObj = nativeDetector();
  }
  
  public void loadModel(@NonNull Context context, @NonNull String modelFileName) throws IOException {
    ByteBuffer buffer = readFile(context, modelFileName);
    byte[] byteArray = new byte[buffer.remaining()];
    buffer.get(byteArray);
    nativeDetectorLoadModel(nativeObj, byteArray, byteArray.length);
  }
  
  @Override
  protected void finalize() throws Throwable {
  
    super.finalize();
  }
  
  public native String stringFromJNI();
  
  
  private MappedByteBuffer readFile(@NonNull Context context, @NonNull String filePath) throws IOException {
    AssetFileDescriptor fileDescriptor = context.getAssets().openFd(filePath);
  
    MappedByteBuffer buffer;
    try {
      FileInputStream inputStream = new FileInputStream(fileDescriptor.getFileDescriptor());
    
      try {
        FileChannel fileChannel = inputStream.getChannel();
        long startOffset = fileDescriptor.getStartOffset();
        long declaredLength = fileDescriptor.getDeclaredLength();
        buffer = fileChannel.map(FileChannel.MapMode.READ_ONLY, startOffset, declaredLength);
      } catch (Throwable ex) {
        try {
          inputStream.close();
        } catch (Throwable ex2) {
          ex.addSuppressed(ex2);
        }
      
        throw ex;
      }
    
      inputStream.close();
    } catch (Throwable ex3) {
      if (fileDescriptor != null) {
        try {
          fileDescriptor.close();
        } catch (Throwable var10) {
          ex3.addSuppressed(var10);
        }
      }
    
      throw ex3;
    }
  
    if (fileDescriptor != null) {
      fileDescriptor.close();
    }
  
    return buffer;
  }
  
  
  
  private long nativeObj = 0;
  
  private native long nativeDetector();
  private native void nativeDetectorDelete(long obj);
  private native boolean nativeDetectorLoadModel(long obj, byte[] buffer, long buffer_size);
}
