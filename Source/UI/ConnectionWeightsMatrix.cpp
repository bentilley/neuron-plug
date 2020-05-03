/*
 * ConnectionWeightsMatrix.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "ConnectionWeightsMatrix.hpp"

/*
 * Connection Weight Matrix
 */

ConnectionWeightsMatrix::ConnectionWeightsMatrix(WellsAudioProcessor &p)
    : processor(p) {
  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    add_neuron_row_label(i);
  }

  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    add_connection_weight_slider(i);
  }
}
ConnectionWeightsMatrix::~ConnectionWeightsMatrix() {}

void ConnectionWeightsMatrix::paint(Graphics &g) {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);

  g.setColour(lightGrey);
  g.fillRoundedRectangle(area.toFloat(), 5.0);
  g.setColour(darkGrey);
  g.drawRoundedRectangle(area.toFloat(), 5.0, 1.0);

  auto titleArea = area.removeFromTop(titleHeight);
  rowLabelPadding.subtractFrom(titleArea);
  g.setFont(16.0f);
  g.drawText("Connection Weights", titleArea, Justification::centredLeft, true);
}

void ConnectionWeightsMatrix::resized() {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);
  area.removeFromTop(titleHeight);

  for (int i = 0; i < neuronRowLabels.size(); ++i) {
    auto rowArea = area.removeFromTop(rowHeight);
    neuronRowLabels.at(i)->setBounds(rowArea.removeFromLeft(rowLabelWidth));
    for (int j = 0; j < connectionWeightSliders.size(); ++j) {
      auto sliderArea = rowArea.removeFromLeft(colWidth);
      componentPadding.subtractFrom(sliderArea);
      connectionWeightSliders.at(i).at(j)->setBounds(sliderArea);
    }
  }
}

void ConnectionWeightsMatrix::add_neuron_row_label(int neuron_index) {
  std::unique_ptr<NeuronRowLabel> label =
      std::make_unique<NeuronRowLabel>(neuron_index);
  label->setJustificationType(Justification::centred);
  addAndMakeVisible(*label);
  neuronRowLabels.push_back(std::move(label));
}

void ConnectionWeightsMatrix::add_connection_weight_slider(int neuron_index) {
  for (int i{0}; i < connectionWeightSliders.size(); ++i) {
    std::unique_ptr<ConnectionWeightSlider> slider =
        std::make_unique<ConnectionWeightSlider>(processor, i, neuron_index);
    addAndMakeVisible(*slider);
    connectionWeightSliders.at(i).push_back(std::move(slider));
  }

  std::vector<std::unique_ptr<ConnectionWeightSlider>> sliderRow{};
  for (int j = 0; j < connectionWeightSliders.size() + 1; ++j) {
    std::unique_ptr<ConnectionWeightSlider> slider =
        std::make_unique<ConnectionWeightSlider>(processor, neuron_index, j);
    addAndMakeVisible(*slider);
    sliderRow.push_back(std::move(slider));
  }
  connectionWeightSliders.push_back(std::move(sliderRow));
}

// Public Methods

void ConnectionWeightsMatrix::updateComponents() {
  for (auto sliderRow = connectionWeightSliders.begin();
       sliderRow != connectionWeightSliders.end(); ++sliderRow) {
    for (auto slider = sliderRow->begin(); slider != sliderRow->end();
         ++slider) {
      (*slider)->updateComponent();
    }
  }
}

void ConnectionWeightsMatrix::add_neuron_ui_update() {
  add_neuron_row_label(neuronRowLabels.size());
  add_connection_weight_slider(connectionWeightSliders.size());
  resized();
}
void ConnectionWeightsMatrix::remove_neuron_ui_update() {}

/*
 * Neuron Row Label
 */

NeuronRowLabel::NeuronRowLabel(int i)
    : Label("neuron" + String(i + 1), "Neuron " + String(i + 1)) {
  setColour(Label::ColourIds::textColourId, darkGrey);
}
NeuronRowLabel::~NeuronRowLabel() {}

/*
 * Connection Weight Slider
 */

ConnectionWeightSlider::ConnectionWeightSlider(WellsAudioProcessor &p, int from,
                                               int to)
    : Slider("connectionWeightSlider" + String(from) + "-" + String(to)),
      processor(p), neuron_from(from), neuron_to(to) {
  setSliderStyle(Slider::IncDecButtons);
  setRange(-256, 256, 1);
  setColour(Slider::ColourIds::textBoxBackgroundColourId, darkGrey);
  onValueChange = [this]() {
    processor.midiGenerator->set_neuron_connection_weight(
        neuron_from, neuron_to, getValue());
  };
}
ConnectionWeightSlider::~ConnectionWeightSlider() {}

void ConnectionWeightSlider::updateComponent() {
  setValue(processor.midiGenerator->get_neuron_connection_weight(neuron_from,
                                                                 neuron_to));
}
