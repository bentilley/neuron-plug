/*
 * MainComponent.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MainComponent.hpp"
#include "Styles.hpp"

MainComponent::MainComponent(WellsAudioProcessor &p)
    : processor(p), titleBar(p), pluginBody(p) {
  addAndMakeVisible(&titleBar);
  addAndMakeVisible(&pluginBody);
  startTimer(100);
};
MainComponent::~MainComponent(){};

void MainComponent::paint(Graphics &g) {
  auto area = getLocalBounds();
  g.setColour(AppStyle.veryDarkGrey);
  g.fillRect(area);
}

void MainComponent::resized() {
  auto area = getLocalBounds();

  titleBar.setBounds(area.removeFromTop(AppStyle.titleHeight));
  pluginBody.setBounds(area);
}

void MainComponent::timerCallback() {
  titleBar.updateComponents();
  pluginBody.updateComponents();
};
