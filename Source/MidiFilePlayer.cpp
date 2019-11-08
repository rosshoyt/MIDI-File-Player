/*
  ==============================================================================

    MidiFilePlayer.cpp
    Created: 7 Nov 2019 9:10:38pm
    Author:  Ross Hoyt

  ==============================================================================
*/

#include "MidiFilePlayer.h"

String filePath = "/Users/RossHoyt/Documents/MidiFiles/WebsiteSource-mfiles-co-uk/beethoven/fur-elise.mid";

MidiFilePlayer::MidiFilePlayer() : theMidiFile()
{
    loadMidiFile(filePath);
}
bool MidiFilePlayer::loadMidiFile(const String& fullPath)
{
    File file(filePath);
    FileInputStream inputStream(file);
    theMidiFile.readFrom(inputStream);
    DBG(theMidiFile.getNumTracks());
    return true; // TODO should only return true if successfull
}


