#include "main.h"

extern UART_HandleTypeDef huart4;

uint8_t Send_Note_On(uint8_t note_Number, uint8_t velocity, uint8_t channel);
uint8_t Send_Note_Off(uint8_t note_Number, uint8_t velocity, uint8_t channel);