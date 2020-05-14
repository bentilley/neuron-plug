/*
 * MidiNotesBar.cpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "MidiNotesBar.hpp"
#include "Styles.hpp"

MidiNotesBar::MidiNotesBar(WellsAudioProcessor &p) : processor(p) {

  for (int i = 0; i < p.midiGenerator->num_neurons(); ++i) {
    add_midi_note_selector(i);
  }
}
MidiNotesBar::~MidiNotesBar() {}

void MidiNotesBar::paint(Graphics &g) {
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);

  g.setColour(AppStyle.darkGrey);
  g.fillRoundedRectangle(area.toFloat(), 5.0);
  g.setColour(AppStyle.mediumGrey);
  g.drawRoundedRectangle(area.toFloat(), 5.0, 1.0);

  auto rowLabelArea = area.removeFromLeft(AppStyle.rowLabelWidth);
  AppStyle.rowLabelPadding.subtractFrom(rowLabelArea);
  g.setFont(AppStyle.fontSizeMedium);
  g.setColour(AppStyle.lightGrey);
  g.drawText("MIDI Notes", rowLabelArea, Justification::centredLeft, true);
}

void MidiNotesBar::resized() {
  auto area = getLocalBounds();
  AppStyle.blockPadding.subtractFrom(area);
  area.removeFromLeft(AppStyle.rowLabelWidth);

  for (auto combo = midiNoteSelectors.begin(); combo != midiNoteSelectors.end();
       ++combo) {
    auto componentArea = area.removeFromLeft(AppStyle.colWidth);
    AppStyle.componentPadding.subtractFrom(componentArea);
    (*combo)->setBounds(componentArea);
  }
}

void MidiNotesBar::add_midi_note_selector(int neuron_index) {
  std::unique_ptr<MidiNoteComboBox> combo =
      std::make_unique<MidiNoteComboBox>(processor, neuron_index);
  addAndMakeVisible(*combo);
  midiNoteSelectors.push_back(std::move(combo));
}

// Public Methods

void MidiNotesBar::updateComponents() {
  for (auto combo = midiNoteSelectors.begin(); combo != midiNoteSelectors.end();
       ++combo) {
    (*combo)->updateComponent();
  }
}

void MidiNotesBar::add_neuron_ui_update() {
  add_midi_note_selector(midiNoteSelectors.size());
  resized();
}
void MidiNotesBar::remove_neuron_ui_update() {
  midiNoteSelectors.pop_back();
  resized();
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
