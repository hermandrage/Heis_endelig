#include "statemachine.h"
#include "elev.h"
#include "io.h"
#include "quecontroller.h"

#include <stdio.h>
#include <stdlib.h>

int current_floor=-1;
int previous_floor=-1;

int get_current_floor(void) {
  return current_floor;
}

int get_previous_floor(void){
  return previous_floor;
}
// setter variabelene cuttent_floor og previous_floor til riktig etasje.
void set_floor_variables(void){
  int temp_get_floor = elev_get_floor_sensor_signal();
  if (temp_get_floor!=current_floor && temp_get_floor != -1){
    previous_floor=current_floor;
  }
  if (temp_get_floor > -1 && temp_get_floor < N_FLOORS){
    current_floor = temp_get_floor;
  }
  //printf("current_floor: %d\n", current_floor );
  //printf("previous_floor: %d\n", previous_floor );
}


void print_status(){
  printf("QUE:\n");
  print_que();
  printf("\n");
  printf("VARIABLES:\n");
  printf("CURRENT FLOOR: %d\n", get_current_floor());
  printf("CURRENT DIRECTION    %d\n", get_current_dircetion() );
}

void set_current_state(state_t state){
  current_state=state;
}

