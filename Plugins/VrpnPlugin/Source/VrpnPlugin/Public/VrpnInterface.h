/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "VrpnController.h"
#include "VrpnInterface.generated.h"

UINTERFACE(MinimalAPI)
class UVrpnInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IVrpnInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = VrpnEvents)
	void DeviceDisabled();

	UFUNCTION(BlueprintImplementableEvent, Category = VrpnEvents)
	void OnVrpnTrackerData(int32 SensorID, FVector Position, FRotator Rotation);

	virtual FString ToString();
};