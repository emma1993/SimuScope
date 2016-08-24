/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
//#include "VrpnDelegate.generated.h"

/** Inherit override and set delegate to subscribe to callbacks*/

DECLARE_LOG_CATEGORY_EXTERN(VrpnPluginLog, Log, All);

//Input Mapping Key Structure
struct EKeysVrpn
{
	//Axis, given in arm orientation by default (use calibration or use raw functions to specify)
	static const FKey VrpnMouseX;
	static const FKey VrpnMouseY;

	static const FKey VrpnTrackerX;
	static const FKey VrpnTrackerY;
	static const FKey VrpnTrackerZ;
};

class VrpnDelegate
{
public:
	/** Events */
	virtual void VrpnOnMousePosition(uint64 timestamp, FVector2D mousePosition);
	virtual void VrpnOnTrackerData(uint64 timestamp, uint32 sensor, FVector pos, FRotator rot);

	/*Vrpn being disabled*/
	virtual void VrpnDisabled();	

	/** Callable Functions */
	virtual bool VrpnIsRemoteEnabled();

	//Required Functions
	virtual void VrpnStartup(FString vrpnIPAddress, FString vrpnTrackerName);
	virtual void VrpnShutdown();
	virtual void VrpnTick(float DeltaTime);

	FString getServerIP();
	void setServerIP(FString ip);

	FString getTrackerName();
	void setTrackerName(FString tracker);

private:
	FString mVrpnServerIP;
	FString mVrpnTrackerName;
};