#include "PluginProcessor.h"
#include "PluginEditor.h"

ContinuumFingerboardPluginAudioProcessorEditor::ContinuumFingerboardPluginAudioProcessorEditor (ContinuumFingerboardPluginAudioProcessor& p)
: AudioProcessorEditor(&p), processor(p), surface(std::make_unique<Fingerboard>(p))
{
  surface->setCentrePosition(32  , 32 );
  surface->setSize          (1600, 300);
  addAndMakeVisible(surface.get());

  centreWithSize(1664, 364);
}
ContinuumFingerboardPluginAudioProcessorEditor::~ContinuumFingerboardPluginAudioProcessorEditor()
{

}

void ContinuumFingerboardPluginAudioProcessorEditor::paint  (Graphics& graphics)
{
  graphics.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}
void ContinuumFingerboardPluginAudioProcessorEditor::resized()
{
  
}

