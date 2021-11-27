package com.example.tensorflowlite_android_cpp_example;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.tensorflowlite_android_cpp_example.databinding.ActivityMainBinding;

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
    tv.setText(stringFromJNI());
  }
  
  /**
   * A native method that is implemented by the 'tensorflowlite_android_cpp_example' native library,
   * which is packaged with this application.
   */
  public native String stringFromJNI();
}