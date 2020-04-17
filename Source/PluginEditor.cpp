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
    : AudioProcessorEditor(&p), processor(p), mainComponent(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setResizable(true, true);
  setSize(800, 600);

  addAndMakeVisible(&mainComponent);
}

WellsAudioProcessorEditor::~WellsAudioProcessorEditor() {}

//==============================================================================
void WellsAudioProcessorEditor::paint(Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(Colours::white);
}

void WellsAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  /* mainComponent.setSize(this->getWidth(), this->getHeight()); */
  mainComponent.setBounds(getLocalBounds());
}
