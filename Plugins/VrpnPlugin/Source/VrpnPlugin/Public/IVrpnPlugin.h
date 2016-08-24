/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "ModuleManager.h"

class VrpnDelegate;

/**
 * The public interface to this module
 */
class IVrpnPlugin : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IVrpnPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IVrpnPlugin >( "VrpnPlugin" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "VrpnPlugin" );
	}

	/**
	 * Public API, implemented in FVrpnPlugin.cpp
	 * Optional way of getting data is to subscribe to the VrpnDelegate class through inheritance
	 */
	virtual void SetDelegate(VrpnDelegate* newDelegate) {};
	virtual void RemoveDelegate(){};
	virtual void VrpnTick(float DeltaTime) {};

	virtual bool IsRemoteEnabled() = 0;
};

