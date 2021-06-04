﻿#include "pch.h"
#include <testdef.h>
#include <iostream>
#include <sstream>
#include <wil/win32_helpers.h>
#include <winrt/Windows.ApplicationModel.Background.h> // we need this for BackgroundTask APIs
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace winrt::Microsoft::Windows::PushNotifications;
using namespace winrt;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Background; // BackgroundTask APIs

enum UnitTest {
    channelRequestUsingNullRemoteId, channelRequestUsingRemoteId, multipleChannelRequestUsingSameRemoteId,
    multipleChannelRequestUsingMultipleRemoteId, activatorTest, registerActivatorNullDetails,
    registerActivatorNullClsid, unregisterActivatorNullToken, unregisterActivatorNullBackgroundRegistration,
    multipleRegisterActivatorTest, multipleChannelClose
};

static std::map<std::string, UnitTest> switchMapping;
winrt::guid remoteId1(L"a2e4a323-b518-4799-9e80-0b37aeb0d225");
winrt::guid remoteId2(L"CA1A4AB2-AC1D-4EFC-A132-E5A191CA285A");
winrt::guid remoteId3(L"40FCE789-C6BF-4F47-A6CF-6B9C1DCE31BA");
PushNotificationRegistrationToken appToken = nullptr;
PushNotificationRegistrationToken fakeToken = nullptr;

void signalPhase(const std::wstring& phaseEventName)
{
    wil::unique_event phaseEvent;
    if (phaseEvent.try_open(phaseEventName.c_str(), EVENT_MODIFY_STATE, false))
    {
        phaseEvent.SetEvent();
    }
}

void initUnitTestMapping()
{
    switchMapping["ChannelRequestUsingNullRemoteId"] = UnitTest::channelRequestUsingNullRemoteId;
    switchMapping["ChannelRequestUsingRemoteId"] = UnitTest::channelRequestUsingRemoteId;
    switchMapping["MultipleChannelClose"] = UnitTest::multipleChannelClose;
    switchMapping["MultipleChannelRequestUsingSameRemoteId"] = UnitTest::multipleChannelRequestUsingSameRemoteId;
    switchMapping["MultipleChannelRequestUsingMultipleRemoteId"] = UnitTest::multipleChannelRequestUsingMultipleRemoteId;
    switchMapping["ActivatorTest"] = UnitTest::activatorTest;
    switchMapping["RegisterActivatorNullDetails"] = UnitTest::registerActivatorNullDetails;
    switchMapping["RegisterActivatorNullClsid"] = UnitTest::registerActivatorNullClsid;
    switchMapping["UnregisterActivatorNullToken"] = UnitTest::unregisterActivatorNullToken;
    switchMapping["UnregisterActivatorNullBackgroundRegistration"] = UnitTest::unregisterActivatorNullBackgroundRegistration;
    switchMapping["MultipleRegisterActivatorTest"] = UnitTest::multipleRegisterActivatorTest;
}

bool ChannelRequestUsingNullRemoteId()
{
    winrt::hresult hr = S_OK;

    try
    {
        auto channelOperation = PushNotificationManager::CreateChannelAsync(winrt::guid()).get();
    }
    catch (...)
    {
        auto channelRequestException = hresult_error(to_hresult());
        hr = channelRequestException.code();
    }

    return (hr == E_INVALIDARG);
}

HRESULT ChannelRequestHelper(IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& channelOperation)
{
    if (channelOperation.wait_for(std::chrono::seconds(300)) != AsyncStatus::Completed)
    {
        channelOperation.Cancel();
        return ERROR_TIMEOUT; // timed out or failed
    }

    auto result = channelOperation.GetResults();
    auto status = result.Status();
    if (status != PushNotificationChannelStatus::CompletedSuccess)
    {
        return result.ExtendedError(); // did not produce a channel
    }

    result.Channel().Close();
    return S_OK;
}

bool ChannelRequestUsingRemoteId()
{
    auto channelOperation = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperationResult = ChannelRequestHelper(channelOperation);

    return channelOperationResult == S_OK;
}

bool MultipleChannelClose()
{
    auto channelOperation = PushNotificationManager::CreateChannelAsync(remoteId1);
    if (channelOperation.wait_for(std::chrono::seconds(300)) != AsyncStatus::Completed)
    {
        channelOperation.Cancel();
        return false; // timed out or failed
    }

    auto result = channelOperation.GetResults();
    auto status = result.Status();
    if (status != PushNotificationChannelStatus::CompletedSuccess)
    {
        return false; // did not produce a channel
    }

    result.Channel().Close();
    try
    {
        result.Channel().Close();
    }
    catch (...)
    {
        auto channelRequestException = hresult_error(to_hresult());
        return channelRequestException.code() == WPN_E_CHANNEL_CLOSED;
    }
    return false;
}

bool MultipleChannelRequestUsingSameRemoteId()
{

    auto channelOperation1 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperation2 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperationResult2 = ChannelRequestHelper(channelOperation2);
    auto channelOperationResult1 = ChannelRequestHelper(channelOperation1);

    return channelOperationResult2 == S_OK;
}

bool MultipleChannelRequestUsingMultipleRemoteId()
{
    auto channelOperation1 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperation2 = PushNotificationManager::CreateChannelAsync(remoteId2);
    auto channelOperationResult2 = ChannelRequestHelper(channelOperation2);
    auto channelOperationResult1 = ChannelRequestHelper(channelOperation1);

    return channelOperationResult2 == S_OK;
}

bool ActivatorTest()
{
    PushNotificationManager::UnregisterActivator(appToken, PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator);
    try
    {
        PushNotificationActivationInfo info(
            PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator,
            c_fakeComServerId);

        fakeToken = PushNotificationManager::RegisterActivator(info);
        if (!fakeToken.TaskRegistration())
        {
            return false;
        }

        PushNotificationManager::UnregisterActivator(fakeToken, PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool RegisterActivatorNullDetails()
{
    winrt::hresult hr = S_OK;
    try
    {
        PushNotificationManager::RegisterActivator(nullptr);
    }
    catch (...)
    {
        auto registerActivatorException = hresult_error(to_hresult());
        hr = registerActivatorException.code();
    }
    return hr == E_INVALIDARG;
}

bool RegisterActivatorNullClsid()
{
    winrt::hresult hr = S_OK;
    try
    {
        PushNotificationActivationInfo info(
            PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator,
            winrt::guid()); // same clsid as app manifest
        PushNotificationManager::RegisterActivator(info);
    }
    catch (...)
    {
        auto registerActivatorException = hresult_error(to_hresult());
        hr = registerActivatorException.code();
    }
    return hr == E_INVALIDARG;
}

bool UnregisterActivatorNullToken()
{
    winrt::hresult hr = S_OK;
    try
    {
        PushNotificationManager::UnregisterActivator(nullptr, PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator);
    }
    catch (...)
    {
        auto registerActivatorException = hresult_error(to_hresult());
        hr = registerActivatorException.code();
    }
    return hr == E_INVALIDARG;
}

bool UnregisterActivatorNullBackgroundRegistration()
{
    winrt::hresult hr = S_OK;
    try
    {
        PushNotificationRegistrationToken badToken{ 0, nullptr };
        PushNotificationManager::UnregisterActivator(badToken, PushNotificationRegistrationOption::PushTrigger);
    }
    catch (...)
    {
        auto registerActivatorException = hresult_error(to_hresult());
        hr = registerActivatorException.code();
    }
    return hr == HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
}

bool MultipleRegisterActivatorTest()
{
    winrt::hresult hr = S_OK;
    try
    {
        PushNotificationActivationInfo info(
            PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator,
            c_fakeComServerId); // same clsid as app manifest

        appToken = PushNotificationManager::RegisterActivator(info);
    }
    catch (...)
    {
        auto registerActivatorException = hresult_error(to_hresult());
        hr = registerActivatorException.code();
    }
    return hr == E_INVALIDARG;
}

bool runUnitTest(std::string unitTest)
{
    switch (switchMapping[unitTest])
    {
    case UnitTest::channelRequestUsingNullRemoteId:
        return ChannelRequestUsingNullRemoteId();

    case UnitTest::channelRequestUsingRemoteId:
        return ChannelRequestUsingRemoteId();

    case UnitTest::multipleChannelClose:
        return MultipleChannelClose();

    case UnitTest::multipleChannelRequestUsingSameRemoteId:
        return MultipleChannelRequestUsingSameRemoteId();

    case UnitTest::multipleChannelRequestUsingMultipleRemoteId:
        return MultipleChannelRequestUsingMultipleRemoteId();

    case UnitTest::activatorTest:
        return ActivatorTest();

    case UnitTest::registerActivatorNullDetails:
        return RegisterActivatorNullDetails();

    case UnitTest::registerActivatorNullClsid:
        return RegisterActivatorNullClsid();

    case UnitTest::unregisterActivatorNullToken:
        return UnregisterActivatorNullToken();

    case UnitTest::unregisterActivatorNullBackgroundRegistration:
        return UnregisterActivatorNullBackgroundRegistration();

    case UnitTest::multipleRegisterActivatorTest:
        return MultipleRegisterActivatorTest();

    default:
        return false;
    }
}

// Cleanup function for exiting test app.
void UnregisterClsid()
{
    if (appToken)
    {
        ::CoRevokeClassObject(static_cast<DWORD>(appToken.Cookie()));
        PushNotificationManager::UnregisterActivator(appToken, PushNotificationRegistrationOption::PushTrigger);
    }

    if (fakeToken)
    {
        ::CoRevokeClassObject(static_cast<DWORD>(fakeToken.Cookie()));
        PushNotificationManager::UnregisterActivator(fakeToken, PushNotificationRegistrationOption::PushTrigger);
    }

}

int main()
{
    initUnitTestMapping();
    bool testResult = false;
    auto scope_exit = wil::scope_exit([&] {
        UnregisterClsid();
    });

    PushNotificationActivationInfo info(
        PushNotificationRegistrationOption::PushTrigger | PushNotificationRegistrationOption::ComActivator,
        winrt::guid(c_comServerId)); // same clsid as app manifest

    appToken = PushNotificationManager::RegisterActivator(info);
    
    auto args = AppInstance::GetCurrent().GetActivatedEventArgs();
    auto kind = args.Kind();

    if (kind == ExtendedActivationKind::Launch)
    {
        auto launchArgs = args.Data().as<ILaunchActivatedEventArgs>();
        std::string unitTest = to_string(launchArgs.Arguments());
        std::cout << unitTest << std::endl;

        testResult = runUnitTest(unitTest);
    }
    else if (kind == ExtendedActivationKind::Push)
    {
        PushNotificationReceivedEventArgs pushArgs = args.Data().as<PushNotificationReceivedEventArgs>();
        auto payload = pushArgs.Payload();
        std::wstring payloadString(payload.begin(), payload.end());

        testResult = !payloadString.compare(c_rawNotificationPayload);
    }

    return testResult ? 0 : 1; // We want 0 to be success and 1 failure
};