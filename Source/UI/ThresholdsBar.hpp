/*
 * ThresholdsBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

/*
 * Threshold Inc/Dec Slider - controls the threshold for an inidividual neuron
 */

class ThresholdSlider : public Slider {
public:
  ThresholdSlider(WellsAudioProcessor &p, int neuron_num);
  ~ThresholdSlider();

  void updateComponent();

private:
  WellsAudioProcessor &processor;
  int neuron_index;
};

/*
 * Thresholds Bar
 *
 * Controls the thresholds for each of the individual neurons. Using the
 * Threshold Inc/Dec Sliders.
 */

class ThresholdsBar : public Component {
public:
  ThresholdsBar(WellsAudioProcessor &p);
  ~ThresholdsBar();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();
  void add_neuron_ui_update();
  void remove_neuron_ui_update();

private:
  WellsAudioProcessor &processor;

  std::vector<std::unique_ptr<ThresholdSlider>> thresholdSliders;

  void add_threshold_slider(int neuron_index);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThresholdsBar)
};
