/*
 * RotarySliderWithLabel.hpp
 * Copyright (C) 2022 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginProcessor.h"
#include "../Styles.hpp"

/** Rotary slider with a text label to the left of the knob. */
class RotarySliderWithLabel : public Component {
public:
  RotarySliderWithLabel(WellsAudioProcessor& p, String labelText)
      : label(labelText + "Label", labelText), slider(), processor(p)
  {
    slider.setSliderStyle(Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 10, 0);
    slider.setPopupDisplayEnabled(true, false, getParentComponent());

    label.setJustificationType(Justification::right);
    label.setColour(Label::ColourIds::textColourId, AppStyle.lightGrey);

    addAndMakeVisible(&label);
    addAndMakeVisible(&slider);
  }

  void resized()
  {
    auto area = getLocalBounds();
    slider.setBounds(area.removeFromRight(area.getHeight()));
    label.setBounds(area);
  }

protected:
  Label label;
  Slider slider;
  WellsAudioProcessor& processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySliderWithLabel)
};
