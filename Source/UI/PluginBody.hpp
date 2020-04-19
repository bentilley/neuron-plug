/*
 * PluginBody.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "ConnectionWeightsMatrix.hpp"
#include "InputWeightsBar.hpp"
#include "MidiNotesBar.hpp"
#include "NeuronTitleBar.hpp"
#include "ThresholdsBar.hpp"

class PluginBody : public Component {
public:
  PluginBody(WellsAudioProcessor &p);
  ~PluginBody();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();

private:
  /* Colour lightGrey{221, 221, 221}; */
  Colour darkGrey{51, 51, 51};

  NeuronTitleBar neuronTitleBar;
  MidiNotesBar midiNotesBar;
  InputWeightsBar inputWeightsBar;
  ThresholdsBar thresholdsBar;
  ConnectionWeightsMatrix connectionWeightsMatrix;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginBody)
};
