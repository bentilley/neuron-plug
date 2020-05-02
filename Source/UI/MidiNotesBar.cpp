/*
 * MidiNotesBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiNotesBar.hpp"

MidiNotesBar::MidiNotesBar(WellsAudioProcessor &p) : processor(p) {

  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    std::unique_ptr<MidiNoteComboBox> combo =
        std::make_unique<MidiNoteComboBox>(p, i);
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
       ++combo) {
    auto componentArea = area.removeFromLeft(colWidth);
    componentPadding.subtractFrom(componentArea);
    (*combo)->setBounds(componentArea);
  }
}

void MidiNotesBar::updateComponents() {
  for (auto combo = midiNoteSelectors.begin(); combo != midiNoteSelectors.end();
       ++combo) {
    (*combo)->updateComponent();
  }
}

/*
 * MIDI Note Combo Box
 */

MidiNoteComboBox::MidiNoteComboBox(WellsAudioProcessor &p, int idx)
    : ComboBox("midiCombo" + String(idx)), processor(p), neuron_index(idx) {
  StringArray midiNoteNums;
  for (int i = 1; i < 128; ++i) {
    midiNoteNums.add(String(i));
  }

  addItemList(midiNoteNums, 1);
  setEditableText(false);
  onChange = [this]() {
    processor.midiGenerator->set_neuron_midi_note(neuron_index,
                                                 getText().getIntValue());
  };
}
MidiNoteComboBox::~MidiNoteComboBox() {}

void MidiNoteComboBox::updateComponent() {
  int id{get_midi_note_id(
      processor.midiGenerator->get_neuron_midi_note(neuron_index))};
  setSelectedId(id);
}
int MidiNoteComboBox::get_midi_note_id(int note_num) { return note_num; }
