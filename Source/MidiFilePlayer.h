/*
  ==============================================================================

    MidiFilePlayer.h
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MidiFilePlayer {
public:
    MidiFilePlayer();
    bool loadMidiFile(const String&);
private:
    MidiFile theMidiFile;
};
