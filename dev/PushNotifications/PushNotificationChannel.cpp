﻿#include "pch.h"

#include "PushNotificationChannel.h"
#include "PushNotificationChannel.g.cpp"
#include <winrt\Windows.Networking.PushNotifications.h>

namespace winrt::Microsoft::ProjectReunion::implementation
{
    PushNotificationChannel::PushNotificationChannel(Windows::Networking::PushNotifications::PushNotificationChannel const& channel)
    {
        m_channel = channel;
    }
    hstring PushNotificationChannel::Uri()
    {
        return m_channel.Uri();
    }
    Windows::Foundation::DateTime PushNotificationChannel::ExpirationTime()
    {
        return m_channel.ExpirationTime();
    }
    void PushNotificationChannel::Close()
    {
        m_channel.Close();
    }

    winrt::event_token PushNotificationChannel::PushReceived(Windows::Foundation::TypedEventHandler<Microsoft::ProjectReunion::PushNotificationChannel, Microsoft::ProjectReunion::PushNotificationReceivedEventArgs> const& handler)
    {
        throw hresult_not_implemented();
    }
    void PushNotificationChannel::PushReceived(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}