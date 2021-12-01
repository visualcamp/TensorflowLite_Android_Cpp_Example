package io.seeso.example.customview;

import io.seeso.lib_cpp_wrapper.Detector.Recognition;

import java.util.List;

public interface ResultsView {
  public void setResults(final List<Recognition> results);
}
