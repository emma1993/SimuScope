/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"

#include "VrpnDelegate.h"
#include "VrpnPlayerController.h"

//Constructor/Initializer
AVrpnPlayerController::AVrpnPlayerController(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Three mandatory overrides
void AVrpnPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;	//required from v0.7
	VrpnStartup(vrpnServerIP, vrpnTrackerName);
}

void AVrpnPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	VrpnShutdown();
}

void AVrpnPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	VrpnTick(DeltaTime);
}

//Functions forwards, required implementations
bool AVrpnPlayerController::IsRemoteEnabled()
{
	return VrpnIsRemoteEnabled();
}