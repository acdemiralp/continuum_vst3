#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

class Fingerboard : public Label, private Timer
{
public:
  Fingerboard(ContinuumFingerboardPluginAudioProcessor&);
 ~Fingerboard();
 
  void paint        (Graphics&)               override;
  void resized      ()                        override;
  void timerCallback()                        override;

  void mouseDrag    (const MouseEvent& event) override;
  void mouseDown    (const MouseEvent& event) override;
  void mouseUp      (const MouseEvent& event) override;

private:
  ContinuumFingerboardPluginAudioProcessor& processor_;
  Point<int>                                position_ ;
  std::uint8_t                              last_note_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fingerboard)
};
