#include "Fingerboard.h"

Fingerboard::Fingerboard(ContinuumFingerboardPluginAudioProcessor& p) : Label("Fingerboard"), processor_(p)
{
  setInterceptsMouseClicks(true, true);
  setColour               (backgroundColourId, Colours::darkred);
  startTimer              (16);
}
Fingerboard::~Fingerboard()
{

}

void Fingerboard::paint        (Graphics& graphics)
{
  Label::paint(graphics);

  graphics.setColour  (Colours::black);
  for (auto i = 0; i < 128; ++i)
    if (MidiMessage::getMidiNoteName(i, true, true, true).contains("#"))
      graphics.fillRect((float(i) - 0.5f) * float(getWidth() / 128.0f), float(0), float(getWidth() / 128.0f), float(getWidth()));
  
  graphics.setColour  (Colours::white);
  graphics.drawText   (position_.toString(), 4 , 4, 64, 32, Justification::topLeft);
}
void Fingerboard::resized      ()
{
  Label::resized();
}
void Fingerboard::timerCallback()
{
  repaint();
}

void Fingerboard::mouseDrag(const MouseEvent& event)
{
  Label::mouseDrag(event);
  
  const auto note  = 127 * float(event.getPosition().getX()) / getWidth();
  const auto pitch = MidiMessage::pitchbendToPitchwheelPos(-event.getPosition().getY(), 2 * getHeight());

  if (std::uint8_t(note) != last_note_)
    processor_.addNoteCommand(last_note_, false);

  if (!contains(event.getPosition()))
    return;

  if (std::uint8_t(note) != last_note_)
    processor_.addNoteCommand(note      , true );
  processor_.addPitchWheelCommand(note, pitch);

  last_note_ = note;
  position_  = event.getPosition();
}
void Fingerboard::mouseDown(const MouseEvent& event)
{
  Label::mouseDown(event);

  const auto note  = 127 * float(event.getPosition().getX()) / getWidth();
  const auto pitch = MidiMessage::pitchbendToPitchwheelPos(-event.getPosition().getY(), 2 * getHeight());

  processor_.addNoteCommand      (note, true );
  processor_.addPitchWheelCommand(note, pitch);

  last_note_ = note;
  position_  = event.getPosition();
}
void Fingerboard::mouseUp  (const MouseEvent& event)
{
  Label::mouseUp(event);

  const auto note  = 127 * float(event.getPosition().getX()) / getWidth();

  processor_.addNoteCommand(note, false);

  last_note_ = note;
  position_  = event.getPosition();
}
