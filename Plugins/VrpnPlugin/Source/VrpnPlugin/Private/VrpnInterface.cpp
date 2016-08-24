/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "VrpnPluginPrivatePCH.h"
#include "VrpnInterface.h"

UVrpnInterface::UVrpnInterface(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
FString IVrpnInterface::ToString()
{
	return "IVrpnInterface::ToString()";
}
