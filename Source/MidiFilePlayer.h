/*
  ==============================================================================

    MidiFilePlayer.h
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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
    //void processMidiMessage(const MidiMessage*, bool*, HashMap<int,int>*);
    
    MidiKeyboardState& keyboardState;
    MidiFile theMidiFile;
    
    
    //Array<MidiMessage> noteOnOffList;
    int numTracks;
    bool canPlay = false;
    
    
    // MIDI Containers
    MidiMessageSequence * tracks;
    typedef std::map<int, MidiMessage*> ChannelNoteMap;
    typedef std::pair<int, MidiMessage*> ChannelNotePair;
};
