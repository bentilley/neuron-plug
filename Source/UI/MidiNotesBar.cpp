/*
 * MidiNotesBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiNotesBar.hpp"

MidiNotesBar::MidiNotesBar() {

  StringArray midiNoteNums;
  for (int i = 1; i < 128; ++i) {
    midiNoteNums.add(String(i));
  }

  for (int i = 0; i < numNeurons; ++i) {
    std::unique_ptr<ComboBox> combo =
        std::make_unique<ComboBox>("midiCombo" + String(i));
    combo->addItemList(midiNoteNums, 1);
    combo->setEditableText(false);
    combo->setSelectedId(60);
    addAndMakeVisible(*combo);
    midiNoteSelectors.push_back(std::move(combo));
  }
}
MidiNotesBar::~MidiNotesBar() {}

void MidiNotesBar::paint(Graphics &g) {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);

  g.setColour(lightGrey);
  g.fillRoundedRectangle(area.toFloat(), 10.0);
  g.setColour(darkGrey);
  g.drawRoundedRectangle(area.toFloat(), 10.0, 1.0);

  auto rowLabelArea = area.removeFromLeft(rowLabelWidth);
  rowLabelPadding.subtractFrom(rowLabelArea);
  g.setFont(16.0f);
  g.drawText("MIDI Notes", rowLabelArea, Justification::centredLeft, true);
}

void MidiNotesBar::resized() {
  auto area = getLocalBounds();
  blockPadding.subtractFrom(area);
  area.removeFromLeft(rowLabelWidth);

  for (auto combo = midiNoteSelectors.begin(); combo != midiNoteSelectors.end();
       combo++) {
    auto componentArea = area.removeFromLeft(colWidth);
    componentPadding.subtractFrom(componentArea);
    (*combo)->setBounds(componentArea);
  }
}
