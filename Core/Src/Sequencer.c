/**
 * @ Author: Ruben Middelman
 * @ Create Time: 2024-03-21 21:10:08
 * @ Modified by: Ruben Middelman
 * @ Modified time: 2024-04-20 03:09:01
 * @ Description:
 */

#include "Sequencer.h"

/**
 * @brief Calculate BPM to waiting time
 * the waiting time is used in the Play_Loop_Once function, this is the time we
 * wait before a new note is played
 * right now the synth is set up to play 8 notes all at quarter notes
 *
 * TODO: We should make the amount of notes choseable (4 quarter notes, 8
 * eightnotes, 16 16notes)
 *
 * @param BPM
 * @param Usec_To_Wait needs an int that is used to set as the wait time, this
 * is also what we send into the other function
 */
void Calculate_BPM_To_Wait_Period(int BPM, int *Usec_To_Wait) {
  float BPM_In_Sec;
  if (BPM < 60)
    return 1;
  else
    BPM_In_Sec = 60.0 / BPM;
  float BPM_In_USec = BPM_In_Sec * 10000;
  *Usec_To_Wait = (int)BPM_In_USec;
}

/**
 * @brief Plays the whole sequencer loop once, Also sets the LED's
 *
 * TODO: Might be nice to let user change the LED colors
 *
 * @param wait_Time
 * @param notes_On_Or_Off
 * @param notes_Values
 * @param size_Of_Array
 */
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
