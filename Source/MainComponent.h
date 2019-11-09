/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiFilePlayer.h"
#include "SynthAudioSource.h"
//#include "StartButton.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent   :  public AudioAppComponent, public Button::Listener, private Timer
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
    
    // input handling
    void buttonClicked(Button* button) override;
    

private:
    //==============================================================================
    // Methods
    void timerCallback() override;
    void updateMidiDevices();
    
    // Member Vars
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    
    //UI Components
    TextButton playButton;
    //StartButton startButton;
    
    // Custom Member var classes
    SynthAudioSource synthAudioSource;
    MidiFilePlayer midiFilePlayer;
    
    
    // Constants
    static const int MARGIN = 4, MAX_WINDOW_HEIGHT = 800, MAX_WINDOW_WIDTH = 1200 + 2 * MARGIN,
            MAX_KEYB_WIDTH = 1200, MAX_KEYB_HEIGHT = 82, PLAY_BUTTON_WIDTH = 50, PLAY_BUTTON_HEIGHT = 30;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
