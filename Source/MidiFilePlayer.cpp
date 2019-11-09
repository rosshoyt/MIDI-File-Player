/*
  ==============================================================================

    MidiFilePlayer.cpp
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#include "MidiFilePlayer.h"

String filePath = "/Users/RossHoyt/Documents/MidiFiles/WebsiteSource-mfiles-co-uk/beethoven/fur-elise.mid";

MidiFilePlayer::MidiFilePlayer(MidiKeyboardState& keyState) :  keyboardState(keyState), theMidiFile()
{
    loadMidiFile(filePath);
}
MidiFilePlayer::~MidiFilePlayer()
{
    delete [] tracks;
    
}
void MidiFilePlayer::play()
{
    DBG("Playing Midi File");
    int numTrackEvents =tracks[1].getNumEvents();
    
    for(int i = 0; i < numTrackEvents - 1; i++)
    {
        MidiMessage* currMsg = &tracks[1].getEventPointer(i)->message;
        
        double thisMsgTimestamp = currMsg->getTimeStamp(), nextMsgTimestamp;
        //if(i + 1 < numTrackEvents)
        nextMsgTimestamp = tracks[1].getEventPointer(i + 1)->message.getTimeStamp();
        DBG("NextMsgTimestamp " + std::to_string(nextMsgTimestamp));
        //else
        //{
        //    DBG("Stopping play");
        //    return;
        //}
        if(currMsg->isNoteOn())
            keyboardState.noteOn(currMsg->getChannel(), currMsg->getNoteNumber(), currMsg->getFloatVelocity());
        else if(currMsg->isNoteOff())
            keyboardState.noteOff(currMsg->getChannel(), currMsg->getNoteNumber(), currMsg->getFloatVelocity());
        
        Time::waitForMillisecondCounter(nextMsgTimestamp - thisMsgTimestamp);
    }
    
    
}
bool MidiFilePlayer::loadMidiFile(const String& path)
{
    // TODO could optimize file loading slightly https://forum.juce.com/t/please-help-me-play-midi/5860
    File file(path);
    if(!file.exists())
        return false;
    FileInputStream inputStream(file);
    theMidiFile.readFrom(inputStream);
    DBG("Loaded Midi File");
    theMidiFile.convertTimestampTicksToSeconds();
    setTracks();
    return true;

}
void MidiFilePlayer::setTracks()
{
    DBG("Setting Tracks");
    numTracks = theMidiFile.getNumTracks();
    tracks = new MidiMessageSequence[numTracks];
    
    for(int i = 0; i < numTracks; i++)
        tracks[i] = *theMidiFile.getTrack(i);
    
    printAllTracks();
}

void MidiFilePlayer::printAllTracks()
{
    for(int i = 0; i < numTracks; i++)
    {
        int numTrackEvents = tracks[i].getNumEvents();
        DBG("Track #" + std::to_string(i) +" NumTrack Events #" + std::to_string(numTrackEvents));// + ": ");
        
        for(int j = 0; j < tracks[i].getNumEvents(); j++)
            DBG(String(std::to_string(j)) + " @" + std::to_string(tracks[i].getEventPointer(j)->message.getTimeStamp()) + " " + tracks[i].getEventPointer(j)->message.getDescription());
    }
}

