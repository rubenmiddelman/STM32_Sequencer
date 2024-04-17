#include "MIDI.h"

void Init_Midi() {}

// function to send note on message
uint8_t Send_Note_On(uint8_t note_Number, uint8_t velocity, uint8_t channel) {
  char Midi_Message[] = {0, 0, 0};
  if (channel > 16)
    return 1;
  else
    // for the channel we do - 1 this is because midi starts count form 1 not 0
    Midi_Message[0] = 0x90 + (channel - 1);
  if (note_Number > 127)
    return 1;
  else
    Midi_Message[1] = note_Number;
  if (velocity > 127)
    return 1;
  else
    Midi_Message[2] = velocity;
  return 0;
  HAL_UART_Transmit(&huart4, Midi_Message, sizeof(Midi_Message), 1);
}

// function to send note off message
uint8_t Send_Note_Off(uint8_t note_Number, uint8_t velocity, uint8_t channel) {
  char Midi_Message[] = {0, 0, 0};
  // for the channel we do - 1 this is because midi starts count form 1 not 0
  if (channel > 16)
    return 1;
  else
    Midi_Message[0] = 0x80 + (channel - 1);
  if (note_Number > 127)
    return 1;
  else
    Midi_Message[1] = note_Number;
  if (velocity > 127)
    return 1;
  else
    Midi_Message[2] = velocity;
  HAL_UART_Transmit(&huart4, Midi_Message, sizeof(Midi_Message), 1);
  return 0;
}
