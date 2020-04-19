/*
 * MainComponent.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MainComponent.hpp"

MainComponent::MainComponent(WellsAudioProcessor &p)
    : processor(p), titleBar(p), pluginBody(p) {
  addAndMakeVisible(&titleBar);
  addAndMakeVisible(&pluginBody);
  startTimer(100);
};
MainComponent::~MainComponent(){};

void MainComponent::paint(Graphics &g) {}

void MainComponent::resized() {
  auto area = getLocalBounds();

  titleBar.setBounds(area.removeFromTop(titleHeight));
  pluginBody.setBounds(area.removeFromLeft(area.getWidth() * 0.8));
}

void MainComponent::timerCallback() {
  titleBar.updateComponents();
  pluginBody.updateComponents();
};
