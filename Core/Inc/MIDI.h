/**
 * @ Author: Ruben Middelman
 * @ Create Time: 2024-03-21 17:38:50
 * @ Modified by: Your name
 * @ Modified time: 2024-04-20 02:45:28
 * @ Description:
 * STM32 MIDI library using HAL_UART
 */

#include "main.h"

extern UART_HandleTypeDef huart4;

void Init_Midi();
uint8_t Send_Note_On(uint8_t note_Number, uint8_t velocity, uint8_t channel);
uint8_t Send_Note_Off(uint8_t note_Number, uint8_t velocity, uint8_t channel);
uint8_t Send_Pitchbend();