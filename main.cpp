#include <iostream>
#include <openacc.h>

using namespace std;

int main()
{
    cout << "========== OpenACC Playground ==========\n";

    cout << "Host Devices    : "
         << acc_get_num_devices(acc_device_host)
         << endl;

    cout << "NVIDIA Devices  : "
         << acc_get_num_devices(acc_device_nvidia)
         << endl;

    cout << "Current Device  : "
         << acc_get_device_type()
         << endl;

    return 0;
}