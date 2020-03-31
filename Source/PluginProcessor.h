#pragma once

#include <JuceHeader.h>

class ContinuumFingerboardPluginAudioProcessor : public AudioProcessor
{
public:
  ContinuumFingerboardPluginAudioProcessor();
 ~ContinuumFingerboardPluginAudioProcessor();

  const String          getName               () const                                 override;
  bool                  acceptsMidi           () const                                 override;
  bool                  producesMidi          () const                                 override;
  bool                  isMidiEffect          () const                                 override;
  double                getTailLengthSeconds  () const                                 override;
                                                                                       
  bool                  hasEditor             () const                                 override;
  AudioProcessorEditor* createEditor          ()                                       override;
                                                                                       
  int                   getNumPrograms        ()                                       override;
  int                   getCurrentProgram     ()                                       override;
  void                  setCurrentProgram     (int index)                              override;
  const String          getProgramName        (int index)                              override;
  void                  changeProgramName     (int index, const String& newName)       override;
                                                                                       
  void                  getStateInformation   (MemoryBlock& destData)                  override;
  void                  setStateInformation   (const void* data, int sizeInBytes)      override;

  void                  prepareToPlay         (double sampleRate, int samplesPerBlock) override;
  void                  releaseResources      ()                                       override;
  void                  processBlock          (AudioBuffer<float>&, MidiBuffer&)       override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool                  isBusesLayoutSupported(const BusesLayout& layouts) const       override;
#endif
  
  void                  addNoteCommand        (std::uint8_t note, bool is_down);
  void                  addPitchWheelCommand  (std::uint8_t note, std::int16_t pitch);

private:
  std::vector<std::tuple<std::uint8_t, bool>>         note_queue_         ;
  std::vector<std::tuple<std::uint8_t, std::int16_t>> pitch_queue_        ;
  MPEChannelAssigner                                  channel_assigner_   ;
  std::map<std::uint8_t, std::int32_t>                note_to_channel_map_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContinuumFingerboardPluginAudioProcessor)
};
