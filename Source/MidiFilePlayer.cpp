/*
  ==============================================================================

    MidiFilePlayer.cpp
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#include "MidiFilePlayer.h"
#include <unordered_map>

String filePath = "/Users/RossHoyt/Documents/MidiFiles/WebsiteSource-mfiles-co-uk/beethoven/fur-elise.mid";

MidiFilePlayer::MidiFilePlayer(MidiKeyboardState& keyState) : Thread("MIDI Player Thread"), keyboardState(keyState),  theMidiFile()
{
    loadMidiFile(filePath);
}
MidiFilePlayer::~MidiFilePlayer()
{
    stopThread (1000);
    delete [] tracks;
}

bool MidiFilePlayer::loadMidiFile(const String& path)
{
    // TODO could optimize file loading slightly https://forum.juce.com/t/please-help-me-play-midi/5860
    File file(path);
    if(file.exists()){
        FileInputStream inputStream(file);
        theMidiFile.readFrom(inputStream);
        DBG("Loaded Midi File, ready to play");
        theMidiFile.convertTimestampTicksToSeconds();
        setTracks();
        return (canPlay = true);
    }
    DBG("Cannot Play File or Find Path");
    return (canPlay = false);
}

void MidiFilePlayer::play()
{
    if(canPlay) startThread(10);
}

// https://github.com/WeAreROLI/JUCE/blob/master/examples/Utilities/MultithreadingDemo.h
void MidiFilePlayer::run()
{
    DBG("Playing Midi File");
    if(ignoreSustainPedalMessages) playFile_IgnoreSustainPedal();
    else playFile();
}
void MidiFilePlayer::playFile()
{
    int numTrackEvents = tracks[1].getNumEvents();
    int i = 0;
    SortedSet<int> channelNoteMap;
    //std::vector<MidiMessage*> channelNoteMap;
    //std::unordered_multimap<int, int> channelMap;
    //ChannelNoteMap channelNoteMap;
    bool sustainOn = false;
    while(i < numTrackEvents && !threadShouldExit())
    {
        MidiMessage* currMsg = &tracks[1].getEventPointer(i)->message;
        if(!currMsg->isEndOfTrackMetaEvent()) // TODO ERROR CORRECTION - MIGHT NOT BE END OF TRACK MSG
        {
            MidiMessage* nextMsg = &tracks[1].getEventPointer(i + 1)->message;
            double thisMsgTimestamp = currMsg->getTimeStamp(), nextMsgTimestamp = nextMsg->getTimeStamp();
            
            //if(thisMsgTimestamp != nextMsgTimestamp)
            double waitTimeSecs = 1000 * (nextMsgTimestamp - thisMsgTimestamp);
            
            DBG("Waittime(ms): " + std::to_string(waitTimeSecs));
            
            if(currMsg->isNoteOn())
            {
                DBG("NoteOn" + std::to_string(currMsg->getNoteNumber()));
                keyboardState.noteOn(1, currMsg->getNoteNumber(), currMsg->getFloatVelocity());
                if(sustainOn) channelNoteMap.add(currMsg->getNoteNumber());
            }
            else if(currMsg->isNoteOff())
            {
                DBG("NoteOff" + std::to_string(currMsg->getNoteNumber()));
                if(!sustainOn) keyboardState.noteOff(1/*currMsg->getChannel()*/, currMsg->getNoteNumber(), currMsg->getFloatVelocity());
                else DBG("Note " + std::to_string(currMsg->getNoteNumber()) + " RELEASED BUT SUSTAINED");
            }
            else if(currMsg->isSustainPedalOn())
            {
                DBG("---SUSTAIN PEDAL ON");
                sustainOn = true;
            }
            else if(currMsg->isSustainPedalOff())
            {
                sustainOn = false;
                DBG("---SUS PED OFF--- #Notes to release:" + std::to_string(channelNoteMap.size()));
                //const int i = *channelNoteMap.begin();
                for(int i = 0; i < channelNoteMap.size(); ++i)
                {
                    int relNote = channelNoteMap.getUnchecked(i);
                    DBG("RELEASING NOTE" + std::to_string(relNote));
                    keyboardState.noteOff(1, relNote, 96);
                }
                
            }
            
            if(waitTimeSecs != 0) wait((int)waitTimeSecs);
        } else{
            threadShouldExit();
        }
        i++;
    }
    DBG("Finished Playing!");
}
void MidiFilePlayer::playFile_IgnoreSustainPedal()
{
    int numTrackEvents = tracks[1].getNumEvents();
    int i = 0;
    
    while(i < numTrackEvents && !threadShouldExit())
    {
        MidiMessage* currMsg = &tracks[1].getEventPointer(i)->message;
        if(!currMsg->isEndOfTrackMetaEvent()) // TODO ERROR CORRECTION - MIGHT NOT BE END OF TRACK MSG
        {
            MidiMessage* nextMsg = &tracks[1].getEventPointer(i + 1)->message;
            double thisMsgTimestamp = currMsg->getTimeStamp(), nextMsgTimestamp = nextMsg->getTimeStamp();
            
            //if(thisMsgTimestamp != nextMsgTimestamp)
            double waitTimeSecs = 1000 * (nextMsgTimestamp - thisMsgTimestamp);
            
            DBG("Waittime(ms): " + std::to_string(waitTimeSecs));
            
            if(currMsg->isNoteOn())
            {
                DBG("NoteOn" + std::to_string(currMsg->getNoteNumber()));
                keyboardState.noteOn(1, currMsg->getNoteNumber(), currMsg->getFloatVelocity());
            }
            else if(currMsg->isNoteOff())
            {
                DBG("NoteOff" + std::to_string(currMsg->getNoteNumber()));
                keyboardState.noteOff(1/*currMsg->getChannel()*/, currMsg->getNoteNumber(), currMsg->getFloatVelocity());
               
            }
            if(waitTimeSecs != 0) wait((int)waitTimeSecs);
        } else{
            threadShouldExit();
        }
        i++;
    }
    DBG("Finished Playing!");
}
//void MidiFilePlayer::processMidiMessage(const MidiMessage* currMsg, bool* sustainOn, HashMap<int,int>* channelNoteMap){
//
//
//}
void MidiFilePlayer::stop()
{
    DBG("Stopping MidiFilePlayer");
    threadShouldExit();
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


