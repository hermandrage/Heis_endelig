#include "buttons.h"
#include "statemachine.h"
#include "elev.h"
#include "io.h"
#include "quecontroller.h"
#include <stdlib.h>
#include <stdio.h>


void read_command_button(void) {
  for (int etg = 0 ; etg<4 ; ++etg){
    if (elev_get_button_signal(BUTTON_COMMAND,etg)){
      add_order_to_que(etg, ORDER_WITHOUT_DIRECTION);
    }
  }
}

void read_up_and_down_buttons(void) {
  for (int etg = 0 ; etg<N_FLOORS ; ++etg){
    if (etg<N_FLOORS-1){
      if (elev_get_button_signal(BUTTON_CALL_UP,etg)){
        printf("HOHOHO!");
        add_order_to_que(etg, ORDER_UP);
      }
    }
    if(etg>0){
      if (elev_get_button_signal(BUTTON_CALL_DOWN,etg)){
        printf("HOHOHO!");
        add_order_to_que(etg, ORDER_DOWN);
      }
    }
  }
}

void read_stop_button(void){
  if (elev_get_stop_signal()){
    empty_que();
    elev_set_motor_direction(DIRN_STOP);
    while (elev_get_stop_signal()){
    }
    if (elev_get_floor_sensor_signal()!=-1){
      current_state=DOOR_OPEN;
    }
    else{
      current_state=IDLE;
    }
  }
}

void read_all_buttons(void){
  read_up_and_down_buttons();
  read_command_button();
  read_stop_button();
}
