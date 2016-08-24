/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "VrpnPluginPrivatePCH.h"

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "VrpnPluginActor.h"

//Constructor/Initializer
AVrpnPluginActor::AVrpnPluginActor(const FObjectInitializer& PCIP)
: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Three mandatory overrides
void AVrpnPluginActor::BeginPlay()
{
	Super::BeginPlay();

	//Actors by default aren't attached to the input chain, so we enable input for this actor to forward Key and Gamepad Events
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	EnableInput(PC);

	//Required Pointer and Startup
	ValidSelfPointer = this;
	VrpnStartup(vrpnServerIP, vrpnTrackerName);
}

void AVrpnPluginActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Required Shutdown
	VrpnShutdown();
}

void AVrpnPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Required Tick
	VrpnTick(DeltaTime);
}


//Functions forwards, required implementations
bool AVrpnPluginActor::IsRemoteEnabled()
{
	return VrpnIsRemoteEnabled();
}



