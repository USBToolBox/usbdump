#pragma once

#include "uvcview.h"
#include "nlohmann/json.hpp"
#include "debugapi.h"

#include <locale>
#include <codecvt>


using json = nlohmann::json;

// https://stackoverflow.com/a/3999597
// Thank you tfinniga!

// Convert a wide Unicode string to an UTF8 string
static std::string utf8_encode(const std::wstring &wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// Convert an UTF8 string to a wide Unicode String
static std::wstring utf8_decode(const std::string &str)
{
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
#pragma clang diagnostic pop


#define VA_LIST(...) __VA_ARGS__

#define ADD_JSON_STRUCT(a, unsafe, safe) \
template<> \
struct adl_serializer<a> { \
    static void to_json(json& j, const a& p) { \
        if (p == NULL) { \
            j = nullptr; \
        } else { \
            j = json { \
                unsafe \
            }; \
            safe \
        } \
    } \
};


#define ADD_JSON_STRUCT_DIRECT(a, unsafe, safe) \
template<> \
struct adl_serializer<a> { \
    static void to_json(json& j, const a& p) { \
        j = json { \
            unsafe \
        }; \
        safe \
    } \
};

#define _ADD_JSON_ENTRY(a, b) {#a, b},
#define ADD_JSON_ENTRY(a) _ADD_JSON_ENTRY(a, p->a)
#define ADD_JSON_ENTRY_DIRECT(a) _ADD_JSON_ENTRY(a, p.a)

#ifdef SERIALIZE_VERBOSE
#define ADD_JSON_ENTRY_VERBOSE(a) ADD_JSON_ENTRY(a)
#define ADD_JSON_ENTRY_DIRECT_VERBOSE(a) ADD_JSON_ENTRY_DIRECT(a)
#else
#define ADD_JSON_ENTRY_VERBOSE(a)
#define ADD_JSON_ENTRY_DIRECT_VERBOSE(a)
#endif

#define _ADD_JSON_ENTRY_SAFE(a, b) \
do { \
    if (b == NULL) { \
        j[#a] = nullptr; \
    } else { \
        j[#a] = b; \
    } \
} while (0);

#define ADD_JSON_ENTRY_SAFE(a) _ADD_JSON_ENTRY_SAFE(a, p->a)
#define ADD_JSON_ENTRY_DIRECT_SAFE(a) _ADD_JSON_ENTRY_SAFE(a, p.a)



#define _ADD_JSON_ENTRY_SAFE_WIDE(a, b) \
do { \
    if (b == NULL) { \
        j[#a] = nullptr; \
    } else { \
        j[#a] = utf8_encode(std::wstring(b)); \
    } \
} while (0);


#define ADD_JSON_ENTRY_SAFE_WIDE(a) _ADD_JSON_ENTRY_SAFE_WIDE(a, p->a)
#define ADD_JSON_ENTRY_DIRECT_SAFE_WIDE(a) _ADD_JSON_ENTRY_SAFE_WIDE(a, p.a)

#ifdef SERIALIZE_VERBOSE
#define ADD_JSON_ENTRY_SAFE_VERBOSE(a) ADD_JSON_ENTRY_SAFE(a)
#define ADD_JSON_ENTRY_DIRECT_SAFE_VERBOSE(a) ADD_JSON_ENTRY_DIRECT_SAFE(a)

#define ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(a) ADD_JSON_ENTRY_SAFE_WIDE(a)
#define ADD_JSON_ENTRY_DIRECT_SAFE_WIDE_VERBOSE(a) ADD_JSON_ENTRY_DIRECT_SAFE_WIDE(a)
#else
#define ADD_JSON_ENTRY_SAFE_VERBOSE(a)
#define ADD_JSON_ENTRY_DIRECT_SAFE_VERBOSE(a)

#define ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(a)
#define ADD_JSON_ENTRY_DIRECT_SAFE_WIDE_VERBOSE(a)
#endif

#define ADD_JSON_ENUM_ENTRY(a) {a, #a}

NLOHMANN_JSON_SERIALIZE_ENUM( USBDEVICEINFOTYPE, {
    ADD_JSON_ENUM_ENTRY(HostControllerInfo),
    ADD_JSON_ENUM_ENTRY(RootHubInfo),
    ADD_JSON_ENUM_ENTRY(ExternalHubInfo),
    ADD_JSON_ENUM_ENTRY(DeviceInfo),
})

NLOHMANN_JSON_SERIALIZE_ENUM( USB_HUB_NODE, {
    ADD_JSON_ENUM_ENTRY(UsbHub),
    ADD_JSON_ENUM_ENTRY(UsbMIParent)
})

NLOHMANN_JSON_SERIALIZE_ENUM(USB_CONNECTION_STATUS, {
    ADD_JSON_ENUM_ENTRY(NoDeviceConnected),
    ADD_JSON_ENUM_ENTRY(DeviceConnected),
    ADD_JSON_ENUM_ENTRY(DeviceFailedEnumeration),
    ADD_JSON_ENUM_ENTRY(DeviceGeneralFailure),
    ADD_JSON_ENUM_ENTRY(DeviceCausedOvercurrent),
    ADD_JSON_ENUM_ENTRY(DeviceNotEnoughPower),
    ADD_JSON_ENUM_ENTRY(DeviceNotEnoughBandwidth),
    ADD_JSON_ENUM_ENTRY(DeviceHubNestedTooDeeply),
    ADD_JSON_ENUM_ENTRY(DeviceInLegacyHub),
    ADD_JSON_ENUM_ENTRY(DeviceEnumerating),
    ADD_JSON_ENUM_ENTRY(DeviceReset)
})

NLOHMANN_JSON_SERIALIZE_ENUM(USB_CONTROLLER_FLAVOR, {
    ADD_JSON_ENUM_ENTRY(USB_HcGeneric),
    ADD_JSON_ENUM_ENTRY(OHCI_Generic),
    ADD_JSON_ENUM_ENTRY(OHCI_Hydra),
    ADD_JSON_ENUM_ENTRY(OHCI_NEC),
    ADD_JSON_ENUM_ENTRY(UHCI_Generic),
    ADD_JSON_ENUM_ENTRY(UHCI_Piix4),
    ADD_JSON_ENUM_ENTRY(UHCI_Piix3),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich2),
    ADD_JSON_ENUM_ENTRY(UHCI_Reserved204),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich1),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich3m),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich4),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich5),
    ADD_JSON_ENUM_ENTRY(UHCI_Ich6),
    ADD_JSON_ENUM_ENTRY(UHCI_Intel),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA_x01),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA_x02),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA_x03),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA_x04),
    ADD_JSON_ENUM_ENTRY(UHCI_VIA_x0E_FIFO),
    ADD_JSON_ENUM_ENTRY(EHCI_Generic),
    ADD_JSON_ENUM_ENTRY(EHCI_NEC),
    ADD_JSON_ENUM_ENTRY(EHCI_Lucent),
    ADD_JSON_ENUM_ENTRY(EHCI_NVIDIA_Tegra2),
    ADD_JSON_ENUM_ENTRY(EHCI_NVIDIA_Tegra3),
    ADD_JSON_ENUM_ENTRY(EHCI_Intel_Medfield)
})

NLOHMANN_JSON_SERIALIZE_ENUM(DEVICE_POWER_STATE, {
    ADD_JSON_ENUM_ENTRY(PowerDeviceUnspecified),
    ADD_JSON_ENUM_ENTRY(PowerDeviceD0),
    ADD_JSON_ENUM_ENTRY(PowerDeviceD1),
    ADD_JSON_ENUM_ENTRY(PowerDeviceD2),
    ADD_JSON_ENUM_ENTRY(PowerDeviceD3),
    ADD_JSON_ENUM_ENTRY(PowerDeviceMaximum)
})

namespace nlohmann {
    ADD_JSON_STRUCT(PUSB_CONTROLLER_INFO_0,
        VA_LIST(
            ADD_JSON_ENTRY(PciVendorId)
            ADD_JSON_ENTRY(PciDeviceId)
            ADD_JSON_ENTRY(PciRevision)
            ADD_JSON_ENTRY(NumberOfRootPorts)
            ADD_JSON_ENTRY(ControllerFlavor)
            ADD_JSON_ENTRY(HcFeatureFlags)
        ), 
    )

    ADD_JSON_STRUCT(PUSB_DEVICE_PNP_STRINGS, ,VA_LIST(
        ADD_JSON_ENTRY_SAFE_WIDE(DeviceId)
        ADD_JSON_ENTRY_SAFE_WIDE(DeviceDesc)
        ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(HwId)
        ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(Service)
        ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(DeviceClass)
        ADD_JSON_ENTRY_SAFE_WIDE_VERBOSE(PowerState)
    ))

    ADD_JSON_STRUCT_DIRECT(USB_HUB_DESCRIPTOR, VA_LIST(
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDescriptorLength)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDescriptorType)
        ADD_JSON_ENTRY_DIRECT(bNumberOfPorts)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(wHubCharacteristics)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bPowerOnToPowerGood)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bHubControlCurrent)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bRemoveAndPowerMask)
    ),)

    ADD_JSON_STRUCT_DIRECT(USB_30_HUB_DESCRIPTOR, VA_LIST(
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bLength)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDescriptorType)
        ADD_JSON_ENTRY_DIRECT(bNumberOfPorts)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(wHubCharacteristics)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bPowerOnToPowerGood)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bHubControlCurrent)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bHubHdrDecLat)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(wHubDelay)
        ADD_JSON_ENTRY_DIRECT(DeviceRemovable)
    ),)

    ADD_JSON_STRUCT_DIRECT(USB_HUB_INFORMATION, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(HubDescriptor)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(HubIsBusPowered)
    ),)
    
    ADD_JSON_STRUCT_DIRECT(USB_MI_PARENT_INFORMATION, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(NumberOfInterfaces)
    ),)

    template<>
    struct adl_serializer<PUSB_NODE_INFORMATION> {
        static void to_json(json& j, const PUSB_NODE_INFORMATION& p) {
            if (p == NULL) {
                j = nullptr;
            } else {
                j = json {
                    ADD_JSON_ENTRY(NodeType)
                };
                if (p->NodeType == UsbHub) {
                    j["HubInformation"] = p->u.HubInformation;
                }
                #ifdef SERIALIZE_VERBOSE
                else {
                    j["MiParentInformation"] = p->u.MiParentInformation;
                }
                #endif
            }
        }
    };
    template<>
    struct adl_serializer<PUSB_HUB_INFORMATION_EX> {
        static void to_json(json& j, const PUSB_HUB_INFORMATION_EX& p) {
            if (p == NULL) {
                j = nullptr;
            } else {
                j = json {
                    ADD_JSON_ENTRY(HubType)
                    ADD_JSON_ENTRY(HighestPortNumber)
                };
                if (p->HubType == Usb30Hub) {
                    j["Usb30HubDescriptor"] = p->u.Usb30HubDescriptor;
                } else {
                    j["UsbHubDescriptor"] = p->u.UsbHubDescriptor;
                }
            }
        }
    };

    ADD_JSON_STRUCT_DIRECT(USB_PORT_PROPERTIES, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(PortIsUserConnectable)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(PortIsDebugCapable)
        ADD_JSON_ENTRY_DIRECT(PortHasMultipleCompanions)
        ADD_JSON_ENTRY_DIRECT(PortConnectorIsTypeC)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(ReservedMBZ)
    ),)

    template <>
    struct adl_serializer<PUSB_PORT_CONNECTOR_PROPERTIES> {
        static void to_json(json& j, const PUSB_PORT_CONNECTOR_PROPERTIES& p) {
            if (p == 0) {
                j = nullptr;
            } else {
                j = json{
                    ADD_JSON_ENTRY(ConnectionIndex)
                    ADD_JSON_ENTRY_VERBOSE(ActualLength)
                    ADD_JSON_ENTRY(UsbPortProperties)
                    ADD_JSON_ENTRY_VERBOSE(CompanionIndex)
                    ADD_JSON_ENTRY(CompanionPortNumber)
                };
                j["CompanionHubSymbolicLinkName"] = utf8_encode(std::wstring(p->CompanionHubSymbolicLinkName));
            }
        }
    };

    ADD_JSON_STRUCT(PDEVICE_INFO_NODE, VA_LIST(
            ADD_JSON_ENTRY_VERBOSE(DeviceDescNameLength)
            ADD_JSON_ENTRY_VERBOSE(DeviceDriverNameLength)
            ADD_JSON_ENTRY_VERBOSE(LatestDevicePowerState)
        ),
        VA_LIST(
            ADD_JSON_ENTRY_SAFE_WIDE(DeviceDescName)
            ADD_JSON_ENTRY_SAFE_WIDE(DeviceDriverName)
        )
    )

    ADD_JSON_STRUCT_DIRECT(USB_HUB_CAP_FLAGS, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(HubIsHighSpeedCapable)
        ADD_JSON_ENTRY_DIRECT(HubIsHighSpeed)
        ADD_JSON_ENTRY_DIRECT(HubIsMultiTtCapable)
        ADD_JSON_ENTRY_DIRECT(HubIsMultiTt)
        ADD_JSON_ENTRY_DIRECT(HubIsRoot)
        ADD_JSON_ENTRY_DIRECT(HubIsArmedWakeOnConnect)
        ADD_JSON_ENTRY_DIRECT(HubIsBusPowered)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(ReservedMBZ)
    ),)

    ADD_JSON_STRUCT(PUSB_HUB_CAPABILITIES_EX, VA_LIST(
        ADD_JSON_ENTRY(CapabilityFlags)
    ),)

    template<typename T>
    struct adl_serializer<std::vector<T>> {
        static void to_json(json& j, const std::vector<T>& p) {
            for (const auto& element : p) {
                j.push_back(element);
            }
        }
    };

    ADD_JSON_STRUCT(PUSBROOTHUBINFO, VA_LIST(
            ADD_JSON_ENTRY(DeviceInfoType)
            ADD_JSON_ENTRY(HubPorts)
        ),
        VA_LIST(
            ADD_JSON_ENTRY_SAFE(HubInfo)
            ADD_JSON_ENTRY_SAFE(HubInfoEx)
            ADD_JSON_ENTRY_SAFE_WIDE(HubName)
            ADD_JSON_ENTRY_SAFE(PortConnectorProps)
            ADD_JSON_ENTRY_SAFE_VERBOSE(UsbDeviceProperties)
            ADD_JSON_ENTRY_SAFE(DeviceInfoNode)
            ADD_JSON_ENTRY_SAFE_VERBOSE(HubCapabilityEx)
        )
    )


    ADD_JSON_STRUCT_DIRECT(USB_DEVICE_DESCRIPTOR, VA_LIST(
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bLength)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDescriptorType)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bcdUSB)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDeviceClass)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDeviceSubClass)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bDeviceProtocol)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bMaxPacketSize0)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(idVendor)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(idProduct)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bcdDevice)
        ADD_JSON_ENTRY_DIRECT(iManufacturer)
        ADD_JSON_ENTRY_DIRECT(iProduct)
        ADD_JSON_ENTRY_DIRECT(iSerialNumber)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(bNumConfigurations)
    ),)


    ADD_JSON_STRUCT_DIRECT(USB_ENDPOINT_DESCRIPTOR, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(bLength)
        ADD_JSON_ENTRY_DIRECT(bDescriptorType)
        ADD_JSON_ENTRY_DIRECT(bEndpointAddress)
        ADD_JSON_ENTRY_DIRECT(bmAttributes)
        ADD_JSON_ENTRY_DIRECT(wMaxPacketSize)
        ADD_JSON_ENTRY_DIRECT(bInterval)
    ),)

    ADD_JSON_STRUCT(PUSB_PIPE_INFO, VA_LIST(
        ADD_JSON_ENTRY(EndpointDescriptor)
        ADD_JSON_ENTRY(ScheduleOffset)
    ),)
    ADD_JSON_STRUCT_DIRECT(USB_PIPE_INFO, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(EndpointDescriptor)
        ADD_JSON_ENTRY_DIRECT(ScheduleOffset)
    ),)


    template<>
    struct adl_serializer<PUSB_NODE_CONNECTION_INFORMATION_EX> {
        static void to_json(json& j, const PUSB_NODE_CONNECTION_INFORMATION_EX& p) {
            if (p == NULL) {
                j = nullptr;
            } else {
                j = json {
                    ADD_JSON_ENTRY(ConnectionIndex)
                    ADD_JSON_ENTRY(DeviceDescriptor)
                    ADD_JSON_ENTRY_VERBOSE(CurrentConfigurationValue)
                    ADD_JSON_ENTRY(Speed)
                    ADD_JSON_ENTRY_VERBOSE(DeviceIsHub)
                    ADD_JSON_ENTRY_VERBOSE(DeviceAddress)
                    ADD_JSON_ENTRY_VERBOSE(NumberOfOpenPipes)
                    ADD_JSON_ENTRY(ConnectionStatus)
                };
                #ifdef SERIALIZE_VERBOSE
                j["PipeList"] = json::array();
                for (ULONG i = 0; i < p->NumberOfOpenPipes; i++) {
                    j["PipeList"].push_back(p->PipeList[i]);
                }
                #endif
            }
        }
    };
    
    ADD_JSON_STRUCT(PUSB_DESCRIPTOR_REQUEST, VA_LIST(
        ADD_JSON_ENTRY(ConnectionIndex)
        ADD_JSON_ENTRY(SetupPacket.bmRequest)
        ADD_JSON_ENTRY(SetupPacket.bRequest)
        ADD_JSON_ENTRY(SetupPacket.wValue)
        ADD_JSON_ENTRY(SetupPacket.wIndex)
        ADD_JSON_ENTRY(SetupPacket.wLength) //,
        // ADD_JSON_ENTRY(Data)
    ),)
        
    template <>
    struct adl_serializer<USB_STRING_DESCRIPTOR> {
        static void to_json(json& j, const USB_STRING_DESCRIPTOR& p) {
            j = json{
                ADD_JSON_ENTRY_DIRECT_VERBOSE(bLength)
                ADD_JSON_ENTRY_DIRECT_VERBOSE(bDescriptorType)
            };
            j["bString"] = utf8_encode(std::wstring(p.bString));
        }
    };
    template<>
    struct adl_serializer<PSTRING_DESCRIPTOR_NODE> {
        static void to_json(json& j, const PSTRING_DESCRIPTOR_NODE& p) {
            PSTRING_DESCRIPTOR_NODE current = p;
            j = json::array();
            do {
                j.push_back({
                    {"DescriptorIndex", current->DescriptorIndex},
                    #ifdef SERIALIZE_VERBOSE
                    {"LanguageID", current->LanguageID},
                    #endif
                    {"StringDescriptor", current->StringDescriptor}
                });
                current = current->Next;
            } while (current);
        }
    };


    ADD_JSON_STRUCT_DIRECT(USB_PROTOCOLS, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(Usb110)
        ADD_JSON_ENTRY_DIRECT(Usb200)
        ADD_JSON_ENTRY_DIRECT(Usb300)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(ReservedMBZ)
    ),)


    ADD_JSON_STRUCT_DIRECT(USB_NODE_CONNECTION_INFORMATION_EX_V2_FLAGS, VA_LIST(
        ADD_JSON_ENTRY_DIRECT(DeviceIsOperatingAtSuperSpeedOrHigher)
        ADD_JSON_ENTRY_DIRECT(DeviceIsSuperSpeedCapableOrHigher)
        ADD_JSON_ENTRY_DIRECT(DeviceIsOperatingAtSuperSpeedPlusOrHigher)
        ADD_JSON_ENTRY_DIRECT(DeviceIsSuperSpeedPlusCapableOrHigher)
        ADD_JSON_ENTRY_DIRECT_VERBOSE(ReservedMBZ)
    ),)

    ADD_JSON_STRUCT(PUSB_NODE_CONNECTION_INFORMATION_EX_V2, VA_LIST(
        ADD_JSON_ENTRY(ConnectionIndex)
        ADD_JSON_ENTRY_VERBOSE(Length)
        ADD_JSON_ENTRY(SupportedUsbProtocols)
        ADD_JSON_ENTRY(Flags)
    ),)
    
    ADD_JSON_STRUCT(PUSBEXTERNALHUBINFO, VA_LIST(
            ADD_JSON_ENTRY(DeviceInfoType)
            ADD_JSON_ENTRY(HubPorts)
        ),
        VA_LIST(
            ADD_JSON_ENTRY_SAFE(HubInfo)
            ADD_JSON_ENTRY_SAFE(HubInfoEx)
            ADD_JSON_ENTRY_SAFE_WIDE(HubName)
            ADD_JSON_ENTRY_SAFE(PortConnectorProps)
            ADD_JSON_ENTRY_SAFE(UsbDeviceProperties)
            ADD_JSON_ENTRY_SAFE(DeviceInfoNode)
            ADD_JSON_ENTRY_SAFE_VERBOSE(HubCapabilityEx)
            ADD_JSON_ENTRY_SAFE(ConnectionInfo)
            ADD_JSON_ENTRY_SAFE_VERBOSE(ConfigDesc)
            ADD_JSON_ENTRY_SAFE_VERBOSE(BosDesc)
            ADD_JSON_ENTRY_SAFE(StringDescs)
            ADD_JSON_ENTRY_SAFE(ConnectionInfoV2)
        )
    )

    ADD_JSON_STRUCT(PUSBHOSTCONTROLLERINFO, VA_LIST(
            ADD_JSON_ENTRY(DeviceInfoType)
            ADD_JSON_ENTRY(VendorID)
            ADD_JSON_ENTRY(DeviceID)
            ADD_JSON_ENTRY(SubSysID)
            ADD_JSON_ENTRY(Revision)
            ADD_JSON_ENTRY(BusDeviceFunctionValid)
            ADD_JSON_ENTRY(BusNumber)
            ADD_JSON_ENTRY(BusDevice)
            ADD_JSON_ENTRY(BusFunction)
        ),
        VA_LIST(
            ADD_JSON_ENTRY_SAFE_WIDE(DriverKey)
            ADD_JSON_ENTRY_SAFE(ControllerInfo)
            ADD_JSON_ENTRY_SAFE(UsbDeviceProperties)
            ADD_JSON_ENTRY_SAFE(RootHub)
        )
    )

}