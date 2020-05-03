/*
 * PluginBody.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "PluginBody.hpp"

PluginBody::PluginBody(WellsAudioProcessor &p)
    : processor(p), editor_num_neurons{p.midiGenerator->num_neurons()},
      neuronTitleBar(p), midiNotesBar(p), inputWeightsBar(p), thresholdsBar(p),
      connectionWeightsMatrix(p) {
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

void PluginBody::updateComponents() {
  midiNotesBar.updateComponents();
  inputWeightsBar.updateComponents();
  thresholdsBar.updateComponents();
  connectionWeightsMatrix.updateComponents();

  int processor_num_neurons = processor.midiGenerator->num_neurons();
  if (processor_num_neurons != editor_num_neurons) {
    int neuron_diff = processor_num_neurons - editor_num_neurons;
    editor_num_neurons = processor_num_neurons;
    update_neuron_ui(neuron_diff);
  }
}

void PluginBody::update_neuron_ui(int neuron_num_change) {
  if (neuron_num_change > 0) {
    for (int i{0}; i < neuron_num_change; ++i) {
      add_neuron_ui_update();
    }
  } else if (neuron_num_change < 0) {
    for (int i{0}; i < -neuron_num_change; ++i) {
      remove_neuron_ui_update();
    }
  }
}

void PluginBody::add_neuron_ui_update() {
  neuronTitleBar.add_neuron_ui_update();
  midiNotesBar.add_neuron_ui_update();
  inputWeightsBar.add_neuron_ui_update();
  thresholdsBar.add_neuron_ui_update();
  connectionWeightsMatrix.add_neuron_ui_update();
}
void PluginBody::remove_neuron_ui_update() {
  neuronTitleBar.remove_neuron_ui_update();
  midiNotesBar.remove_neuron_ui_update();
  inputWeightsBar.remove_neuron_ui_update();
  thresholdsBar.remove_neuron_ui_update();
  connectionWeightsMatrix.remove_neuron_ui_update();
}
