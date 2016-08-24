/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "VrpnDelegate.h"
#include "VrpnController.generated.h"

UCLASS(BlueprintType)
class UVrpnController : public UObject
{
friend class VrpnDelegateBlueprint;
	GENERATED_UCLASS_BODY()
public:

private:
	VrpnDelegate* _VrpnDelegate;
};