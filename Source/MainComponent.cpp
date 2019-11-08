/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//f==============================================================================
MainComponent::MainComponent() :   keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard), synthAudioSource (keyboardState), midiFilePlayer()
{
    
    addAndMakeVisible (keyboardComponent);
    setAudioChannels (0, 2);
    
    setSize (MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT + MAX_KEYB_HEIGHT);
    
    updateMidiDevices();
    startTimer(400);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::updateMidiDevices()
{
    DBG("-Available Midi Devices-");
    for(MidiDeviceInfo m : MidiOutput::getAvailableDevices()){
        DBG(m.name);
    }
    DBG("-End Midi Device List-");
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    int keybCompWidth = getWidth() - KEYB_MARGIN * 2, keybCompHeight = getHeight() - 5;
    keyboardComponent.setBounds (KEYB_MARGIN, KEYB_MARGIN, keybCompWidth > MAX_KEYB_WIDTH ? MAX_KEYB_WIDTH : keybCompWidth, keybCompHeight > MAX_KEYB_HEIGHT ? MAX_KEYB_HEIGHT : keybCompHeight);
    
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

