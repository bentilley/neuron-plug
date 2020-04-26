/*
 * ConnectionWeightsMatrix.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

class ConnectionWeightsMatrix : public Component {
public:
  ConnectionWeightsMatrix(WellsAudioProcessor &p);
  ~ConnectionWeightsMatrix();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();

private:
  int numNeurons{3};

  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  BorderSize<int> blockPadding{5, 5, 5, 5};
  int titleHeight = 50;
  int rowLabelWidth{120};
  int rowHeight{50};
  BorderSize<int> rowLabelPadding{0, 10, 0, 0};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};

  std::vector<std::unique_ptr<Label>> neuronRowLabels;
  std::vector<std::vector<std::unique_ptr<Slider>>> connectionWeightSliders;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionWeightsMatrix)
};
