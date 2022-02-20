// Win32Test.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include <windows.h>
#include <SetupAPI.h>
#include <winioctl.h>
#include <initguid.h>
#include <usbiodef.h>
#include <usbioctl.h>
#include <strsafe.h>
#include <memory>
#include <usbdi.h>
#include <usbuser.h>
#include <vector>

#include "uvcview.h"
#include "specializations.hpp"
#include "nlohmann/json.hpp"

#define TOOL_VERSION "0.1.1"


#define QUICKCOUT(...) std::cout << __VA_ARGS__ << std::endl


extern DEVICE_GUID_LIST gHubList;
extern DEVICE_GUID_LIST gDeviceList;

struct HandleDeleter
{
    // By defining the pointer type, we can delete a type other than T*.
    // In other words, we can declare unique_ptr<HANDLE, Deleter> instead of
    // unique_ptr<void, Deleter> which leaks the HANDLE abstraction.
    typedef HANDLE pointer;
 
    void operator()(HANDLE h)
    {
        if(h != INVALID_HANDLE_VALUE)
        {
            CloseHandle(h);
        }
    }
};

typedef std::unique_ptr<HANDLE, HandleDeleter> handle_unique_ptr;

