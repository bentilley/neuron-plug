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

ConnectionWeightsMatrix::ConnectionWeightsMatrix(WellsAudioProcessor &p) {
  for (int i = 0; i < numNeurons; ++i) {
    std::unique_ptr<Label> label = std::make_unique<Label>(
        "neuron" + String(i + 1), "Neuron " + String(i + 1));
    label->setColour(Label::ColourIds::textColourId, darkGrey);
    addAndMakeVisible(*label);
    neuronRowLabels.push_back(std::move(label));
  }

  for (int i = 0; i < numNeurons; ++i) {
    std::vector<std::unique_ptr<Slider>> sliderRow{};
    for (int j = 0; j < numNeurons; ++j) {
      std::unique_ptr<Slider> slider = std::make_unique<Slider>(
          "connectionWeightSlider" + String(i) + "-" + String(j));
      slider->setSliderStyle(Slider::IncDecButtons);
      slider->setRange(1, 256, 1);
      slider->setColour(Slider::ColourIds::textBoxBackgroundColourId, darkGrey);
      addAndMakeVisible(*slider);
      sliderRow.push_back(std::move(slider));
    }
    connectionWeightSliders.push_back(std::move(sliderRow));
  }
}
ConnectionWeightsMatrix::~ConnectionWeightsMatrix() {}

void ConnectionWeightsMatrix::paint(Graphics &g) {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);

  g.setColour(lightGrey);
  g.fillRoundedRectangle(area.toFloat(), 10.0);
  g.setColour(darkGrey);
  g.drawRoundedRectangle(area.toFloat(), 10.0, 1.0);

  auto titleArea = area.removeFromTop(titleHeight);
  rowLabelPadding.subtractFrom(titleArea);
  g.setFont(16.0f);
  g.drawText("Connection Weights", titleArea, Justification::centredLeft, true);
}

void ConnectionWeightsMatrix::resized() {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);
  area.removeFromTop(titleHeight);

  for (int i = 0; i < numNeurons; ++i) {
    auto rowArea = area.removeFromTop(rowHeight);
    neuronRowLabels.at(i)->setBounds(rowArea.removeFromLeft(rowLabelWidth));
    for (int j = 0; j < numNeurons; ++j) {
      auto sliderArea = rowArea.removeFromLeft(colWidth);
      componentPadding.subtractFrom(sliderArea);
      connectionWeightSliders.at(i).at(j)->setBounds(sliderArea);
    }
  }
}

void ConnectionWeightsMatrix::updateComponents() {
  for (auto sliderRow = connectionWeightSliders.begin();
       sliderRow != connectionWeightSliders.end(); ++sliderRow) {
    for (auto slider = sliderRow->begin(); slider != sliderRow->end();
         ++slider) {
      (*slider)->updateComponent();
    }
  }
}
