#include"lights.h"
#include"elev.h"
#include"io.h"
#include"statemachine.h"
#include<stdlib.h>
#include <stdio.h>
#include <time.h>

//Sets the floor lights (Runde små lamper på siden av kontrolleren).
void actual_set_floor_lights(void){  //funksjon som tar inn floor_sensor og setter lys, samt kaller funksjoner som oppdaterer floor_indicator_lights_matrix
  if (get_current_floor() >=0 && get_current_floor() <N_FLOORS && elev_get_floor_indicator_matrix(get_current_floor())==0){
	//printf("in floor %d \n", curr_floor);
    elev_set_floor_indicator(get_current_floor());
  }
}


void set_command_button_lights(void){ //setter lys inni heis når knapp trykkes, og skrur av når heis er i aktuell etasje
  int temp_var_floor_sensor_signal=elev_get_floor_sensor_signal();
  for (int etg = 0 ; etg<4 ; ++etg){
    if (elev_get_button_signal(BUTTON_COMMAND,etg)){
      elev_set_button_lamp(BUTTON_COMMAND,etg,1);
    }
  }
  if (temp_var_floor_sensor_signal >=0 && temp_var_floor_sensor_signal<N_FLOORS  && get_lamp_channel_matrix(temp_var_floor_sensor_signal, BUTTON_COMMAND) >= 1){
    elev_set_button_lamp(BUTTON_COMMAND,temp_var_floor_sensor_signal,0);
  }
}

//Sets the light in the up and dow buttons. is not finished. Intended functionality: Lights up after pressed, and stop when order is finished.
void set_up_and_down_button_lights(void){
  int temp_var_floor_sensor_signal=elev_get_floor_sensor_signal();
  for (int etg = 0 ; etg<N_FLOORS ; ++etg){
    if (etg<(N_FLOORS-1) && elev_get_button_signal(BUTTON_CALL_UP,etg)){
      printf("opp-knapp-lampe skal skrus på\n");
      elev_set_button_lamp(BUTTON_CALL_UP,etg,1);
    }
  }
  if (temp_var_floor_sensor_signal >=0 && temp_var_floor_sensor_signal<(N_FLOORS-1)  && get_lamp_channel_matrix(temp_var_floor_sensor_signal, BUTTON_CALL_UP) >= 1){
    elev_set_button_lamp(BUTTON_CALL_UP,temp_var_floor_sensor_signal,0);
  }
}
/*
  for (int i = 0 ; i<N_FLOORS ; ++i){
    if (i<(N_FLOORS-1) && elev_get_button_signal(BUTTON_CALL_UP,i)){
      elev_set_button_lamp(BUTTON_CALL_UP,i,1);
    }
    //Skru av hvis heisen stopper ved gitt etasje.
    if (i>0 && elev_get_button_signal(BUTTON_CALL_DOWN,i)){
      elev_set_button_lamp(BUTTON_CALL_DOWN,i,1);
    }
  }
//legg inn funksjonalitet for at lys stopper når heis stopper i aktuell etasje
}
*/

//Is not finished. Lights up when pressed, stops when it's not pressed down anny more:)
void stop_button (){
  while (elev_get_stop_signal()){
        elev_set_stop_lamp(1);
        elev_set_motor_direction(DIRN_STOP);
    //SLETT KØ
  }
  elev_set_stop_lamp(0);
  //FÅ HEIS TIL Å STÅ I RO FRAM TIL NY BESTILLING MOTTAT

}

void open_door(void){
  elev_set_door_open_lamp(1);
  clock_t before=clock();
  clock_t difference = clock();
  long millisec=0;
  long trigger=1000;
  int count=0;
  while(millisec < trigger) {
    difference=clock()-before;
    millisec=difference*(CLOCKS_PER_SEC/1000);
    printf("STUCK IN DOOR-LOOP\n %d",count);
  }
  elev_set_door_open_lamp(0);
}
