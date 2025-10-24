#include "fsm.h"
#include "stm32f4xx_hal.h"


uint8_t nbutt=0;
/*
 * Associate all state codes with subroutines
 */
	enum RET_CODES (*state_executer[])(void) = {
		[STARTUP] = do_startup,
		[BUTTON_POLL] = do_button_poll,
		[LED_TURN_ON_1] = do_led_turn_1,
		[LED_TURN_ON_2] = do_led_turn_2,
		[LED_TURN_ON_3] = do_led_turn_3,
		[LED_TURN_ON_4] = do_led_turn_4
	};

	/*
	 * Transition table
	 * src_state | return code | dst_state
	 * WARNING: avoid dublicatins
	 */
	fsm_cell_t fsm_table[] = {
	    {STARTUP, OK_FSM_1, BUTTON_POLL},
	    {BUTTON_POLL, OK_FSM_1, LED_TURN_ON_1},
	    {BUTTON_POLL, OK_FSM_2, LED_TURN_ON_2},
	    {BUTTON_POLL, OK_FSM_3, LED_TURN_ON_3},
		{BUTTON_POLL, OK_FSM_4, LED_TURN_ON_4},
	    {BUTTON_POLL, REPEAT_FSM, BUTTON_POLL},
	    {LED_TURN_ON_1, OK_FSM_1, BUTTON_POLL},
	    {LED_TURN_ON_2, OK_FSM_2, BUTTON_POLL},
	    {LED_TURN_ON_3, OK_FSM_3, BUTTON_POLL},
		{LED_TURN_ON_4, OK_FSM_4, BUTTON_POLL}
	};

	void fsm_loop(enum FSM_STATES init_state)
	{
		int state = init_state;
		enum RET_CODES ret_code;
		int iter;
		while (1) {

			ret_code = state_executer[state]();

			for (iter = 0; iter < sizeof(fsm_table); iter++) {
				if (fsm_table[iter].src_state == state &&
					fsm_table[iter].ret_code == ret_code) {
						state = fsm_table[iter].dst_state;
						break;
					}
			}
		}
	}
	/*
	 * ========================================================
	 * ============ Implement state executers =================
	 * ========================================================
	 */

	enum RET_CODES do_startup(void)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		return OK_FSM_1;
	}

	enum RET_CODES do_button_poll(void)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET) {
			nbutt++;
		}
		else {
			return REPEAT_FSM;
		}

		switch (nbutt)
		{
			case 1:
				return OK_FSM_1;
				break;
			case 2:
				return OK_FSM_2;
				break;
			case 3:
				return OK_FSM_3;
				break;
			case 4:
				return OK_FSM_4;
				nbutt=0;
				break;
		}
	}
		/*
		 * poll button, if it is pressed then return OK,
		 * otherwise REPEAT
		 */


	enum RET_CODES do_led_turn_1(void)
	{
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		 HAL_Delay(500);
		/*
		 * turn on the LED, then come back to polling
		 */
		return OK_FSM_1;
	}

	enum RET_CODES do_led_turn_2(void)
	{
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		 HAL_Delay(500);
		/*
		 * turn on the LED, then come back to polling
		 */
		return OK_FSM_2;
	}

	enum RET_CODES do_led_turn_3(void)
	{
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		 HAL_Delay(500);
		/*
		 * turn on the LED, then come back to polling
		 */
		return OK_FSM_3;
	}

	enum RET_CODES do_led_turn_4(void)
	{
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		 HAL_Delay(500);
		/*
		 * turn on the LED, then come back to polling
		 */
		return OK_FSM_4;
	}

/*
 * fsm.c
 *
 *  Created on: Mar 22, 2024
 *      Author: n.cheshkov
 */


