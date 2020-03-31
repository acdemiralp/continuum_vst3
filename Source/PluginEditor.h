#pragma once

#include <JuceHeader.h>

#include "Fingerboard.h"
#include "PluginProcessor.h"

class ContinuumFingerboardPluginAudioProcessorEditor : public AudioProcessorEditor
{
public:
  ContinuumFingerboardPluginAudioProcessorEditor(ContinuumFingerboardPluginAudioProcessor&);
 ~ContinuumFingerboardPluginAudioProcessorEditor();

  void paint  (Graphics&) override;
  void resized()          override;

private:
  ContinuumFingerboardPluginAudioProcessor& processor;
  std::unique_ptr<Fingerboard>              surface;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContinuumFingerboardPluginAudioProcessorEditor)
};
