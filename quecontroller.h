typedef enum order_direction {
    NO_ORDER = 0,
    ORDER_UP = 1,
    ORDER_DOWN = 2,
    ORDER_WITHOUT_DIRECTION = 3
}order_dir;




void empty_que(void);
void print_que(void);
int read_next_order(int floor);
int check_for_order(int floor, int direction);
int check_if_should_stop(int floor, int direcion);
void add_order_to_que(int floor,order_dir direction);
void delete_order_form_que(int order_nr);
void testing(void);
