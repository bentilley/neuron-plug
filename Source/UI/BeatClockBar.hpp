/*
 * BeatClockBar.hpp
 * Copyright (C) 2022 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "Components/OnOffButton.hpp"
#include "Components/RotarySliderWithLabel.hpp"
#include "Styles.hpp"

/** Turn the beat clock input on or off. */
class BeatClockOnOffButton : public OnOffButton {
public:
  BeatClockOnOffButton(WellsAudioProcessor& p) : OnOffButton(p)
  {
    setTooltip("turn beat clock input on or off");
    onClick = [this]() { processor.midiGenerator->toggleBeatClockIsOn(); };
  }
  void updateComponent()
  {
    setColour(
      TextButton::ColourIds::buttonColourId,
      processor.midiGenerator->getBeatClockIsOn() ? AppStyle.buttonOnColour
                                                  : AppStyle.buttonOffColour
    );
  }
};

/** Changes the subdivision in the BeatClock. */
class SubdivisionSlider : public RotarySliderWithLabel {
public:
  SubdivisionSlider(WellsAudioProcessor& p) : RotarySliderWithLabel(p, "Subdivision")
  {
    slider.setTooltip("number of subdivisions per beat");
    slider.setRange(1, 256, 1);
    slider.onValueChange = [this]() {
      processor.midiGenerator->setBeatClockSubdivision(slider.getValue());
    };
  }
  void updateComponent() { slider.setValue(processor.midiGenerator->getBeatClockSubdivision()); };
};

/** Changes the input weight in the BeatClock. */
class BeatClockInputWeightSlider : public RotarySliderWithLabel {
public:
  BeatClockInputWeightSlider(WellsAudioProcessor& p) : RotarySliderWithLabel(p, "Input Weight")
  {
    slider.setTooltip("the weight of the input vector created by the beat clock");
    slider.setRange(1, 10, 1);
    slider.onValueChange = [this]() {
      processor.midiGenerator->setBeatClockInputWeight(slider.getValue());
    };
  }
  void updateComponent() { slider.setValue(processor.midiGenerator->getBeatClockInputWeight()); };
};

/**Used to control the parameters of the BeatClock. */
class BeatClockBar : public Component {
public:
  BeatClockBar(WellsAudioProcessor& p);

  void paint(Graphics&) override;
  void resized() override;

  void updateComponents();

private:
  WellsAudioProcessor& processor;

  BeatClockOnOffButton onOffButton;
  BeatClockInputWeightSlider inputWeightSlider;
  SubdivisionSlider subdivisionSlider;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BeatClockBar)
};
