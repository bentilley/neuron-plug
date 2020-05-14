/*
 * Styles.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct PluginStyle {
  // Colours
  const Colour lightGrey{221, 221, 221};
  const Colour mediumGrey{157, 157, 157};
  const Colour darkGrey{57, 57, 57};
  const Colour veryDarkGrey{45, 45, 45};
  const Colour buttonOnColour{133, 150, 241};
  const Colour buttonOffColour{39, 44, 71};

  // Text
  float fontSizeMedium{16.0f};

  // Dimensions
  const int neuronTitleBarHeight{40};
  const int controlsBarHeight{50};

  const int titleHeight{80};
  const int colWidth{80};
  const int rowLabelWidth{110};

  const int connectionMatrixTitleHeight{40};
  const int connectionMatrixRowHeight{40};

  const BorderSize<int> componentPadding{3, 3, 3, 3};
  const BorderSize<int> blockPadding{5, 10, 5, 10};
  const BorderSize<int> rowLabelPadding{0, 10, 0, 0};
};

inline PluginStyle AppStyle;
