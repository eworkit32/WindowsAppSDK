#include "pch.h"
#include "AppNotificationContentBuilder.h"
#include "Microsoft.Windows.PushNotifications.AppNotificationContentBuilder.g.cpp"

using namespace winrt::Windows::Data::Xml::Dom;

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    hstring AppNotificationContentBuilder::GetXml()
    {
        winrt::hstring xmlPayload{ L"<toast>" + m_text + L"</toast>" };
        return xmlPayload;
    }

    void AppNotificationContentBuilder::AddText(hstring text)
    {
        m_text = text;
    }
}
