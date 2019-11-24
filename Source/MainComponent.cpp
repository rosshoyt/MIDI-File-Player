/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//f==============================================================================
MainComponent::MainComponent() :   keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard), playButton(), stopButton(), useSustainPedalButton(), openGLDisplay(), synthAudioSource (keyboardState), midiFilePlayer(keyboardState)
{
    
    addAndMakeVisible (keyboardComponent);
    
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);

    addAndMakeVisible(useSustainPedalButton);
    useSustainPedalButton.setButtonText("Use Sustain Pedal");
    useSustainPedalButton.addListener(this);

    addAndMakeVisible(openGLDisplay);
    keyboardState.addListener(&openGLDisplay);
    
    setAudioChannels (0, 2);

    setSize (MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);

    updateMidiDevices();
    
    startTimer(400);
}
void MainComponent::setSharedChordState()
{
    //midiFilePlayer.setSharedChordArray(chordStateArray);
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


}

void MainComponent::resized()
{
    // REFACTOR Keyboard dimensino calculations
    int resizedKeybWidth = getWidth() - MARGIN * 2, resizedKeybHeight = getHeight() - 5;
    int keybWidth = resizedKeybWidth > MAX_KEYB_WIDTH ? MAX_KEYB_WIDTH : resizedKeybWidth;
    int keybHeight = resizedKeybHeight > MAX_KEYB_HEIGHT ? MAX_KEYB_HEIGHT : resizedKeybHeight;
    keyboardComponent.setBounds (MARGIN, MARGIN, keybWidth, keybHeight );

    float bottomButtonY =  getHeight() - (MARGIN + TEXT_BUTTON_HEIGHT);
    playButton.setBounds(MARGIN, bottomButtonY, TEXT_BUTTON_WIDTH, TEXT_BUTTON_HEIGHT);
    stopButton.setBounds(2 * MARGIN + TEXT_BUTTON_WIDTH, bottomButtonY, TEXT_BUTTON_WIDTH, TEXT_BUTTON_HEIGHT);
    useSustainPedalButton.setBounds(3 * MARGIN + 2 * TEXT_BUTTON_WIDTH, bottomButtonY, TEXT_BUTTON_WIDTH * 3, TEXT_BUTTON_HEIGHT);

    int openGLDisplayYCoord =  MARGIN * 2 + keybHeight;
    openGLDisplay.setBounds(MARGIN, openGLDisplayYCoord, getWidth() - 2 * MARGIN, getHeight() - openGLDisplayYCoord - TEXT_BUTTON_HEIGHT - MARGIN);
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

void MainComponent::buttonClicked(Button *button)
{
    DBG("BUTTON CLICKED");
    if(button == &playButton)
    {
        DBG("PLAYING");
        midiFilePlayer.play();

    }
    else if(button == &stopButton)
    {
        DBG("STOPPING");
        midiFilePlayer.stop();
    }
    else if(button == &useSustainPedalButton)
    {
        DBG("USE-SUSTAIN-PEDAL BUTTON PRESSED");
        midiFilePlayer.useSustainPedalMessages = !midiFilePlayer.useSustainPedalMessages;
    }

}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}
