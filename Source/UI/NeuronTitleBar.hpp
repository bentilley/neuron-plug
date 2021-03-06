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
 * Neuron Label - label a column of neuron parameters, and used to remove neuron
 */

class NeuronLabel : public Component {
public:
  NeuronLabel(WellsAudioProcessor &p, int neuron_index);
  ~NeuronLabel();

  void paint(Graphics &) override;
  void resized() override;

private:
  WellsAudioProcessor &processor;
  int neuron_index;

  Colour darkGrey{51, 51, 51};

  Label columnLabel;
  TextButton removeNeuron;
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

  AddNeuronButton addNeuron;
  std::vector<std::unique_ptr<NeuronLabel>> neuronColumnLabels;

  void add_neuron_label(int neuron_index);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuronTitleBar)
};
