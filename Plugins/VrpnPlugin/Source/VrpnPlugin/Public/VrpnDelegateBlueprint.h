/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
#include "VrpnDelegate.h"
#include "VrpnInterface.h"

//Declares Blueprint event calls, used for the Vrpn main component, other functions may be called from the VrpnController

class VrpnDelegateBlueprint : public VrpnDelegate
{
public:

	//Component Functions - override these in your classes to expose to blueprint, need different naming however (e.g. drop Vrpn prefix)
	virtual bool VrpnIsRemoteEnabled() override;

	//Required functions to make plugin work
	virtual void VrpnStartup(FString vrpnIPAddress, FString vrpnTrackerName) override;
	virtual void VrpnShutdown() override;
	virtual void VrpnTick(float DeltaTime) override;

	//If you want an alternate delegate, set it here
	void SetInterfaceDelegate(UObject* newDelegate);
protected:
	bool IsValidDelegate();
	UObject* ValidSelfPointer;	//REQUIRED: has to be set before VrpnStartup by a UObject subclass.
private:
	//Delegate function override to pass-through and convert parameters to blueprint
	virtual void VrpnOnMousePosition(uint64 timestamp, FVector2D mousePosition) override;
	virtual void VrpnOnTrackerData(uint64 timestamp, uint32 sensor, FVector pos, FRotator rot) override;

	virtual void VrpnDisabled() override;

	UObject* _interfaceDelegate;
};