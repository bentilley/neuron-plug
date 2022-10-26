/*
 * MidiInputBar.hpp
 * Copyright (C) 2022 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "Components/OnOffButton.hpp"
#include "Components/RotarySliderWithLabel.hpp"

/** Turn the MIDI input on or off. */
class MidiInputOnOffButton : public OnOffButton {
public:
  MidiInputOnOffButton(WellsAudioProcessor& p) : OnOffButton(p)
  {
    setTooltip("turn MIDI input on or off");
    onClick = [this]() { processor.midiGenerator->toggleMidiInputIsOn(); };
  }
  void updateComponent()
  {
    setColour(
      TextButton::ColourIds::buttonColourId,
      processor.midiGenerator->getMidiInputIsOn() ? AppStyle.buttonOnColour
                                                  : AppStyle.buttonOffColour
    );
  }
};

/** Changes the input weight in the MidiInput. */
class MidiInputInputWeightSlider : public RotarySliderWithLabel {
public:
  MidiInputInputWeightSlider(WellsAudioProcessor& p) : RotarySliderWithLabel(p, "Input Weight")
  {
    slider.setTooltip("the weight to scale the input vector created by the MIDI input");
    slider.setRange(1, 10, 1);
    slider.onValueChange = [this]() {
      processor.midiGenerator->setMidiInputInputWeight(slider.getValue());
    };
  }
  void updateComponent() { slider.setValue(processor.midiGenerator->getMidiInputInputWeight()); };
};
/**Used to control the parameters of the MIDI input module. */
class MidiInputBar : public Component {
public:
  MidiInputBar(WellsAudioProcessor& p);

  void paint(Graphics&) override;
  void resized() override;

  void updateComponents();

private:
  WellsAudioProcessor& processor;

  MidiInputOnOffButton onOffButton;
  MidiInputInputWeightSlider inputWeightSlider;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiInputBar)
};
