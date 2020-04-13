/*
 * NeuronTitleBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "NeuronTitleBar.hpp"

NeuronTitleBar::NeuronTitleBar() : addNeurons("+") {
  for (int i = 0; i < numNeurons; ++i) {
    std::unique_ptr<Label> label = std::make_unique<Label>(
        "neuron" + String(i + 1), "Neuron " + String(i + 1));
    label->setColour(Label::ColourIds::textColourId, darkGrey);
    label->setJustificationType(Justification::centred);
    addAndMakeVisible(*label);
    neuronColumnLabels.push_back(std::move(label));
  }
  addAndMakeVisible(addNeurons);
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
  addNeurons.setBounds(buttonArea);
}
