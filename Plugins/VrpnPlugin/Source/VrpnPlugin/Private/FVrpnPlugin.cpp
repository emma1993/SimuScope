/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"

#include "AllowWindowsPlatformTypes.h" 

#include "vrpn/vrpn_Connection.h"
#include "vrpn/vrpn_Mouse.h"
#include "vrpn/vrpn_Shared.h"
#include "vrpn/vrpn_Tracker.h"

#include "HideWindowsPlatformTypes.h"

#include "IVrpnPlugin.h"
#include "FVrpnPlugin.h"
#include "VrpnDelegate.h"
#include "VrpnDelegateBlueprint.h"
#include "SlateBasics.h"

#include <iostream>
#include <stdexcept>
#include <vector>

IMPLEMENT_MODULE(FVrpnPlugin, VrpnPlugin)

#define LOCTEXT_NAMESPACE "VrpnPlugin"

#define PLUGIN_VERSION "0.2.0"

//Private API - This is where the magic happens

//UE v4.6 IM event wrappers
bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int32 user)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, false, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

//String Conversion to UE4
TCHAR* tcharFromStdString(std::string str){
	TCHAR *param = new TCHAR[str.size() + 1];
	param[str.size()] = 0;
	std::copy(str.begin(), str.end(), param);
	return param;
}
FRotator convertOrientationToUE(FRotator rawOrientation)
{
	FRotator convertedOrientation;
	convertedOrientation.Pitch = rawOrientation.Pitch*-1.f;
	convertedOrientation.Yaw = rawOrientation.Yaw*-1.f;
	convertedOrientation.Roll = rawOrientation.Roll;
	return convertedOrientation;

	//debug return raw orientation
	//return rawOrientation;
}

//Same as blueprint function, internal copy
FRotator combineRotators(FRotator A, FRotator B)
{
	FQuat AQuat = FQuat(A);
	FQuat BQuat = FQuat(B);

	return FRotator(BQuat*AQuat);
}

FVector convertVectorToUE(FVector rawAcceleration)
{
	return FVector(rawAcceleration.X, -rawAcceleration.Y, -rawAcceleration.Z);
}


//DataCollector used to register on vrpn remotes
class DataCollector {
public:
	DataCollector(){
		vrpnDelegate = NULL;
		vrpn_remote = NULL;
		Enabled = false;
		Listening = false;
	}
	~DataCollector(){
		ShutDown();
	}

	void StartListening(){
		if (!Listening){
			vrpn_remote->register_change_handler(this, handle_tracker);
			Listening = true;
		}
	}
	void StopListening(){
		if (Listening){
			vrpn_remote->unregister_change_handler(this, handle_tracker);
			Listening = false;
		}
	}

	//The plugin may live longer than the playing session, run() causes a backlog of events to stream, to fix this we will close it in between sessions
	bool Startup()
	{
		if (vrpn_remote == NULL)
		{
			//Build vrpn connection string
			FString connectionString;
			if (vrpnDelegate->getTrackerName().Len() > 0){
				connectionString = vrpnDelegate->getTrackerName();
			}
			else {
				connectionString = "Tracker0";
				UE_LOG(VrpnPluginLog, Log, TEXT("Set VRPN tracker name to DEFAULT: Tracker0. Change corresponding component setting to specify another one."));
			}
			connectionString.Append("@");
			if (vrpnDelegate->getServerIP().Len() > 0){
				connectionString.Append(vrpnDelegate->getServerIP());
			}
			else {
				connectionString.Append("127.0.0.1");
				UE_LOG(VrpnPluginLog, Log, TEXT("Set VRPN IP to DEFAULT: 127.0.0.1. Change corresponding component setting to specify another one."));
			}
			
			vrpn_remote = new vrpn_Tracker_Remote(TCHAR_TO_UTF8(*connectionString));
			UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Remote Initialized: %s"), *connectionString);
		}
		if (vrpn_remote) {
			UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Remote Connection established"));
			StartListening();
			Enabled = true;
			return true;
		}
		else{
			UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Remote Connection failed"));
			return false;
		}
	}

	void ShutDown()
	{
		StopListening();

		if (vrpn_remote != NULL){
			delete vrpn_remote;
			vrpn_remote = NULL;

			//Shutdown message
			UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Remote Shutdown."));
		}
	}

	void ResetHub()
	{
		ShutDown();
		Startup();
	}

	VrpnDelegate* vrpnDelegate;
	bool Enabled;
	bool Listening;

	vrpn_Tracker_Remote *vrpn_remote;
};

// Callbacks called by vrpn server

// analog callback: currently not used, but could be
void VRPN_CALLBACK handle_analog(void *userdata, const vrpn_ANALOGCB a)
{
	UE_LOG(LogClass, Log, TEXT("Analog:\n         %5.2f - %5.2f"), a.channel[0], a.channel[1]);

	DataCollector* worker = reinterpret_cast<DataCollector*>(userdata);
	if (worker != NULL && worker->vrpnDelegate != NULL){
		float mouseX = a.channel[0] * 2.0 - 1.0;
		float mouseY = a.channel[1] * 2.0 - 1.0;
		worker->vrpnDelegate->VrpnOnMousePosition(1, FVector2D(mouseX, mouseY));
		EmitAnalogInputEventForKey(EKeysVrpn::VrpnMouseX, mouseX, 0);
		EmitAnalogInputEventForKey(EKeysVrpn::VrpnMouseY, mouseY, 0);
	}
}

// tracker position callback
void VRPN_CALLBACK handle_tracker(void *userdata, const vrpn_TRACKERCB t)
{	
	// WorldViz coordinate mapping
	float x = t.pos[0]; // orthogonal zu leinwand
	float y = t.pos[1]; // height
	float z = t.pos[2]; // parallel zu leinwand
	
	UE_LOG(LogClass, Log, TEXT("Tracker %d Pos:\n         %5.2f - %5.2f - %5.2f"), t.sensor, x, y, z);
	UE_LOG(LogClass, Log, TEXT("Tracker %d Quat:\n         %5.2f - %5.2f - %5.2f - %5.2f"), t.sensor, t.quat[0], t.quat[1], t.quat[2], t.quat[3]);

	DataCollector* worker = reinterpret_cast<DataCollector*>(userdata);
	if (worker != NULL && worker->vrpnDelegate != NULL){
		worker->vrpnDelegate->VrpnOnTrackerData(1, (uint32)t.sensor, FVector(x, z, y), convertOrientationToUE(FRotator(FQuat(t.quat[2], -t.quat[0], -t.quat[1], t.quat[3])))); //remap coordinates to map unreal system
		EmitAnalogInputEventForKey(EKeysVrpn::VrpnTrackerX, x, 0);
		EmitAnalogInputEventForKey(EKeysVrpn::VrpnTrackerY, y, 0);
		EmitAnalogInputEventForKey(EKeysVrpn::VrpnTrackerZ, z, 0);
	}
}

// currently not used, only supports mouse (analog), maybe use this class for a more generic approach
class device_info {
public:
	char *name;
	//vrpn_Tracker_Remote *tkr;
	//vrpn_Button_Remote *btn;
	vrpn_Analog_Remote *ana;
	//vrpn_Dial_Remote *dial;
	//vrpn_Text_Receiver *text;
};

//Init and Runtime
void FVrpnPlugin::StartupModule()
{
	UE_LOG(VrpnPluginLog, Log, TEXT("Using VRPN Plugin version %s"), TEXT(PLUGIN_VERSION));

	// Instantiate the DataCollector
	collector = new DataCollector;

	//Register all input mapping keys and axes
	EKeys::AddKey(FKeyDetails(EKeysVrpn::VrpnMouseX, LOCTEXT("VrpnMouseX", "Vrpn Mouse X"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EKeysVrpn::VrpnMouseY, LOCTEXT("VrpnMouseY", "Vrpn Mouse Y"), FKeyDetails::FloatAxis));

	EKeys::AddKey(FKeyDetails(EKeysVrpn::VrpnTrackerX, LOCTEXT("VrpnTrackerX", "Vrpn Tracker X"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EKeysVrpn::VrpnTrackerY, LOCTEXT("VrpnTrackerY", "Vrpn Tracker Y"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EKeysVrpn::VrpnTrackerZ, LOCTEXT("VrpnTrackerZ", "Vrpn Tracker Z"), FKeyDetails::FloatAxis));
}

void FVrpnPlugin::ShutdownModule()
{
	collector->vrpnDelegate = NULL;
	
	collector->ShutDown();
	delete collector;
}


//Public API Implementation

bool FVrpnPlugin::IsRemoteEnabled()
{
	return collector->Enabled;
}

void FVrpnPlugin::SetDelegate(VrpnDelegate* newDelegate){ 
	collector->vrpnDelegate = newDelegate;
	collector->Startup();

	//Emit disabled event if we didn't manage to create the vrpn_remote
	if (!collector->Enabled){
		collector->vrpnDelegate->VrpnDisabled();
		UE_LOG(VrpnPluginLog, Warning, TEXT("VRPN collector is Disabled."));
	}

	UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Delegate Set (should only be called once per begin play or you have duplicates)."));
}

void FVrpnPlugin::RemoveDelegate()
{
	collector->vrpnDelegate = NULL;
	collector->ShutDown();	//we only allow one delegate to be active so remove listening to stop overload of streaming
}

void FVrpnPlugin::VrpnTick(float DeltaTime)
{
	/**
	* We're trying to emulate a single tick;
	* This will add roughly 1-2ms to render loop time
	* While this is not ideal and a separate loop would be an improvement, 1ms is an 
	* acceptable frame lag for simplicity.
	*/
	if (collector->Listening){
		if (collector->Enabled){
			collector->vrpn_remote->mainloop();
			//UE_LOG(VrpnPluginLog, Log, TEXT("VRPN Tick. %1.4f"), DeltaTime);
		}
	}
}

#undef LOCTEXT_NAMESPACE