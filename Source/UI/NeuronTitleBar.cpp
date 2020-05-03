/*
 * NeuronTitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "NeuronTitleBar.hpp"

/*
 * Neuron Title Bar
 */

NeuronTitleBar::NeuronTitleBar(WellsAudioProcessor &p)
    : processor(p), addNeuron(p) {
  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    add_neuron_label(i + 1);
  }
  addAndMakeVisible(addNeuron);
}
NeuronTitleBar::~NeuronTitleBar() {}

void NeuronTitleBar::paint(Graphics &g) {}

void NeuronTitleBar::resized() {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);
  area.removeFromLeft(rowLabelPadding);

  for (std::vector<std::unique_ptr<Label>>::iterator it =
           neuronColumnLabels.begin();
       it != neuronColumnLabels.end(); it++) {
    auto labelArea = area.removeFromLeft(colWidth);
    (*it)->setBounds(labelArea);
  }
  auto buttonArea = area.removeFromLeft(colWidth);
  componentPadding.subtractFrom(buttonArea);
  addNeuron.setBounds(buttonArea);
}

void NeuronTitleBar::add_neuron_label(int neuron_num) {
  std::unique_ptr<Label> label = std::make_unique<Label>(
      "neuron" + String(neuron_num), "Neuron " + String(neuron_num));
  label->setColour(Label::ColourIds::textColourId, darkGrey);
  label->setJustificationType(Justification::centred);
  addAndMakeVisible(*label);
  neuronColumnLabels.push_back(std::move(label));
}

// Public Methods

void NeuronTitleBar::add_neuron_ui_update() {
  add_neuron_label();
  resized();
}
void NeuronTitleBar::remove_neuron_ui_update() {}

void NeuronTitleBar::add_neuron_label() {
  add_neuron_label(neuronColumnLabels.size() + 1);
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
