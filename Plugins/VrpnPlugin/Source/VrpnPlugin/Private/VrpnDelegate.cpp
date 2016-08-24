/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"

#include "IVrpnPlugin.h"
#include "VrpnDelegate.h"

DEFINE_LOG_CATEGORY(VrpnPluginLog);

//Input Mapping EKey definitions
const FKey EKeysVrpn::VrpnMouseX("VrpnMouseX");
const FKey EKeysVrpn::VrpnMouseY("VrpnMouseY");

const FKey EKeysVrpn::VrpnTrackerX("VrpnTrackerX");
const FKey EKeysVrpn::VrpnTrackerY("VrpnTrackerY");
const FKey EKeysVrpn::VrpnTrackerZ("VrpnTrackerZ");

//Empty Implementations
void VrpnDelegate::VrpnOnMousePosition(uint64 timestamp, FVector2D mousePosition){}
void VrpnDelegate::VrpnOnTrackerData(uint64 timestamp, uint32 sensor, FVector pos, FRotator rot){}
void VrpnDelegate::VrpnDisabled(){}


bool VrpnDelegate::VrpnIsRemoteEnabled()
{
	if (IVrpnPlugin::IsAvailable())
	{
		return IVrpnPlugin::Get().IsRemoteEnabled();
	}
	else{
		return false;
	}
}

void VrpnDelegate::VrpnTick(float DeltaTime)
{
	if (IVrpnPlugin::IsAvailable())
	{
		IVrpnPlugin::Get().VrpnTick(DeltaTime);
	}
}
void VrpnDelegate::VrpnStartup(FString vrpnIPAddress, FString vrpnTrackerName)
{
	mVrpnServerIP = vrpnIPAddress; //set address used in FVrpnPlugin Startup
	mVrpnTrackerName = vrpnTrackerName; //set tracker name used in FVrpnPlugin Startup
	
	if (IVrpnPlugin::IsAvailable())
	{
		IVrpnPlugin::Get().SetDelegate((VrpnDelegate*)this);
	}
}

void VrpnDelegate::VrpnShutdown()
{
	if (IVrpnPlugin::IsAvailable())
	{
		IVrpnPlugin::Get().RemoveDelegate();
	}
}

FString VrpnDelegate::getServerIP(){
	return mVrpnServerIP;
}

void VrpnDelegate::setServerIP(FString ip){
	mVrpnServerIP = ip;
}

FString VrpnDelegate::getTrackerName(){
	return mVrpnTrackerName;
}

void VrpnDelegate::setTrackerName(FString name){
	mVrpnTrackerName = name;
}