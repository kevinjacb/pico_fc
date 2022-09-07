#ifndef VAR_CLASS
#define VAR_CLASS

class VAR
{
public:
    //    int menu_item_count = 6;
    //    int menu_list_item_count[6] = {3, 0, 0, 0, 0, 0};
    int curr_menu_item = -1;
    //    int sub_menu_count[1] = {2},
    //        sub_menu_count_sum[1] = {2};

    String menu_items[6] = {"IMU Calibration", "GPS Calibration", "ESC Calibration", "PID tuning", "Receiver test", "Trimming"};
    int main_menu_count = 6;
    int menu_content_list_count[6] = {3, 0, 0, 0, 0, 0};
    String imu_menu[2] = {"Calibrate", "Back"};
    int imu_mcount = 2;
    //    gps_menu[] =,     //TODO
    //    esc_menu[] =,
    //    pid_menu[] =,
    //    receiver_menu[] =,
    //    trim_menu[] =;

    void getMenu(String **items, int *count)
    {
        Serial.printf(" curr menu item -> %d\n", this->curr_menu_item);
        switch (this->curr_menu_item)
        {
        case 0:
            *items = imu_menu;
            *count = imu_mcount;
            break;
        }
    }
};
#endif
