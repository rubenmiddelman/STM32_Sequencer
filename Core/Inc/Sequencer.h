#include "LEDs.h"
#include "MIDI.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

extern uint8_t recieved_Data[16];

void Calculate_BPM_To_Wait_Period(int BPM, int *Usec_To_Wait);
void Play_Loop_Once(int wait_Time, uint8_t *notes_On_Or_Off,
                    uint8_t *notes_Values, size_t size_Of_Array);