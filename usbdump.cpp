// usbdump.cpp : Defines the entry point for the application.
//

#include "usbdump.h"

using namespace std;
using json = nlohmann::json;

void initGlobals() {
    gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
    InitializeListHead(&gHubList.ListHead);
    gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
    InitializeListHead(&gDeviceList.ListHead);
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v") {
            std::cout << "usbdump " TOOL_VERSION << std::endl;
            std::cout << "Built on " __DATE__ " " __TIME__ << std::endl;
            return 0;
        } else {
            return 1;
        }
    }
    initGlobals();

    ULONG devicesConnected;

    std::vector<USBHOSTCONTROLLERINFO*> hostControllers = EnumerateHostControllers((HTREEITEM)1, &devicesConnected);
    json ree = hostControllers;
    std::cout << ree.dump(4) << std::endl;
    /* for (const auto& hostController : hostControllers) {
        // std::cout << hostController->DriverKey << std::endl;
        json ree = hostController;
        std::cout << ree.dump(4) << std::endl;
    } */

    // std::cout << "Done" << std::endl;
    return 0;
}
