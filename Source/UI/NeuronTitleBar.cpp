/*
 * NeuronTitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "NeuronTitleBar.hpp"
#include "Styles.hpp"

/*
 * Neuron Title Bar
 */

NeuronTitleBar::NeuronTitleBar(WellsAudioProcessor &p)
    : processor(p), addNeuron(p) {
  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    add_neuron_label(i);
  }
  addAndMakeVisible(addNeuron);
}
NeuronTitleBar::~NeuronTitleBar() {}

void NeuronTitleBar::paint(Graphics &g) {}

void NeuronTitleBar::resized() {
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);
  area.removeFromLeft(AppStyle.rowLabelWidth);

  for (auto it = neuronColumnLabels.begin(); it != neuronColumnLabels.end();
       it++) {
    auto labelArea = area.removeFromLeft(AppStyle.colWidth);
    (*it)->setBounds(labelArea);
  }
  auto buttonArea = area.removeFromLeft(AppStyle.colWidth);
  addNeuron.setBounds(buttonArea);
}

void NeuronTitleBar::add_neuron_label(int neuron_index) {
  std::unique_ptr<NeuronLabel> label =
      std::make_unique<NeuronLabel>(processor, neuron_index);
  addAndMakeVisible(*label);
  neuronColumnLabels.push_back(std::move(label));
}

// Public Methods

void NeuronTitleBar::add_neuron_ui_update() {
  add_neuron_label();
  resized();
}
void NeuronTitleBar::remove_neuron_ui_update() {
  neuronColumnLabels.pop_back();
  resized();
}

void NeuronTitleBar::add_neuron_label() {
  add_neuron_label(neuronColumnLabels.size());
}

/*
 * Neuron Label
 */

NeuronLabel::NeuronLabel(WellsAudioProcessor &p, int idx)
    : processor(p), neuron_index(idx),
      columnLabel("neuron" + String(idx + 1), "Neuron " + String(idx + 1)),
      removeNeuron("-") {
  columnLabel.setColour(Label::ColourIds::textColourId, AppStyle.lightGrey);
  columnLabel.setJustificationType(Justification::centred);
  removeNeuron.onClick = [this]() { processor.remove_neuron_at(neuron_index); };
  addAndMakeVisible(columnLabel);
  addAndMakeVisible(removeNeuron);
}
NeuronLabel::~NeuronLabel() {}

void NeuronLabel::paint(Graphics &g) {}
void NeuronLabel::resized() {
  auto area = getLocalBounds();
  columnLabel.setBounds(area.removeFromBottom(area.getHeight() * 0.6));
  int width = area.getWidth();
  area.removeFromLeft(width * 0.3);
  area.removeFromRight(width * 0.3);
  removeNeuron.setBounds(area);
}

/*
 * Add Neuron Button
 */

AddNeuronButton::AddNeuronButton(WellsAudioProcessor &p)
    : TextButton("+"), processor(p) {
  onClick = [this]() { processor.add_neuron(); };
}
AddNeuronButton::~AddNeuronButton() {}

NeuronTitleBar *AddNeuronButton::getNeuronTitleBar() {
  return static_cast<NeuronTitleBar *>(getParentComponent());
}
