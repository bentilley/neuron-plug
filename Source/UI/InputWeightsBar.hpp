/*
 * InputWeightsBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

/*
 * Input Weight Inc/Dec Sliders - control the input weight of each neuron
 */

class InputWeightSlider : public Slider {
public:
  InputWeightSlider(WellsAudioProcessor &p, int neuron_num);
  ~InputWeightSlider();

  void updateComponent();

private:
  Colour darkGrey{51, 51, 51};

  WellsAudioProcessor &processor;
  int neuron_index;
};

/*
 * Input Weights Bar
 *
 * Used to control the input weights of each neuron. This is done through the
 * Input Weight Inc/Dec Sliders.
 */

class InputWeightsBar : public Component {
public:
  InputWeightsBar(WellsAudioProcessor &p);
  ~InputWeightsBar();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();

private:
  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  int rowLabelWidth{120};
  BorderSize<int> rowLabelPadding{0, 10, 0, 0};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  BorderSize<int> blockPadding{5, 5, 5, 5};

  std::vector<std::unique_ptr<InputWeightSlider>> inputWeightSliders;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputWeightsBar)
};
