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
  const int controlsBarHeight{40};

  const int titleBarHeight{40};
  const int titleBarPowerButtonWidth{40};
  const int titleBarRotarySliderWidth{100};
  const int titleBarRangeSliderWidth{160};
  BorderSize<int> titleBarComponentPadding{5, 5, 5, 5};

  BorderSize<int> inputModuleBarComponentPadding{5, 5, 5, 5};
  const int inputModuleBarRotarySliderWidth{120};

  const int colWidth{60};
  const int rowLabelWidth{110};

  const int connectionMatrixTitleHeight{32};
  const int connectionMatrixRowHeight{32};

  const BorderSize<int> componentPadding{3, 3, 3, 3};
  const BorderSize<int> blockPadding{3, 6, 3, 6};
  const BorderSize<int> rowLabelPadding{0, 5, 0, 0};
};

inline PluginStyle AppStyle;

class WellsLookAndFeel : public juce::LookAndFeel_V4 {
public:
  WellsLookAndFeel() { setDefaultSansSerifTypefaceName("Avenir Next"); }
};

class SymbolsLookAndFeel : public WellsLookAndFeel {
public:
  Font getTextButtonFont(TextButton&, int buttonHeight) override
  {
    return Font("Noto Sans Symbols2", 20.0f, Font::plain);
  }
};
