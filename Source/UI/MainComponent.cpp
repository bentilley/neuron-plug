/*
 * MainComponent.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MainComponent.hpp"

MainComponent::MainComponent() {
  setSize(400, 300);

  midiVolume.setSliderStyle(Slider::LinearBarVertical);
  midiVolume.setRange(0.0, 127.0, 1.0);
  midiVolume.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  midiVolume.setPopupDisplayEnabled(true, false, this);
  midiVolume.setTextValueSuffix(" Volume");
  midiVolume.setValue(1.0);
  midiVolume.addListener(this);

  // adds the slider to the editor
  addAndMakeVisible(&midiVolume);
};
MainComponent::~MainComponent(){};

void MainComponent::paint(Graphics &g) {
  g.fillAll(Colours::lightblue);

  g.setColour(Colours::darkblue);
  g.setFont(20.0f);
  g.drawText(currentSizeAsString, getLocalBounds(), Justification::centred,
             true);
}

void MainComponent::resized() {
  currentSizeAsString = String(getWidth()) + " x " + String(getHeight());
  // sets the position and size of the slider with arguments (x, y, width,
  // height)
  midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

// This is the call back for when the slider value is changed
void MainComponent::sliderValueChanged(Slider *slider) {
  /* processor.noteOnVel = midiVolume.getValue(); */
}
