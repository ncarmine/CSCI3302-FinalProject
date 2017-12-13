#include <Sparki.h> // include the sparki library
#include "pitches.h" // include a list of pitches

// Notes in the melody, (mostly) separated by rests
int melody[] = {
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, 0,
  NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_F3, 0,
  NOTE_AS3, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_C4, 0,
  NOTE_AS3, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_AS3, 0,
  NOTE_AS3, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_D4, 0,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_F3, 0,
  NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_C4, 0,
  NOTE_AS3, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_AS3, 0,
  NOTE_AS3, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_D4, 0,
  NOTE_D3, NOTE_F3, NOTE_G3, NOTE_D3, NOTE_D3, NOTE_F3, NOTE_G3, NOTE_F3, NOTE_G3, NOTE_F3, NOTE_D3, NOTE_C3, NOTE_D3, NOTE_C3, NOTE_AS2, 0,
  NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_A3, NOTE_D3, NOTE_F3, NOTE_G3,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_D4, 0,
  NOTE_D3, NOTE_F3, NOTE_G3, NOTE_D3, NOTE_D3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_D3, NOTE_C3, NOTE_D3, NOTE_C3, NOTE_AS2, 0,
  NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_A3, 0,
  NOTE_DS4, NOTE_AS4, NOTE_AS4, NOTE_DS4, NOTE_F4, NOTE_AS4, NOTE_AS4, 0,
  NOTE_AS3, NOTE_AS3, NOTE_A3,
  NOTE_D3, NOTE_D3, NOTE_F3, NOTE_G3, NOTE_AS3, NOTE_G3, 0,
  NOTE_D3, NOTE_D3, NOTE_F3, NOTE_G3, NOTE_AS3, NOTE_G3, 0,
  NOTE_D3, NOTE_F3, NOTE_G3
};

// Note durations: 2 = half note, 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  4, 4, 4, 2, 4,
  4, 2, 4, 2, 2,
  4, 2, 2, 4, 4, 1.5, 1,
  4, 4, 4, 2, 4, 2, 2, 2, 2,
  4, 2, 4, 2, 4, 1, 1,
  4, 4, 4, 2, 4, 2, 4, 1.5, 2,
  4, 1.5, 2, 4, 1, 1,
  4, 4, 4, 2, 4, 2, 2, 2, 2,
  4, 4, 2, 2, 4, 1, 2,
  4, 4, 1.5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1.5, 1.5,
  4, 4, 4, 4, 2, 2, 2, 4, 4, 1.5,
  1, 4, 4, 2, 1.5, 4, 4, 2, 4, 2, 4, 1, 2,
  4, 4, 1.5, 4, 4, 4, 2, 4, 4, 4, 4, 8, 8, 1, 1.5,
  4, 4, 4, 4, 2, 2, 1, 4,
  4, 2, 2, 2, 2, 2, 1, 4,
  1.5, 1.5, 1,
  4, 4, 4, 2, 4, 3, 8,
  4, 4, 4, 2, 4, 3, 8,
  4, 4, 1
};

void setup() {
  sparki.clearLCD();
  sparki.println("SING");
  sparki.updateLCD();
  // Play each note in the arrays
  for (int thisNote = 0; thisNote < 154; thisNote++) {

    // Calculate the note duration as 1 second divided by note type
    // Half note = 1000/2, quarter note = 1000/4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    sparki.beep(melody[thisNote],noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // The note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // Stop the tone playing:
    sparki.noBeep();
  }
}

void loop() {
  // Maybe repeat the melody in the final project implementation
}
