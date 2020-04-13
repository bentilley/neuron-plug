/*
 * InputWeightsBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class InputWeightsBar : public Component {
public:
  InputWeightsBar();
  ~InputWeightsBar();

  void paint(Graphics &) override;
  void resized() override;

private:
  Colour lightGrey{221, 221, 221};
  Colour darkGrey{51, 51, 51};

  int rowLabelWidth{120};
  BorderSize<int> rowLabelPadding{0, 10, 0, 0};
  int numNeurons{3};
  int colWidth{80};
  BorderSize<int> componentPadding{10, 5, 10, 5};
  BorderSize<int> blockPadding{5, 5, 5, 5};

  std::vector<std::unique_ptr<Slider>> inputWeightSliders;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputWeightsBar)
};
