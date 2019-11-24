/*
  ==============================================================================

    MidiFilePlayer.h
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AtomicWrapper.h"

class MidiFilePlayer : public Thread//, public HighResolutionTimer
{
public:
    MidiFilePlayer(MidiKeyboardState&);
    ~MidiFilePlayer();
    bool loadMidiFile(const String&);
    
    // methods controlled by buttons
    void play();
    void stop();
    bool useSustainPedalMessages = false;
    
    
private:
    void run() override;
    
    void playFile();
    void playFile_IgnoreSustainPedal();
    
    void setTracks();
    void printAllTracks();
    
    MidiKeyboardState& keyboardState;
    MidiFile theMidiFile;
    MidiMessageSequence * tracks;
    
    
    
    int numTracks;
    bool canPlay = false;
};
