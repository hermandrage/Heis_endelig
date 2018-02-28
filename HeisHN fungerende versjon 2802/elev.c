// Wrapper for libComedi Elevator control.
// These functions provides an interface to the elevators in the real time lab
//
// 2007, Martin Korsgaard


#include "channels.h"
#include "elev.h"
#include "io.h"
#include "statemachine.h"
#include "quecontroller.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>



// Number of signals and lamps on a per-floor basis (excl sensor)
#define N_BUTTONS 3

static const int lamp_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {LIGHT_UP1, LIGHT_DOWN1, LIGHT_COMMAND1},
    {LIGHT_UP2, LIGHT_DOWN2, LIGHT_COMMAND2},
    {LIGHT_UP3, LIGHT_DOWN3, LIGHT_COMMAND3},
    {LIGHT_UP4, LIGHT_DOWN4, LIGHT_COMMAND4},
};


static const int button_channel_matrix[N_FLOORS][N_BUTTONS] = {
    {BUTTON_UP1, BUTTON_DOWN1, BUTTON_COMMAND1},
    {BUTTON_UP2, BUTTON_DOWN2, BUTTON_COMMAND2},
    {BUTTON_UP3, BUTTON_DOWN3, BUTTON_COMMAND3},
    {BUTTON_UP4, BUTTON_DOWN4, BUTTON_COMMAND4},
};

static int floor_indicator_lights_matrix [N_FLOORS] = { // DENNE HAR VI LAGET SELV - lagrer om floor_inditator_lights er på eller ikke. HURRA
  0 , 0 , 0 , 0
};

int get_current_dircetion(void){
  return current_direction;
}

int elev_init(void) {
    int i;

    // Init hardware
    if (!io_init())
        return 0;

    // Zero all floor button lamps
    for (i = 0; i < N_FLOORS; ++i) {
        if (i != 0)
            elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);

        if (i != N_FLOORS - 1)
            elev_set_button_lamp(BUTTON_CALL_UP, i, 0);

        elev_set_button_lamp(BUTTON_COMMAND, i, 0);
    }

    // Clear stop lamp, door open lamp, and set floor indicator to ground floor.
    elev_set_stop_lamp(0);
    elev_set_door_open_lamp(0);
    elev_set_floor_indicator(0);

    //Dette har me gjort sjøv:)
    empty_que();
    if (elev_get_floor_sensor_signal() ==-1){
      elev_set_motor_direction(DIRN_UP);
      while( elev_get_floor_sensor_signal()==-1){
      }
    }
    elev_set_motor_direction(DIRN_STOP);
    set_floor_variables();
    current_state=IDLE;
    current_direction=DIRN_STOP;
    printf("Elev_init is finished");

    // Return success.
    return 1;
}

void elev_set_motor_direction(elev_motor_direction_t dirn) {
    if (dirn == 0){
        io_write_analog(MOTOR, 0);
    } else if (dirn > 0) {
        io_clear_bit(MOTORDIR);
        io_write_analog(MOTOR, 2800);
    } else if (dirn < 0) {
        io_set_bit(MOTORDIR);
        io_write_analog(MOTOR, 2800);
    }
}

void elev_set_door_open_lamp(int value) {
    if (value)
        io_set_bit(LIGHT_DOOR_OPEN);
    else
        io_clear_bit(LIGHT_DOOR_OPEN);
}

int elev_get_obstruction_signal(void) {
    return io_read_bit(OBSTRUCTION);
}

int elev_get_stop_signal(void) {
    return io_read_bit(STOP);
}

void elev_set_stop_lamp(int value) {
    if (value)
        io_set_bit(LIGHT_STOP);
    else
        io_clear_bit(LIGHT_STOP);
}

int elev_get_floor_sensor_signal(void) {
    if (io_read_bit(SENSOR_FLOOR1))
        return 0;
    else if (io_read_bit(SENSOR_FLOOR2))
        return 1;
    else if (io_read_bit(SENSOR_FLOOR3))
        return 2;
    else if (io_read_bit(SENSOR_FLOOR4))
        return 3;
    else
        return -1;
}

void elev_set_floor_indicator(int floor) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    // Binary encoding. One light must always be on.
    if (floor & 0x02)
        io_set_bit(LIGHT_FLOOR_IND1);
    else
        io_clear_bit(LIGHT_FLOOR_IND1);

    if (floor & 0x01)
        io_set_bit(LIGHT_FLOOR_IND2);
    else
        io_clear_bit(LIGHT_FLOOR_IND2);
    //Detta har me skrevve sjøl:)
    for (int temp_etasje=0; temp_etasje<N_FLOORS; temp_etasje++){
      floor_indicator_lights_matrix[temp_etasje]=0;
    }
    floor_indicator_lights_matrix[floor]=1;
    //
}
//Denna funksjonen har me laga sjøv:)
int elev_get_floor_indicator_matrix(int etasje){
  //printf("etasje1 %d\n", floor_indicator_lights_matrix[0]);
  //printf("etasje2 %d\n", floor_indicator_lights_matrix[1]);
  //printf("etasje3 %d\n", floor_indicator_lights_matrix[2]);
  //printf("etasje4 %d\n", floor_indicator_lights_matrix[3]);
  return floor_indicator_lights_matrix[etasje];
}
int get_lamp_channel_matrix(int floor, elev_button_type_t button){
  return lamp_channel_matrix[floor][button];
}
//
int elev_get_button_signal(elev_button_type_t button, int floor) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS - 1));
    assert(!(button == BUTTON_CALL_DOWN && floor == 0));
    assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button == BUTTON_COMMAND);

    if (io_read_bit(button_channel_matrix[floor][button]))
        return 1;
    else
        return 0;
}

void elev_set_button_lamp(elev_button_type_t button, int floor, int value) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS - 1));
    assert(!(button == BUTTON_CALL_DOWN && floor == 0));
    assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button == BUTTON_COMMAND);

    if (value)
        io_set_bit(lamp_channel_matrix[floor][button]);
    else
        io_clear_bit(lamp_channel_matrix[floor][button]);
}

//funksjoner vi har laget selv
