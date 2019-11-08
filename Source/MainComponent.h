/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiFilePlayer.h"
#include "SynthAudioSource.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   :  public AudioAppComponent, private Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void prepareToPlay(int, double) override;
    void getNextAudioBlock(const AudioSourceChannelInfo&) override;
    void releaseResources() override;
    

private:
    //==============================================================================
    // Methods
    void timerCallback() override;
    void updateMidiDevices();
    
    // Member Vars
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    //AudioDeviceManager audioDeviceManager;
    
    // Custom member var classes
    SynthAudioSource synthAudioSource;
    MidiFilePlayer midiFilePlayer;
    
    
    // Constants
    static const int MAX_WINDOW_HEIGHT = 800, MAX_WINDOW_WIDTH = 1208, KEYB_MARGIN = 4, MAX_KEYB_WIDTH = 1200, MAX_KEYB_HEIGHT = 82;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
