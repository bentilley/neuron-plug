/*
 * PluginBody.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "PluginBody.hpp"

PluginBody::PluginBody(WellsAudioProcessor &p) : midiNotesBar(p) {
  addAndMakeVisible(neuronTitleBar);
  addAndMakeVisible(midiNotesBar);
  addAndMakeVisible(inputWeightsBar);
  addAndMakeVisible(thresholdsBar);
  addAndMakeVisible(connectionWeightsMatrix);
}
PluginBody::~PluginBody() {}

void PluginBody::paint(Graphics &g) {}

void PluginBody::resized() {
  auto area = getLocalBounds();

  int neuronTitleBarHeight{40};
  neuronTitleBar.setBounds(area.removeFromTop(neuronTitleBarHeight));

  int controlsBarHeight{80};
  midiNotesBar.setBounds(area.removeFromTop(controlsBarHeight));
  inputWeightsBar.setBounds(area.removeFromTop(controlsBarHeight));
  thresholdsBar.setBounds(area.removeFromTop(controlsBarHeight));

  connectionWeightsMatrix.setBounds(area);
}

void PluginBody::updateComponents() { midiNotesBar.updateComponents(); }
