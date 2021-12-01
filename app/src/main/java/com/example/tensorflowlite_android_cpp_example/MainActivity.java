package com.example.tensorflowlite_android_cpp_example;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import com.example.tensorflowlite_android_cpp_example.databinding.ActivityMainBinding;

import com.example.lib_cpp_wrapper.Detector;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {
  
  // Used to load the 'tensorflowlite_android_cpp_example' library on application startup.
  static {
    System.loadLibrary("tensorflowlite_android_cpp_example");
  }
  
  private ActivityMainBinding binding;
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    
    binding = ActivityMainBinding.inflate(getLayoutInflater());
    setContentView(binding.getRoot());
    
    // Example of a call to a native method
    TextView tv = binding.sampleText;
    
    Detector f = new Detector();
    
    try {
      f.loadModel(this, "detect.tflite");
    } catch (final IOException e) {
      Toast.makeText(getApplicationContext(), "Failed to load model file", Toast.LENGTH_LONG).show();
    }
    
    tv.setText(f.stringFromJNI());
  }
}