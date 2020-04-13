/*
 * NeuronTitleBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>

class NeuronTitleBar : public Component {
public:
  NeuronTitleBar();
  ~NeuronTitleBar();

  void paint(Graphics &) override;
  void resized() override;

private:
  /* Colour lightGrey{221, 221, 221}; */
  Colour darkGrey{51, 51, 51};

  int rowLabelPadding{120};
  int numNeurons{3};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  BorderSize<int> blockPadding{0, 5, 0, 5};

  TextButton addNeurons;
  std::vector<std::unique_ptr<Label>> neuronColumnLabels;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuronTitleBar)
};
