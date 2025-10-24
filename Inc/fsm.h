/*
 * fsm.h
 *
 *  Created on: Mar 22, 2024
 *      Author: n.cheshkov
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

	enum FSM_STATES {
		STARTUP = 0,
		BUTTON_POLL,
		LED_TURN_ON_1,
		LED_TURN_ON_2,
		LED_TURN_ON_3,
		LED_TURN_ON_4
	};

	/*
	 * Define all possible return codes for each state
	 */
	enum RET_CODES {
		OK_FSM_1 = 0,
		OK_FSM_2,
		OK_FSM_3,
		OK_FSM_4,
		ERROR_FSM,
		REPEAT_FSM
	};

	/*
	 * Define the structure for transition table
	 */
	typedef struct {
		enum FSM_STATES src_state;
		enum RET_CODES ret_code;
		enum FSM_STATES dst_state;
	} fsm_cell_t;

	/*
	 * Here all managing happens
	 */
	void fsm_loop(enum FSM_STATES init_state);

	/*
	 * Define all subroutines associated with FSM state
	 */
	enum RET_CODES do_startup(void);
	enum RET_CODES do_button_poll(void);
	enum RET_CODES do_led_turn_1(void);
	enum RET_CODES do_led_turn_2(void);
	enum RET_CODES do_led_turn_3(void);
	enum RET_CODES do_led_turn_4(void);

#endif /* INC_FSM_H_ */
