/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
WellsAudioProcessor::WellsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      midiGenerator(std::make_unique<MidiGenerator>(5)) {

  // Run all tests when plugin loads in debug
#ifdef DEBUG
  UnitTestRunner testRunner;
  testRunner.runAllTests();
#endif
}

WellsAudioProcessor::~WellsAudioProcessor() {}

//==============================================================================
const String WellsAudioProcessor::getName() const { return JucePlugin_Name; }

bool WellsAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool WellsAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool WellsAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double WellsAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int WellsAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int WellsAudioProcessor::getCurrentProgram() { return 0; }

void WellsAudioProcessor::setCurrentProgram(int index) {}

const String WellsAudioProcessor::getProgramName(int index) { return {}; }

void WellsAudioProcessor::changeProgramName(int index, const String &newName) {}

//==============================================================================
void WellsAudioProcessor::prepareToPlay(double sampleRate,
                                        int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void WellsAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WellsAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void WellsAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                       MidiBuffer &midiMessages) {
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);

    // ..do something to the data...
  }

  // Well Neuron Processing

  MidiBuffer processedMidi;
  double sample_rate = getSampleRate();
  int num_buffer_samples = buffer.getNumSamples();
  AudioPlayHead::CurrentPositionInfo pos;
  getPlayHead()->getCurrentPosition(pos);

  if (midiGenerator->get_is_on() && pos.isPlaying) {
    midiGenerator->generate_next_midi_buffer(processedMidi, pos, sample_rate,
                                             num_buffer_samples);
  }

  midiMessages.swapWith(processedMidi);
}

void WellsAudioProcessor::add_neuron() {
  std::unique_ptr<MidiGenerator> new_generator =
      std::make_unique<MidiGenerator>(*midiGenerator);
  new_generator->add_neuron();
  midiGenerator = std::move(new_generator);
}

//==============================================================================
bool WellsAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *WellsAudioProcessor::createEditor() {
  return new WellsAudioProcessorEditor(*this);
}

//==============================================================================
void WellsAudioProcessor::getStateInformation(MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void WellsAudioProcessor::setStateInformation(const void *data,
                                              int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new WellsAudioProcessor();
}
