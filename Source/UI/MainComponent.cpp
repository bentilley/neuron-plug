/*
 * MainComponent.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MainComponent.hpp"

MainComponent::MainComponent() {

  addAndMakeVisible(&titleBar);
};
MainComponent::~MainComponent(){};

void MainComponent::paint(Graphics &g) {}

void MainComponent::resized() {
  auto area = getLocalBounds();

  titleBar.setBounds(area.removeFromTop(titleHeight));
}
