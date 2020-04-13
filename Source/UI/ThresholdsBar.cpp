/*
 * ThresholdsBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "ThresholdsBar.hpp"

ThresholdsBar::ThresholdsBar() {
  for (int i = 0; i < numNeurons; ++i) {
    std::unique_ptr<Slider> slider =
        std::make_unique<Slider>("thresholdSlider" + String(i));
    slider->setSliderStyle(Slider::IncDecButtons);
    slider->setRange(1, 256, 1);
    slider->setColour(Slider::ColourIds::textBoxBackgroundColourId, darkGrey);
    addAndMakeVisible(*slider);
    thresholdSliders.push_back(std::move(slider));
  }
}
ThresholdsBar::~ThresholdsBar() {}

void ThresholdsBar::paint(Graphics &g) {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);

  g.setColour(lightGrey);
  g.fillRoundedRectangle(area.toFloat(), 10.0);
  g.setColour(darkGrey);
  g.drawRoundedRectangle(area.toFloat(), 10.0, 1.0);

  auto rowLabelArea = area.removeFromLeft(rowLabelWidth);
  rowLabelPadding.subtractFrom(rowLabelArea);
  g.setFont(16.0f);
  g.drawText("Thresholds", rowLabelArea, Justification::centredLeft, true);
}

void ThresholdsBar::resized() {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);
  area.removeFromLeft(rowLabelWidth);

  for (auto slider = thresholdSliders.begin(); slider != thresholdSliders.end();
       slider++) {
    auto componentArea = area.removeFromLeft(colWidth);
    componentPadding.subtractFrom(componentArea);
    (*slider)->setBounds(componentArea);
  }
}
