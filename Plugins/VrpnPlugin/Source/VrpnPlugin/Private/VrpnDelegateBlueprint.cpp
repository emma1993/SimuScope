/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"
#include "VrpnDelegateBlueprint.h"

//Events

//mouse not used currently
void VrpnDelegateBlueprint::VrpnOnMousePosition(uint64 timestamp, FVector2D mousePosition)
{
	if (IsValidDelegate())
		//IVrpnInterface::Execute_OnMousePosition(_interfaceDelegate, mousePosition);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(mousePosition.X));
}

void VrpnDelegateBlueprint::VrpnOnTrackerData(uint64 timestamp, uint32 sensor, FVector pos, FRotator rot)
{
	if (IsValidDelegate())
		IVrpnInterface::Execute_OnVrpnTrackerData(_interfaceDelegate, (int32)sensor, pos, rot);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Sensor: " + FString::FromInt((int32)sensor) + " X: " + FString::SanitizeFloat(pos.X) + " Y: " + FString::SanitizeFloat(pos.Y) + " Z: " + FString::SanitizeFloat(pos.Z));
}

void VrpnDelegateBlueprint::VrpnDisabled()
{
	IVrpnInterface::Execute_DeviceDisabled(_interfaceDelegate);
}


//Blueprint functions forward
bool VrpnDelegateBlueprint::VrpnIsRemoteEnabled()
{
	return VrpnDelegate::VrpnIsRemoteEnabled();
}

void VrpnDelegateBlueprint::VrpnStartup(FString vrpnIPAddress, FString vrpnTrackerName)
{
	VrpnDelegate::VrpnStartup(vrpnIPAddress, vrpnTrackerName);

	UObject* validUObject = NULL;
	validUObject = Cast<UObject>(ValidSelfPointer);

	//Set self as interface delegate by default
	if (!_interfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
}

void VrpnDelegateBlueprint::VrpnShutdown()
{
	VrpnDelegate::VrpnShutdown();
}

void VrpnDelegateBlueprint::VrpnTick(float DeltaTime)
{
	VrpnDelegate::VrpnTick(DeltaTime);
}

bool VrpnDelegateBlueprint::IsValidDelegate()
{
	return (_interfaceDelegate != NULL);
}

void VrpnDelegateBlueprint::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());

	//Use this format to support both blueprint and C++ form
	if (newDelegate->GetClass()->ImplementsInterface(UVrpnInterface::StaticClass()))
	{
		_interfaceDelegate = newDelegate;
	}
	else
	{
		//Try casting as self
		if (ValidSelfPointer->GetClass()->ImplementsInterface(UVrpnInterface::StaticClass()))
		{
			_interfaceDelegate = (UObject*)this;
		}
		else
		{
			//If you're crashing its probably because of this setting causing an assert failure
			_interfaceDelegate = NULL;
		}

		//Either way post a warning, this will be a common error
		UE_LOG(LogClass, Log, TEXT("VrpnDelegateBlueprint Warning: Delegate is NOT set, did your class implement VrpnInterface?"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("VrpnDelegateBlueprint Warning: Delegate is NOT set, did your class implement VrpnInterface?"));
	}
}
