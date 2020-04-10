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
  setResizable(true, true);
  setSize(400, 300);

  addAndMakeVisible(&mainComponent);
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
}
