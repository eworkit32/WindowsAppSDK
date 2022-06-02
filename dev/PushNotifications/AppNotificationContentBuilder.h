#pragma once
#include "Microsoft.Windows.PushNotifications.AppNotificationContentBuilder.g.h"

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    struct AppNotificationContentBuilder : AppNotificationContentBuilderT<AppNotificationContentBuilder>
    {
        AppNotificationContentBuilder() = default;

        hstring GetXml();

        void AddText(hstring text);
        winrt::Microsoft::Windows::PushNotifications::AppNotificationContentBuilder AddTextFluent(hstring text);

    private:
        std::wstring m_text1{};
        std::wstring m_text2{};
    };
}
namespace winrt::Microsoft::Windows::PushNotifications::factory_implementation
{
    struct AppNotificationContentBuilder : AppNotificationContentBuilderT<AppNotificationContentBuilder, implementation::AppNotificationContentBuilder>
    {
    };
}
