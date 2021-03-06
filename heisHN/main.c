#include "elev.h"
#include <stdio.h>
#include "lights.h"
#include "statemachine.h"
#include "io.h"
#include "buttons.h"
#include "quecontroller.h"
#include "timer.h"




int main(){
  if (!elev_init()) {
      printf("Unable to initialize elevator hardware!\n");
      return 1;
  }
  printf("Starting whileloop");
  while (1){
      switch (current_state){
        case IDLE: // HER STARTER STATEN
        printf("Entered IDLE");
        if (current_direction!=DIRN_STOP){//Stopper heisen
          current_direction=DIRN_STOP;
        }
        //les input
        //reads next order and sets current_state
        int next_order = read_next_order();
        if (next_order== -1){
        }
        else if (next_order - get_current_floor() <0){
          current_state=DRIVE_DOWN;
        }
        else if (next_order - get_current_floor() >0){
          current_state=DRIVE_UP;
        }
        else if (next_order - get_current_floor() ==0){
          current_state=DOOR_OPEN;
        }
        read_all_buttons();
        break;
        //////////------------------------------------------------------------------------------------

        case DOOR_OPEN:// HER STARTER STATEN
        printf("Enterd DOOR_OPEN");
        if (current_direction!=DIRN_STOP){//Stopper heisen
          current_direction=DIRN_STOP;
        }
        if (timer_is_timeout() == -1){//Starter timer hvis den ikke allerede er startet.
          start_timer();
          elev_set_door_open_lamp(1);
        }
        //les iput
        if (timer_is_timeout()){ //reads next order and sets current_state

          elev_set_door_open_lamp(0);
          int next_order = read_next_order();
          if (next_order== -1){
            current_state=IDLE;
          }
          else if (next_order - get_current_floor() <0){
            current_state=DRIVE_DOWN;
          }
          else if (next_order - get_current_floor() >0){
            current_state=DRIVE_UP;
          }
        }
        read_all_buttons();
        break;
        //////////------------------------------------------------------------------------------------

        case DRIVE_UP: // HER STARTER STATEN
        printf("Entered DRIVE_UP");
        if (current_direction!=DIRN_UP){//Setter riktig retning
          current_direction=DIRN_STOP;
        }
        int temp_current_floor=elev_get_floor_sensor_signal();//mellomlagrer etsje/-1 dersom mellom etasjer.

        if ( temp_current_floor != -1){//Slår ut om vi er i en etasje.
          set_floor_variables();//oppdaterer curren og previous_floor
          int temp_order_number= check_if_should_stop(get_current_floor(), ORDER_UP);//mellomlagrer ordrenummeret til orderen vi eventuelt skal stoppe i, tar også hensyn til første order.

          while (temp_order_number != -1){// Bruker while i tillfelle det er flere bestillinger som blir utført og dermed skal slettes.
            current_state = DOOR_OPEN;
            delete_order_from_que(temp_order_number);
            temp_order_number= check_if_should_stop(get_current_floor(), ORDER_UP);//Sjekker på nytt om det er flere ordre som blir utført.
          }
        }
        read_all_buttons();
        break;
        //////////------------------------------------------------------------------------------------

        case DRIVE_DOWN:// HER STARTER STATEN
        printf("Enterd DRIVE_DOWN");
        if (current_direction!=DIRN_DOWN){
          current_direction=DIRN_DOWN;
        }

        int temp_current_floor2=elev_get_floor_sensor_signal();//mellomlagrer etsje/-1 dersom mellom etasjer.

        if ( temp_current_floor2 != -1){//Slår ut om vi er i en etasje.
          set_floor_variables();//oppdaterer curren og previous_floor
          int temp_order_number= check_if_should_stop(get_current_floor(), ORDER_DOWN);//mellomlagrer ordrenummeret til orderen vi eventuelt skal stoppe i, tar også hensyn til første order.

          while (temp_order_number != -1){// Bruker while i tillfelle det er flere bestillinger som blir utført og dermed skal slettes.
            current_state = DOOR_OPEN;
            delete_order_from_que(temp_order_number);
            temp_order_number= check_if_should_stop(get_current_floor(), ORDER_DOWN);//Sjekker på nytt om det er flere ordre som blir utført.
          }
        }
        read_all_buttons();
        break;
  }
}
}

/*


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");
//jgfhh
    elev_set_motor_direction(DIRN_UP);
    //testing();
    start_timer();
    elev_set_door_open_lamp(1);
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        //kode vi har satt inn selv
        set_floor_variables();
        actual_set_floor_lights();
        //actual_set_floor_lights();
        set_command_button_lights();
        set_up_and_down_button_lights();
        read_command_button();
        read_up_and_down_buttons();
        //read_stop_button();
        //print_que();
        //testing();
        //stop_button ();
        if (timer_is_timeout()){
          elev_set_door_open_lamp(0);
          printf("Door close \n");
        }





        //g
    }

    return 0;
}

*/
