/*
 * ConnectionWeightsMatrix.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

/*
 * Neuron Row Labels - label each row on connection weights
 */

class NeuronRowLabel : public Label {
public:
  NeuronRowLabel(int neuron_index);
  ~NeuronRowLabel();

private:
  Colour darkGrey{51, 51, 51};
};

/*
 * Connection Weight Slider - controls the connection weight between two neurons
 */

class ConnectionWeightSlider : public Slider {
public:
  ConnectionWeightSlider(WellsAudioProcessor &p, int from, int to);
  ~ConnectionWeightSlider();

  void updateComponent();

private:
  Colour darkGrey{51, 51, 51};

  WellsAudioProcessor &processor;
  int neuron_from, neuron_to;
};

/*
 * Connection Weight Matrix
 *
 * Used to control the connection weights between all of the neurons. It does
 * this using the connection weight sliders.
 */

class ConnectionWeightsMatrix : public Component {
public:
  ConnectionWeightsMatrix(WellsAudioProcessor &p);
  ~ConnectionWeightsMatrix();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();
  void add_neuron_ui_update();
  void remove_neuron_ui_update();

private:
  WellsAudioProcessor &processor;

  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  BorderSize<int> blockPadding{5, 10, 5, 10};
  int titleHeight = 40;
  int rowLabelWidth{120};
  int rowHeight{40};
  BorderSize<int> rowLabelPadding{0, 10, 0, 0};
  int colWidth{80};
  BorderSize<int> componentPadding{5, 5, 5, 5};

  std::vector<std::unique_ptr<NeuronRowLabel>> neuronRowLabels;
  std::vector<std::vector<std::unique_ptr<ConnectionWeightSlider>>>
      connectionWeightSliders;

  void add_neuron_row_label(int neuron_index);
  void add_connection_weight_slider(int neuron_index);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionWeightsMatrix)
};
