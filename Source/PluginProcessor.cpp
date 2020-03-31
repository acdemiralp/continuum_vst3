#include "PluginProcessor.h"
#include "PluginEditor.h"

ContinuumFingerboardPluginAudioProcessor::ContinuumFingerboardPluginAudioProcessor ()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
  .withInput ("Input" , AudioChannelSet::stereo(), true)
#endif
  .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
  )
#endif
{
  MPEZoneLayout layout;
  layout.setLowerZone(15);
  channel_assigner_ = MPEChannelAssigner(layout.getLowerZone());
}
ContinuumFingerboardPluginAudioProcessor::~ContinuumFingerboardPluginAudioProcessor()
{

}

const String          ContinuumFingerboardPluginAudioProcessor::getName               () const
{
  return JucePlugin_Name;
}
bool                  ContinuumFingerboardPluginAudioProcessor::acceptsMidi           () const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}
bool                  ContinuumFingerboardPluginAudioProcessor::producesMidi          () const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}
bool                  ContinuumFingerboardPluginAudioProcessor::isMidiEffect          () const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}
double                ContinuumFingerboardPluginAudioProcessor::getTailLengthSeconds  () const
{
  return 0.0;
}

bool                  ContinuumFingerboardPluginAudioProcessor::hasEditor             () const
{
  return true;
}
AudioProcessorEditor* ContinuumFingerboardPluginAudioProcessor::createEditor          ()
{
  return new ContinuumFingerboardPluginAudioProcessorEditor(*this);
}

int                   ContinuumFingerboardPluginAudioProcessor::getNumPrograms        ()
{
  return 1;
}
int                   ContinuumFingerboardPluginAudioProcessor::getCurrentProgram     ()
{
  return 0;
}
void                  ContinuumFingerboardPluginAudioProcessor::setCurrentProgram     (int index)
{
}
const String          ContinuumFingerboardPluginAudioProcessor::getProgramName        (int index)
{
  return {};
}
void                  ContinuumFingerboardPluginAudioProcessor::changeProgramName     (int index, const String& newName)
{

}

void                  ContinuumFingerboardPluginAudioProcessor::getStateInformation   (MemoryBlock& destData)
{

}
void                  ContinuumFingerboardPluginAudioProcessor::setStateInformation   (const void* data, int sizeInBytes)
{

}

void                  ContinuumFingerboardPluginAudioProcessor::prepareToPlay         (double sampleRate, int samplesPerBlock)
{

}
void                  ContinuumFingerboardPluginAudioProcessor::releaseResources      ()
{

}
void                  ContinuumFingerboardPluginAudioProcessor::processBlock          (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
  MidiBuffer midi_buffer;

  auto sample_number = 0;
  for (auto& command : note_queue_)
  {
    const auto note = std::get<0>(command);
    const auto down = std::get<1>(command);

    if (down)
    {
      note_to_channel_map_[note] = channel_assigner_.findMidiChannelForNewNote(note);
      midi_buffer.addEvent(MidiMessage::noteOn(note_to_channel_map_.at(note), note, float(1.0)), sample_number++); // TODO: Enable velocity.
    }
    else if (note_to_channel_map_.count(note))
    {
      midi_buffer.addEvent(MidiMessage::noteOff(note_to_channel_map_.at(note), note), sample_number++); // TODO: Enable velocity.
      channel_assigner_   .noteOff(note, note_to_channel_map_.at(note));
      note_to_channel_map_.erase  (note);
    }
  }
  for (auto& command : pitch_queue_)
    if (note_to_channel_map_.count(std::get<0>(command)))
      midi_buffer.addEvent(MidiMessage::pitchWheel(note_to_channel_map_.at(std::get<0>(command)), std::get<1>(command)), sample_number++);
  midiMessages.swapWith(midi_buffer);
  note_queue_ .clear();
  pitch_queue_.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool                  ContinuumFingerboardPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;
#if ! JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif
  return true;
#endif
}
#endif

void                  ContinuumFingerboardPluginAudioProcessor::addNoteCommand        (std::uint8_t note, bool is_down)
{
  note_queue_.push_back({note, is_down});
}
void                  ContinuumFingerboardPluginAudioProcessor::addPitchWheelCommand  (std::uint8_t note, std::int16_t pitch)
{
  pitch_queue_.push_back({note, pitch});
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new ContinuumFingerboardPluginAudioProcessor();
}
