# AppNotificationContentBuilder
The C# Development Community has a [Toast Content Builder](https://docs.microsoft.com/en-us/dotnet/api/microsoft.toolkit.uwp.notifications?view=win-comm-toolkit-dotnet-7.0), which serves as a reference for this project.
## MVP
WindowsAppSDK v1.2 Development Schedule
Milestone | Date
---|---
Scenarios Filled in ADO | **May 16, 2022**
Code Start | **June 10, 2022**
Code Complete | **August 8, 2022**

### Goal 
- 
### Non-Goals
- Provide a solution that can build everything.
  - Since developers can always retrive the content from the builder in XML, it's not necessary the builder supports every possible combinations.
### Class Constructor & GetXml
Method | Description
---|---
ToastContentBuilder() |	Initializes a new instance of the ToastContentBuilder class.
GetXml() | Retrieves the notification XML content as a WinRT XmlDocument, so that it can be used with a local AppNotification's constructor.

### Inputs
Method | Description
---|---
AddButton(String, String) | Add a button to the current toast.
AddTextBoxWithButton(String, String, ToastActivationType, String) | Add a button to the toast that will be display to the right of the input text box, achieving a quick reply scenario.

### Activation Arguments
Method | Description
---|---
AddArgument(String, String)	| Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.

### Toast Notification with Avatar Image
We want devs to write something like this to post a toast:

``` c++
auto contentBuilder{ new AppNotificationContentBuilder()
    .AddImage(L"Path\\To\\MyLittleUnicorn.png", Placement::appLogoOverride, Crop::Circle)
    .AddText(L"Toast Notification with Avatar Image")
    .AddText(L"This is an example message using content builder")
    .AddButton(L"Open App", L"OpenAppAction")
    .AddArgument(L"Sequnce", L"1234") };

auto appNotification{ new AppNotification(contentBuilder.GetXml()) };
AppNotificationManager.Default.Show(toast);
```

Which would produce the equivalent to:

``` xml
<toast>
    <visual>
        <binding template="ToastGeneric">
            <image placement="appLogoOverride" hint-crop="circle" src="Path\To\MyLittleUnicorn.png"/>
            <text>Toast Notification with Avatar Image</text>
            <text>This is an example message using content builder</text>
        </binding>
    </visual>
    <actions>
        <action
            content="Open App"
            arguments="action="OpenAppAction""&amp;"Sequnce="1234""/>
    </actions>
</toast>
```

The proposed solution also includes a `Show` method that can replace
``` c++
auto appNotification{ new AppNotification(contentBuilder.GetXml()) };
AppNotificationManager.Default.Show(toast);
```
and let the caller construct and present the AppNotification in a single operation. 

``` c++
AppNotificationContentBuilder()
    .AddImage(L"Path\\To\\MyLittleUnicorn.png", Placement::appLogoOverride, Crop::Circle)
    .AddText(L"Toast Notification with Avatar Image")
    .AddText(L"This is an example message using content builder")
    .AddButton(L"Open App", L"OpenAppAction")
    .AddArgument(L"Sequnce", L"1234")
    .Show();
```
The `Show` methods is a convenience function (defined later in this document) and as wouldn't normaly be part of the MVP but, given how trivial the implementation is, could easily be provided with the first release.
### Toast Notification with Input Text Box

We want devs to write something like this to post a toast with an input text box and associated button:

``` c++
auto contentBuilder{ new AppNotificationContentBuilder()
    .AddImage(L"Path\\To\\MyLittleUnicorn.png", Placement::appLogoOverride, Crop::Circle)
    .AddText(L"Toast Notification with Avatar Image")
    .AddText(L"This is an example message using content builder")
    .AddTextBoxWithButton(L"Reply", L"ReplyAction", L"Type a reply")
    .AddArgument(L"ConversationId", L"1234") };

auto toast{ new AppNotification(contentBuilder.GetXml()) };
AppNotificationManager.Default.Show(toast);
```

Which would produce the equivalent to:

``` xml
<toast>
    <visual>
        <binding template="ToastGeneric">
            <image placement="appLogoOverride" hint-crop="circle" src="Path\To\MyLittleUnicorn.png"/>
            <text>Toast Notification with Input Text Box</text>
            <text>This is an example message using content builder</text>
        </binding>
    </visual>
    <actions>
        <input
            id="ReplyAction"
            type="text"
            placeHolderContent="Type a reply"/>
            <action
                content="Reply"
                arguments="action="ReplyAction""&amp;"ConversationId="1234""
                hint-inputId="ReplyAction"/>
    </actions>
</toast>
```
### Cost Estimates
All estimates are in days.

Task | Estimate | Comments
---|---:|---
API Design | **13** | Get API for the MVP Scenarios reviewed and approved
Implementation | **13** | Implement features selected for WindowsAppSDK v1.2.
Samples | **3** | Update exisiting C++ and C# samples to use the new toast builder. Use C# sample to verify that projections are working as expected.
Code Review | **3** | Final code review of the feature branch once implementation is complete, before the feature is merged into develop.

#### API Design
Task breakdown for the API design task from above.

Task | Estimate | Comments
---|---:|---
Full North Star API Proposal | **5** | Write the full API description supporting features proposed for v1.2 as well as features for later, including features not yet supported in the WIndowsAppSDK, like Scheduling.
Review and Discussion | **3** | Open a PR aget get reviewert feedback, drive discussion as necessary.
Apply Feedback | **2** | Update the API according to feeedback from the code review.

#### Implementation
Task breakdown for the Implementation task from above.

Task | Estimate | Comments
---|---:|---
Infrastructure | **2** | Create a feature branch; create files for class (IDL, cpp, etc) and tests, then add to vcproj, confirms everything builds. 
Constructor / GetXml | **1** | Setup internal structures that will support the builder.
Text | **1** | Add support for text.
Image Support | **2** | Add support for images, including placement and crop options.
Button | **2** | Add support for a button along with putting the ButtonID into the arguments.
Text Box with Button | **2** | Add support for a text box that accept text input along with a button tied to the textbox.
Custom Argument | **1** | Add support for a custom argument to be passed back to the app when activated.

## North Star
Post v1.2 suggested development.
### Audio
Let devs add audio to their toasts.

Method | Description
---|---
AddAudio(ToastAudio) | Set custom audio to go along with the toast.
AddAudio(Uri, Nullable<Boolean>, Nullable<Boolean>) | Set custom audio to go along with the toast.

### Inputs
Functions that let devs configure various inputs that users can use to enter data directly into the toast.
That user data will be make available to the app during activation.

Method | Description
---|---
AddComboBox(String, String, String, IEnumerable<ValueTuple<String,String>>)| Add a combo box / drop-down menu that contain options for user to select.
AddComboBox(String, String, String, ValueTuple<String,String>[]) | Add a combo box / drop-down menu that contain options for user to select.
AddComboBox(String, String, ValueTuple<String,String>[]) | Add a combo box / drop-down menu that contain options for user to select.
AddComboBox(String, ValueTuple<String,String>[]) | Add a combo box / drop-down menu that contain options for user to select.
AddCustomTimeStamp(DateTime) | Add custom time stamp on the toast to override the time display on the toast.
AddInputTextBox(String, String, String) | Add an input text box that the user can type into.
AddToastInput(IToastInput) | Add an input option to the Toast.

### Arguments
Funtions that let devs add arguments to the toasts.
Arguments are pass back to the app during activation.

Method | Description
---|---
AddArgument(String) | Adds a key (without value) to the activation arguments that will be returned when the toast notification or its buttons are clicked.
AddArgument(String, Boolean) | Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.
AddArgument(String, Double) | Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.
AddArgument(String, Enum) | Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.
AddArgument(String, Int32) | Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.
AddArgument(String, Single) | Adds a key/value to the activation arguments that will be returned when the toast notification or its buttons are clicked.


### Visuals
Advanced functions that let developers configure the look of their toast.

Method | Description
---|---
AddAttributionText(String) | Add an Attribution Text to be displayed on the toast.
AddAttributionText(String, String) | Add an Attribution Text to be displayed on the toast.
AddAppLogoOverride(Uri, Nullable<ToastGenericAppLogoCrop>, String, Nullable<Boolean>) | Override the app logo with custom image of choice that will be displayed on the toast.
AddHeader(String, String, String) | Add a header to a toast.
AddHeader(String, String, ToastArguments) | Add a header to a toast.
AddHeroImage(Uri, String, Nullable<Boolean>) | Add a hero image to the toast.
AddInlineImage(Uri, String, Nullable<Boolean>, Nullable<AdaptiveImageCrop>, Nullable<Boolean>) | Add an image inline with other toast content.
AddText(String, Nullable<AdaptiveTextStyle>, Nullable<Boolean>, Nullable<Int32>, Nullable<Int32>, Nullable<AdaptiveTextAlign>, String) | Add text to the toast.
AddVisualChild(IToastBindingGenericChild) | Add a visual element to the toast.

### Live Updates
Let developpers create and update toasts with progress bars.

Method | Description
---|---
AddProgressBar(String, Nullable<Double>, Boolean, String, String) | Add a progress bar to the toast.
CreateProgressBarData(ToastContent, Int32, String, Nullable<Double>, String, String, UInt32) | Create an instance of NotificationData that can be used to update toast that has a progress bar.

### Toast Scheduling
Scheduling functions that allow devs to post Scheduled Toast. 
The scheduling isn't supported at this time (WindopwsAppSDK v1.1)

Method | Description
---|---
Schedule(DateTimeOffset) | Schedules the notification.
Schedule(DateTimeOffset, CustomizeScheduledToast) | Schedules the notification.
Schedule(DateTimeOffset, CustomizeScheduledToastAsync) | Schedules the notification.
SetToastDuration(ToastDuration) | Sets the amount of time the Toast should display. You typically should use the Scenario attribute instead, which impacts how long a Toast stays on screen.
SetToastScenario(ToastScenario) | Sets the scenario, to make the Toast behave like an alarm, reminder, or more.

### Toast Show
Convenience functions that allow devs to post toasts without explicitely creationg an AppNotification object.

Method | Description
---|---
Show() | Shows a new toast notification with the current content.
