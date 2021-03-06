package io.seeso.lib_cpp_wrapper;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.RectF;

import androidx.annotation.NonNull;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.List;

public class Detector {
  private ArrayList<String> labels;
  
  static {
    System.loadLibrary("seeso_tfl_cpp_native");
  }
  
  public Detector() { nativeObj = nativeDetector(); }
  
  public Detector(
    @NonNull Context context,
    @NonNull String modelFileName,
    @NonNull String labelFileName) throws IOException {
    nativeObj = nativeDetector();
    
    loadModel(context, modelFileName);
    loadLabel(context, labelFileName);
  }
  
  private void loadModel(Context context, String modelFileName) throws IOException {
    ByteBuffer buffer = readFile(context, modelFileName);
    modelBuffer = new byte[buffer.remaining()];
    buffer.get(modelBuffer);
    buildInterpreter();
  }
  
  private void loadLabel(Context context, String file) throws IOException {
    BufferedReader reader = null;
    try {
      reader = new BufferedReader(
        new InputStreamReader(context.getAssets().open(file)));
  
      labels = new ArrayList<>();
      String line;
      while ((line = reader.readLine()) != null) {
        labels.add(line);
      }
    } finally {
      if (reader != null) {
        reader.close();
      }
    }
  }
  
  public void buildInterpreter() {
    nativeDetectorLoadModel(nativeObj, modelBuffer, modelBuffer.length);
    boolean success = nativeBuildInterpreter(nativeObj);
    if (!success)
      throw new RuntimeException("Failed to build interpreter. See logs for more info");
  }
  
  public void rebuildInterpreter() {
    nativeResetInterpreter(nativeObj);
    buildInterpreter();
  }
  
  public void setNumThreads(int num) {
    nativeSetNumThreads(nativeObj, num);
    rebuildInterpreter();
  }
  
  public void setUseCPU() {
    nativeSetUseCPU(nativeObj);
    rebuildInterpreter();
  }
  public void setUseNnAPI() {
    nativeSetUseNnApi(nativeObj);
    rebuildInterpreter();
  }
  public void setUseGPU() {
    nativeSetUseGPU(nativeObj);
    rebuildInterpreter();
  }
  public void setUseXNNPack() {
    nativeSetUseXNNPack(nativeObj);
    rebuildInterpreter();
  }
  
  private RectF getBoundingBox(float top, float left, float bottom, float right, int w, int h) {
    RectF box = new RectF(left, top, right, bottom);
    box.top *= h;
    box.bottom *= h;
    box.left *= w;
    box.right *= w;
    return box;
  }
  
  // TODO(Tony): Pass raw image to C++
  public List<Recognition> recognizeImage(Bitmap image) {
    final int width = image.getWidth();
    final int height = image.getHeight();
  
    final int size = image.getRowBytes() * image.getHeight();
    ByteBuffer byteBuffer = ByteBuffer.allocate(size);
    image.copyPixelsToBuffer(byteBuffer);
    
    nativeAddImage(nativeObj, byteBuffer.array());
    
    nativeInvoke(nativeObj);
    
    final float[] locations = nativeGetOutput(nativeObj, 0);
    final float[] classes = nativeGetOutput(nativeObj, 1);
    final float[] scores = nativeGetOutput(nativeObj, 2);
    final int numDetection = Math.round(nativeGetOutput(nativeObj, 3)[0]);
    
    final ArrayList<Recognition> recognitions = new ArrayList<>();
    
    for (int i = 0; i < numDetection; ++i) {
      recognitions.add(new Recognition(
        "" + i,
        labels.get(Math.round(classes[i])),
        scores[i],
        getBoundingBox(locations[i*4], locations[i*4 + 1], locations[i*4+2], locations[i*4 + 3], width, height)
      ));
    }
    
    return recognitions;
  }
  
  @Override
  protected void finalize() throws Throwable {
  
    super.finalize();
  }
  
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
  private byte[] modelBuffer;
  
  private native long nativeDetector();
  private native void nativeDetectorDelete(long obj);
  private native boolean nativeDetectorLoadModel(long obj, byte[] buffer, long buffer_size);
  private native boolean nativeBuildInterpreter(long obj);
  private native void nativeResetInterpreter(long obj);
  private native void nativeSetNumThreads(long obj, int num);
  private native void nativeSetUseCPU(long obj);
  private native void nativeSetUseGPU(long obj);
  private native void nativeSetUseNnApi(long obj);
  private native void nativeSetUseXNNPack(long obj);
  
  private native void nativeAddImage(long obj, byte[] data);
  private native void nativeInvoke(long obj);
  private native float[] nativeGetOutput(long obj, int index);
}
