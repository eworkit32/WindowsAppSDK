#include "pch.h"
#include "AppNotificationContentBuilder.h"
#include "Microsoft.Windows.PushNotifications.AppNotificationContentBuilder.g.cpp"

using namespace winrt::Windows::Data::Xml::Dom;

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    hstring AppNotificationContentBuilder::GetXml()
    {
        std::wstring xmlPayload{ L"<toast>" };

        if (!m_text1.empty())
        {
            xmlPayload.append(m_text1);
        }

        if (!m_text2.empty())
        {
            xmlPayload.append(L";");
            xmlPayload.append(m_text2);
        }

        xmlPayload.append(L"</toast>");

        return hstring(xmlPayload);
    }

    void AppNotificationContentBuilder::AddText(hstring text)
    {
        if (m_text1.empty())
        {
            m_text1 = text;
        }
        else
        {
            m_text2 = text;
        }
    }

    winrt::Microsoft::Windows::PushNotifications::AppNotificationContentBuilder AppNotificationContentBuilder::AddTextFluent(hstring text)
    {
        AddText(text);

        return *this;
    }
}
