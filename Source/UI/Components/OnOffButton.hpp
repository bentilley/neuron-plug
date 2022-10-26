/*
 * OnOffButton.hpp
 * Copyright (C) 2022 Ben Tilley <targansaikhan@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../../PluginProcessor.h"
#include "../Styles.hpp"

// UTF-8 `⏻`
const CharPointer_UTF8 powerIcon = CharPointer_UTF8("\xe2\x8f\xbb");

/** Button for toggling things on and off. */
class OnOffButton : public TextButton {
public:
  OnOffButton(WellsAudioProcessor& p) : TextButton(powerIcon), processor(p)
  {
    setLookAndFeel(&symbolsLookAndFeel);
  };
  virtual void updateComponent() = 0;

protected:
  WellsAudioProcessor& processor;

private:
  SymbolsLookAndFeel symbolsLookAndFeel;
};
