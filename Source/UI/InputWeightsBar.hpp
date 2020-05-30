/*
 * InputWeightsBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
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
  void add_neuron_ui_update();
  void remove_neuron_ui_update();

private:
  WellsAudioProcessor &processor;

  std::vector<std::unique_ptr<InputWeightSlider>> inputWeightSliders;

  void add_input_weight_slider(int neuron_index);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputWeightsBar)
};
