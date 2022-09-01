#ifndef VAR_CLASS
#define VAR_CLASS

class VAR
{
public:
    String menu_items[6] = {"IMU Calibration", "GPS Calibration", "ESC Calibration", "PID tuning", "Receiver test", "Trimming"};
    int menu_item_count = 6;
    int menu_list_item_count[6] = {3, 0, 0, 0, 0, 0};
    int curr_menu_item = -1;
    String sub_menu_items[2] = {"Calibrate", "Back"};
    int sub_menu_count[1] = {2},
        sub_menu_count_sum[1] = {2};
};
#endif