#include "Sequencer.h"

// function to go from BPM to the wait time between every note
// TODO: error handling would be nice
void Calculate_BPM_To_Wait_Period(int BPM, int *Usec_To_Wait) {
  float BPM_In_Sec;
  if (BPM < 60)
    return 1;
  else
    BPM_In_Sec = 60.0 / BPM;
  float BPM_In_USec = BPM_In_Sec * 10000;
  *Usec_To_Wait = (int)BPM_In_USec;
}

// for the last note there is an error because it will play it quicker then the
// other ones
void Play_Loop_Once(int wait_Time, uint8_t *notes_On_Or_Off,
                    uint8_t *notes_Values, size_t size_Of_Array) {
  int timer_Val = __HAL_TIM_GET_COUNTER(&htim3);
  int i = 0;
  while (1) {
    if (__HAL_TIM_GET_COUNTER(&htim3) - timer_Val >= wait_Time) {
      Set_LEDs_Note_List(notes_On_Or_Off);
      if (notes_On_Or_Off[i]) {
        Send_Note_Off(notes_Values[i], 60, 1);
        Set_LED(i, 0, 255, 100);
      } else {
        Set_LED(i, 0, 0, 255);
      }
      WS2812_Send();
      timer_Val = __HAL_TIM_GET_COUNTER(&htim3);
      i++;
      if (i == size_Of_Array)
        break;
    }
    HAL_UART_Receive_IT(&huart2, recieved_Data, 16);
  }
}
