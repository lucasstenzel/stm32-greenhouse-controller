// greenhouse_fsm.h
#ifndef GREENHOUSE_FSM_H
#define GREENHOUSE_FSM_H



typedef enum
{
    STATE_IDLE,
    STATE_HUMIDIFYING,
    STATE_VENTILATING,
    NUM_STATES
} State;

typedef enum
{
    EVENT_NONE,
    EVENT_HUMIDITY_LOW,     // RH below setpoint -> turn on humidifier
    EVENT_HUMIDITY_GOOD,    // RH sufficiently within setpoint range -> turn off humidifier
    EVENT_HUMIDITY_HIGH,    // RH above setpoint range -> turn on fan
    EVENT_CO2_HIGH,         // CO2 above limit -> turn on fan
    EVENT_CO2_GOOD,         // CO2 sufficiently within limit -> turn off fan
    NUM_EVENTS
} Event;

typedef void (*action_ptr)(void);

typedef struct
{
    State next_state;
    action_ptr action;
} transition;

// Function prototypes for state actions
void action_idle(void);
void action_humidify(void);
void action_ventilate(void);
void action_hold(void);

// State table array
extern transition fsm_table[NUM_STATES][NUM_EVENTS];

/**
 * @brief Determine the next state based on the current state and event, and execute the corresponding action
 * 
 * @param current - The current state of the FSM
 * @param input - The event that occurred
 * @return state -  The next state
 */
State update_fsm(State current, Event input);

#endif // GREENHOUSE_FSM_H