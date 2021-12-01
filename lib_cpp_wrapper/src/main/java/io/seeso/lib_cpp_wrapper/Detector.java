package io.seeso.lib_cpp_wrapper;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.RectF;
import android.media.Image;

import androidx.annotation.NonNull;

import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.List;

public class Detector {
  
  static {
    System.loadLibrary("seeso_tfl_cpp_native");
  }
  
  public Detector() { nativeObj = nativeDetector(); }
  
  public Detector(@NonNull Context context, @NonNull String modelFileName) throws IOException {
    nativeObj = nativeDetector();
    
    loadModel(context, modelFileName);
    
  }
  
  public void loadModel(@NonNull Context context, @NonNull String modelFileName) throws IOException {
    ByteBuffer buffer = readFile(context, modelFileName);
    byte[] byteArray = new byte[buffer.remaining()];
    buffer.get(byteArray);
    nativeDetectorLoadModel(nativeObj, byteArray, byteArray.length);
  }
  
  public void buildInterpreter() {
    nativeBuildInterpreter(nativeObj);
  }
  
  public void rebuildInterpreter() {
    nativeResetInterpreter(nativeObj);
    buildInterpreter();
  }
  
  public void setNumThreads(int num) { nativeSetCpuNumthreads(nativeObj, num); }
  
  public void setUseCPU()   { nativeSetUseCPU(nativeObj);   }
  public void setUseNnAPI() { nativeSetUseNnApi(nativeObj); }
  public void setUseGPU()   { nativeSetUseGPU(nativeObj);   }
  
  public boolean isProcessing() {
    return nativeIsProcessing(nativeObj);
  }
  
  public List<Recognition> recognizeImage(Bitmap image) {
    final ArrayList<Recognition> recognitions = new ArrayList<>();
    return recognitions;
  }
  
  @Override
  protected void finalize() throws Throwable {
  
    super.finalize();
  }
  
  public native String stringFromJNI();
  
  public class Recognition {
    /**
     * A unique identifier for what has been recognized. Specific to the class, not the instance of
     * the object.
     */
    private final String id;
    
    /** Display name for the recognition. */
    private final String title;
    
    /**
     * A sortable score for how good the recognition is relative to others. Higher should be better.
     */
    private final Float confidence;
    
    /** Optional location within the source image for the location of the recognized object. */
    private RectF location;
    
    public Recognition(
      final String id, final String title, final Float confidence, final RectF location) {
      this.id = id;
      this.title = title;
      this.confidence = confidence;
      this.location = location;
    }
    
    public String getId() {
      return id;
    }
    
    public String getTitle() {
      return title;
    }
    
    public Float getConfidence() {
      return confidence;
    }
    
    public RectF getLocation() {
      return new RectF(location);
    }
    
    public void setLocation(RectF location) {
      this.location = location;
    }
    
    @Override
    public String toString() {
      String resultString = "";
      if (id != null) {
        resultString += "[" + id + "] ";
      }
      
      if (title != null) {
        resultString += title + " ";
      }
      
      if (confidence != null) {
        resultString += String.format("(%.1f%%) ", confidence * 100.0f);
      }
      
      if (location != null) {
        resultString += location + " ";
      }
      
      return resultString.trim();
    }
  }
  
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
  private native void nativeBuildInterpreter(long obj);
  private native void nativeResetInterpreter(long obj);
  private native boolean nativeIsProcessing(long obj);
  private native void nativeSetCpuNumthreads(long obj, int num);
  private native void nativeSetUseCPU(long obj);
  private native void nativeSetUseGPU(long obj);
  private native void nativeSetUseNnApi(long obj);
}
