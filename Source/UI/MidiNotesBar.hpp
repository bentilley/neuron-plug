/*
 * MidiNotesBar.hpp
 * Copyright (C) 2020 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include <memory>

/*
 * MIDI Note Combo Boxes - select the MIDI notes for each neuron.
 */

class MidiNoteComboBox : public ComboBox {
public:
  MidiNoteComboBox(WellsAudioProcessor &p, int index);
  ~MidiNoteComboBox();

  void updateComponent();
  int get_midi_note_id(int note_num);

private:
  WellsAudioProcessor &processor;
  int neuron_index;
};

/*
 * Midi Note UI Bar
 *
 * Used to contorl the MIDI notes assiciated with each neuron. This is
 * controlled through the MIDI Note Combo Boxes.
 */

class MidiNotesBar : public Component {
public:
  MidiNotesBar(WellsAudioProcessor &p);
  ~MidiNotesBar();

  void paint(Graphics &) override;
  void resized() override;

  void updateComponents();
  void add_neuron_ui_update();
  void remove_neuron_ui_update();

private:
  WellsAudioProcessor &processor;

  std::vector<std::unique_ptr<MidiNoteComboBox>> midiNoteSelectors;

  void add_midi_note_selector(int neuron_index);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiNotesBar)
};
