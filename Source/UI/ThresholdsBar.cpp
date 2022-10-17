/*
 * ThresholdsBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "ThresholdsBar.hpp"
#include "Styles.hpp"

/*
 * Threshold Bar
 */

ThresholdsBar::ThresholdsBar(WellsAudioProcessor& p) : processor(p)
{
  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    std::unique_ptr<ThresholdSlider> slider = std::make_unique<ThresholdSlider>(p, i);
    addAndMakeVisible(*slider);
    thresholdSliders.push_back(std::move(slider));
  }
}
ThresholdsBar::~ThresholdsBar() {}

void ThresholdsBar::paint(Graphics& g)
{
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);

  g.setColour(AppStyle.darkGrey);
  g.fillRoundedRectangle(area.toFloat(), 5.0);
  g.setColour(AppStyle.mediumGrey);
  g.drawRoundedRectangle(area.toFloat(), 5.0, 1.0);

  auto rowLabelArea = area.removeFromLeft(AppStyle.rowLabelWidth);
  AppStyle.rowLabelPadding.subtractFrom(rowLabelArea);
  g.setFont(AppStyle.fontSizeMedium);
  g.setColour(AppStyle.lightGrey);
  g.drawText("Thresholds", rowLabelArea, Justification::centredLeft, true);
}

void ThresholdsBar::resized()
{
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);
  area.removeFromLeft(AppStyle.rowLabelWidth);

  for (auto slider = thresholdSliders.begin(); slider != thresholdSliders.end(); slider++) {
    auto componentArea = area.removeFromLeft(AppStyle.colWidth);
    AppStyle.componentPadding.subtractFrom(componentArea);
    (*slider)->setBounds(componentArea);
  }
}

void ThresholdsBar::add_threshold_slider(int neuron_index)
{
  std::unique_ptr<ThresholdSlider> slider =
    std::make_unique<ThresholdSlider>(processor, neuron_index);
  addAndMakeVisible(*slider);
  thresholdSliders.push_back(std::move(slider));
}

// Public Methods

void ThresholdsBar::updateComponents()
{
  for (auto slider = thresholdSliders.begin(); slider != thresholdSliders.end(); ++slider) {
    (*slider)->updateComponent();
  }
}

void ThresholdsBar::add_neuron_ui_update()
{
  add_threshold_slider(thresholdSliders.size());
  resized();
}
void ThresholdsBar::remove_neuron_ui_update()
{
  thresholdSliders.pop_back();
  resized();
}

/*
 * Threshold Slider
 */

ThresholdSlider::ThresholdSlider(WellsAudioProcessor& p, int i)
    : Slider("thresholdSlider" + String(i)), processor(p), neuron_index(i)
{
  setSliderStyle(Slider::IncDecButtons);
  setRange(-256, 256, 1);
  setColour(Slider::ColourIds::textBoxBackgroundColourId, AppStyle.darkGrey);
  onValueChange = [this]() {
    processor.midiGenerator->set_neuron_threshold(neuron_index, getValue());
  };
}
ThresholdSlider::~ThresholdSlider() {}

void ThresholdSlider::updateComponent()
{
  setValue(processor.midiGenerator->get_neuron_threshold(neuron_index));
}
