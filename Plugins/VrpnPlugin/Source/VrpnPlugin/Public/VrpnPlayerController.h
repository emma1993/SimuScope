/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "GameFramework/PlayerController.h"
#include "VrpnDelegateBlueprint.h"
#include "VrpnPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AVrpnPlayerController : public APlayerController, public VrpnDelegateBlueprint, public IVrpnInterface
{
	GENERATED_UCLASS_BODY()

	//Callable Blueprint functions - Need to be defined for direct access
	/**
	* Check if Vrpn Remote is enabled, if its not it will remain so until restart.
	*/
	UFUNCTION(BlueprintCallable, Category = VrpnFunctions)
	bool IsRemoteEnabled();

	UPROPERTY(EditAnywhere)
	FString vrpnServerIP;

	UPROPERTY(EditAnywhere)
	FString vrpnTrackerName;

	////Required for setting delegate (startup) and forwarding Tick to the Vrpn Delegate.
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
};
