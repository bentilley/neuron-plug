/*
 * NeuronTitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include <memory>

/*
 * Add Neuron Button - add neurons to the network
 */

class NeuronTitleBar;

class AddNeuronButton : public TextButton {
public:
  AddNeuronButton(WellsAudioProcessor &p);
  ~AddNeuronButton();

private:
  WellsAudioProcessor &processor;

  NeuronTitleBar *getNeuronTitleBar();
};

/*
 * Neuron Title Bar
 *
 * This is the space at the top of the Plugin Body that shows the column labels
 * for each of the neurons. Crucially though it also contains the Add Neuron
 * Button, which is used to add neurons to the network.
 */

class NeuronTitleBar : public Component {
public:
  NeuronTitleBar(WellsAudioProcessor &p);
  ~NeuronTitleBar();

  void paint(Graphics &) override;
  void resized() override;

  void add_neuron_ui_update();
  void remove_neuron_ui_update();

  void add_neuron_label();

private:
  WellsAudioProcessor &processor;

  /* Colour lightGrey{221, 221, 221}; */
  Colour darkGrey{51, 51, 51};

  int rowLabelPadding{120};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  BorderSize<int> blockPadding{0, 5, 0, 5};

  AddNeuronButton addNeuron;
  std::vector<std::unique_ptr<Label>> neuronColumnLabels;

  void add_neuron_label(int neuron_num);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuronTitleBar)
};
