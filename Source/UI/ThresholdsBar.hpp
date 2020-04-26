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
  Colour darkGrey{51, 51, 51};

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

private:
  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  int rowLabelWidth{120};
  BorderSize<int> rowLabelPadding{0, 10, 0, 0};
  int numNeurons{3};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  BorderSize<int> blockPadding{5, 5, 5, 5};

  std::vector<std::unique_ptr<ThresholdSlider>> thresholdSliders;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThresholdsBar)
};
