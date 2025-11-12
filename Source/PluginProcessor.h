/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Simple_EQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Simple_EQAudioProcessor();
    ~Simple_EQAudioProcessor() override;

    //==============================================================================
    // PREPARE TO PLAY
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    // DON'T INTERRUPT THIS UNLESS YOU WANT TO CAUSE DAMAGE
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;


    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // This will help establish the Parameter layout : Returns a parameter layout
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // using "*this" audio processor | no UndoManager | "Parameters" is the valueTreeType | We create a function to return the layout
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };


private:
    // Alias for dsp namespace mess
        // Each has a response of 12 DB/OCT
        // To get 48 DB/OCT, we need 4 Filters linked by a chain.
            // the chain passes a single context to each filter automatically.
        
    // Filter, CutFilter, etc are the aliases 

    using Filter = juce::dsp::IIR::Filter<float>;

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    // Goal: We need 2 monoChains for stereo Processing

    //Defining a chain for the mono path
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;

    // This declares two MonoChains which are "juce::dsp::ProcessorChain<>"under an alias.
    MonoChain leftChain, rightChain; 




    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Simple_EQAudioProcessor)
};
