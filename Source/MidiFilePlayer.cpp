/*
  ==============================================================================

    MidiFilePlayer.cpp
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#include "MidiFilePlayer.h"

String filePath = "/Users/RossHoyt/Documents/MidiFiles/WebsiteSource-mfiles-co-uk/beethoven/fur-elise.mid";

MidiFilePlayer::MidiFilePlayer(MidiKeyboardState& keyState) : Thread("Player Thread"), keyboardState(keyState),  theMidiFile()
{
    loadMidiFile(filePath);
}
MidiFilePlayer::~MidiFilePlayer()
{
    stopThread (2000);
    delete [] tracks;
    
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
    
    canPlay = true;
    return canPlay;

}

void MidiFilePlayer::play()
{
    if(canPlay) startThread(10);
}
// https://github.com/WeAreROLI/JUCE/blob/master/examples/Utilities/MultithreadingDemo.h
void MidiFilePlayer::run()
{
    DBG("Playing Midi File in Thread run()");
    int numTrackEvents = tracks[1].getNumEvents();
    int i = 0;
    while(i < numTrackEvents && !threadShouldExit())
    //for(int i = 0; i < numTrackEvents; i++)
    {
        MidiMessage* currMsg = &tracks[1].getEventPointer(i)->message;
        if(!currMsg->isEndOfTrackMetaEvent()) // TODO ERROR CORRECTION - MIGHT NOT BE END OF TRACK MSG
        {
            MidiMessage* nextMsg = &tracks[1].getEventPointer(i + 1)->message;
            double thisMsgTimestamp = currMsg->getTimeStamp(), nextMsgTimestamp = nextMsg->getTimeStamp();
            double waitTimeSecs = 0;
            if(thisMsgTimestamp != nextMsgTimestamp)
                waitTimeSecs = 1000 * (nextMsgTimestamp - thisMsgTimestamp);
           
            DBG("Waittime(ms): " + std::to_string(waitTimeSecs));
            
            if(currMsg->isNoteOn())
                keyboardState.noteOn(currMsg->getChannel(), currMsg->getNoteNumber(), currMsg->getFloatVelocity());
            else if(currMsg->isNoteOff())
                keyboardState.noteOff(currMsg->getChannel(), currMsg->getNoteNumber(), currMsg->getFloatVelocity());
            
            if(waitTimeSecs != 0) wait((int)waitTimeSecs);
        } else{
            threadShouldExit();
        }
        i++;
    }
    DBG("Finished Playing!");
    // TODO thread must regularly check the threadShouldExit() method
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


