// greenhouse_fsm.c
#include "greenhouse_fsm.h"
#include "relays.h"

/**
 * @brief Turn both the humidifier and fan off
 */
void action_idle(void)
{
    relay_set(DEVICE_FAN, false);
    relay_set(DEVICE_HUMIDIFIER, false);
}

/**
 * @brief Turn the humidifier on but the fan off
 */
void action_humidify(void)
{
    relay_set(DEVICE_FAN, false);
    relay_set(DEVICE_HUMIDIFIER, true);
}

/**
 * @brief Turn the fan on but the humidifier off
 */
void action_ventilate(void)
{
    relay_set(DEVICE_FAN, true);
    relay_set(DEVICE_HUMIDIFIER, false);
}

void action_hold(void)
{
    // Could add NOP here for debugging
}

// State table definition
// I have prioritized ventilation over humidification
transition fsm_table[NUM_STATES][NUM_EVENTS] =
{
    /*                  NONE,                               HUMIDITY_LOW,                           HUMIDITY_HIGH,                          CO2_HIGH                                ALL_GOOD      */
    /* IDLE */          {{STATE_IDLE, action_hold},         {STATE_HUMIDIFYING, action_humidify},   {STATE_VENTILATING, action_ventilate},  {STATE_VENTILATING, action_ventilate},  {STATE_IDLE, action_hold}},
    /* HUMIDIFYING */   {{STATE_HUMIDIFYING, action_hold},  {STATE_HUMIDIFYING, action_hold},       {STATE_VENTILATING, action_ventilate},  {STATE_VENTILATING, action_ventilate},  {STATE_IDLE, action_idle}},
    /* VENTILATING */   {{STATE_VENTILATING, action_hold},  {STATE_HUMIDIFYING, action_humidify},   {STATE_VENTILATING, action_hold},       {STATE_VENTILATING, action_hold},       {STATE_IDLE, action_idle}},
};

State update_fsm(State current, Event input)
{
    // Get the transition for the current state and input event
    transition t = fsm_table[current][input];

    // Execute the action associated with the transition
    //t.action();
    (*t.action)();

    // Return the next state
    return t.next_state;
}