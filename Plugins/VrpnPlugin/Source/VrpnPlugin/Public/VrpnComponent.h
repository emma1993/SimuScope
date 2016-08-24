/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "VrpnPluginPrivatePCH.h"
#include "VrpnDelegate.h"
#include "VrpnDelegateBlueprint.h"
#include "VrpnComponent.generated.h"

UCLASS(ClassGroup="Input Controller", meta=(BlueprintSpawnableComponent))
class UVrpnComponent : public UActorComponent, public VrpnDelegateBlueprint
{
	GENERATED_UCLASS_BODY()

public:

	//Callable Blueprint functions - Need to be defined for direct access
	/**
	* Check if Vrpn Remote is enabled, if its not it will remain so until restart.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = VrpnFunctions)
	bool IsRemoteEnabled();

	UPROPERTY(EditAnywhere)
	FString vrpnServerIP;

	UPROPERTY(EditAnywhere)
	FString vrpnTrackerName;

	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};