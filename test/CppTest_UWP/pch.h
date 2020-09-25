﻿#pragma once
#include <windows.h>
#include <unknwn.h>
#include <wil/cppwinrt.h>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.VisualStudio.TestPlatform.TestExecutor.WinRTCore.h>
#include <CppUnitTest.h>

// Test-specific headers, lifted here
#include <SampleFlatC.h>
#include <winrt/Microsoft.ProjectReunion.h>
#include <winrt/Microsoft.ProjectReunion.Deployment.h>

