#pragma once
#include "Microsoft.Windows.PushNotifications.AppNotificationContentBuilder.g.h"

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    struct AppNotificationContentBuilder : AppNotificationContentBuilderT<AppNotificationContentBuilder>
    {
        AppNotificationContentBuilder() = default;

        hstring GetXml();

        void AddText(hstring text);

    private:
        hstring m_text{};
    };
}
namespace winrt::Microsoft::Windows::PushNotifications::factory_implementation
{
    struct AppNotificationContentBuilder : AppNotificationContentBuilderT<AppNotificationContentBuilder, implementation::AppNotificationContentBuilder>
    {
    };
}
