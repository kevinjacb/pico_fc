#include <iostream>
#include <string>
using namespace std;

class VAR
{
public:
    //    int menu_item_count = 6;
    //    int menu_list_item_count[6] = {3, 0, 0, 0, 0, 0};
    int curr_menu_item = -1;
    //    int sub_menu_count[1] = {2},
    //        sub_menu_count_sum[1] = {2};

    string menu_items[6] = {"IMU Calibration", "GPS Calibration", "ESC Calibration", "PID tuning", "Receiver test", "Trimming"};
    int main_menu_count = 6;
    int menu_content_list_count[6] = {3, 0, 0, 0, 0, 0};
    string imu_menu[2] = {"Calibrate", "Back"};
    int imu_mcount = 2;

    void getMenu(string **items, int *count)
    {
        switch (curr_menu_item)
        {
        case 0:
            *items = imu_menu;
            *count = 2;
            break;
        }
        }
};

string opts[] = {"hello", "ylllep"};
void getString(string **options)
{
    *options = opts;
}
int main()
{

    VAR var = VAR();
    var.curr_menu_item = 0;
    string *options;
    int count;
    var.getMenu(&options, &count);
    cout << "yeah " << count << endl;
    cout << options[0] << endl;
    // for (int i = 0; i < 2; i++)
    //     cout << options[i] << endl;
    return 0;
}
