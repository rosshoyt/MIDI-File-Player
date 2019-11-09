/*
  ==============================================================================

    MidiFilePlayer.h
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MidiFilePlayer
{
public:
    MidiFilePlayer(MidiKeyboardState&);
    ~MidiFilePlayer();
    bool loadMidiFile(const String&);
    void play();
private:
    void setTracks();
    void printAllTracks();
    MidiKeyboardState& keyboardState;
    MidiFile theMidiFile;
    
    MidiMessageSequence * tracks;
    //Array<MidiMessage> noteOnOffList;
    int numTracks;
    
};
