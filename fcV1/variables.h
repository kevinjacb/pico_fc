#ifndef VAR_CLASS
#define VAR_CLASS

class VAR
{
public:
    int menu_item_count = 6;
    int menu_list_item_count[6] = {3, 0, 0, 0, 0, 0};
    int curr_menu_item = -1;
    String sub_menu_items[2] = {"Calibrate", "Back"};
    int sub_menu_count[1] = {2},
        sub_menu_count_sum[1] = {2};

    typedef struct
    {
        String menu_items[6] = {"IMU Calibration", "GPS Calibration", "ESC Calibration", "PID tuning", "Receiver test", "Trimming"};
        int main_menu_count = 6;
        int menu_content_list_count[6] = {3, 0, 0, 0, 0, 0};
        String imu_menu[2] = {"Calibrate", "Back"};
        //    gps_menu[] =,     //TODO
        //    esc_menu[] =,
        //    pid_menu[] =,
        //    receiver_menu[] =,
        //    trim_menu[] =;

    } menuHandler;
};
#endif