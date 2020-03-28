/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
WellsAudioProcessorEditor::WellsAudioProcessorEditor(WellsAudioProcessor &p)
    : AudioProcessorEditor(&p), processor(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(200, 200);

  midiVolume.setSliderStyle(Slider::LinearBarVertical);
  midiVolume.setRange(0.0, 127.0, 1.0);
  midiVolume.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
  midiVolume.setPopupDisplayEnabled(true, false, this);
  midiVolume.setTextValueSuffix(" Volume");
  midiVolume.setValue(1.0);
  midiVolume.addListener(this);

  // adds the slider to the editor
  addAndMakeVisible(&midiVolume);
}

WellsAudioProcessorEditor::~WellsAudioProcessorEditor() {}

//==============================================================================
void WellsAudioProcessorEditor::paint(Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(Colours::white);
  /* g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
   */

  g.setColour(Colours::black);
  g.setFont(15.0f);
  g.drawFittedText("MIDI Chill", 0, 0, getWidth(), 30, Justification::centred,
                   1);
}

void WellsAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  // sets the position and size of the slider with arguments (x, y, width,
  // height)
  midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

// This is the call back for when the slider value is changed
void WellsAudioProcessorEditor::sliderValueChanged(Slider *slider) {
  processor.noteOnVel = midiVolume.getValue();
}
