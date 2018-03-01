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
  int next_order = read_next_order();
  printf("Starting whileloop");
  run_states();
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
