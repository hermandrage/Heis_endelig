
void set_floor_variables(void);
int get_current_floor(void);
int get_previous_floor(void);

typedef enum state_type {
  IDLE = 0,
  DOOR_OPEN = 1,
  DRIVE_UP = 2,
  DRIVE_DOWN=3,
} state_t;


static state_t current_state;
