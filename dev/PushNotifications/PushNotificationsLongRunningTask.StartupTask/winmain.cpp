﻿#include <windows.h>
#include "pch.h"

#include "../PushNotifications-Constants.h"

#include <PushNotificationsLRP_h.h>
#include <wil/resource.h>

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
    RETURN_IF_FAILED(::CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));

    {
        wil::com_ptr<IWpnLrpPlatform> reunionEndpoint{
            wil::CoCreateInstance<WpnLrpPlatform, IWpnLrpPlatform>(CLSCTX_LOCAL_SERVER) };
    }

    ::CoUninitialize();

    return 0;
}