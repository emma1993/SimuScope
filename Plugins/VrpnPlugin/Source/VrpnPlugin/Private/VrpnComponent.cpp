/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"
#include "VrpnComponent.h"
#include "VrpnInterface.h"
#include "Engine.h"
#include "CoreUObject.h"

UVrpnComponent::UVrpnComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

void UVrpnComponent::OnRegister()
{
	Super::OnRegister();
	
	//Attach the delegate pointer automatically to the owner of the component
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	VrpnStartup(vrpnServerIP, vrpnTrackerName);
}

void UVrpnComponent::OnUnregister()
{
	Super::OnUnregister();
	VrpnShutdown();
}

void UVrpnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Forward the component tick
	VrpnTick(DeltaTime);
}

//Functions forwards, required implementations
bool UVrpnComponent::IsRemoteEnabled()
{
	return VrpnIsRemoteEnabled();
}


