/**
 * @ Author: Ruben Middelman
 * @ Create Time: 2024-03-21 17:38:01
 * @ Modified by: Your name
 * @ Modified time: 2024-04-20 02:57:46
 * @ Description: STM32 MIDI library using HAL_UART
 */

#include "MIDI.h"

/**
 * @brief Init function
 * should probably be uses to check if UART is initialized correctly
 */
void Init_Midi() {}

/**
 * @brief Send Note on message
 * @param note_Number
 * @param velocity
 * @param channel
 * @return uint8_t negative number for incorrect variable, 0 if all is oke
 */
uint8_t Send_Note_On(uint8_t note_Number, uint8_t velocity, uint8_t channel) {
  char Midi_Message[] = {0, 0, 0};
  if (channel > 16)
    return -1;
  else
    // for the channel we do - 1 this is because midi starts count form 1 not 0
    Midi_Message[0] = 0x90 + (channel - 1);
  if (note_Number > 127)
    return -1;
  else
    Midi_Message[1] = note_Number;
  if (velocity > 127)
    return -1;
  else
    Midi_Message[2] = velocity;
  HAL_UART_Transmit(&huart4, Midi_Message, sizeof(Midi_Message), 1);
  return 0;
}

/**
 * @brief Send Note off message
 * @param note_Number
 * @param velocity
 * @param channel
 * @return uint8_t  negative number for incorrect variable, 0 if all is oke
 */
uint8_t Send_Note_Off(uint8_t note_Number, uint8_t velocity, uint8_t channel) {
  char Midi_Message[] = {0, 0, 0};
  // for the channel we do - 1 this is because midi starts count form 1 not 0
  if (channel > 16)
    return -1;
  else
    Midi_Message[0] = 0x80 + (channel - 1);
  if (note_Number > 127)
    return -1;
  else
    Midi_Message[1] = note_Number;
  if (velocity > 127)
    return -1;
  else
    Midi_Message[2] = velocity;
  HAL_UART_Transmit(&huart4, Midi_Message, sizeof(Midi_Message), 1);
  return 0;
}
