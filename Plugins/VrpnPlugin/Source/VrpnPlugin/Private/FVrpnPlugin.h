/* Copyright (C) 2015 VRLab Reutlingen
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once

class DataCollector;
class VrpnDelegate;
void VRPN_CALLBACK handle_analog(void *userdata, const vrpn_ANALOGCB a);
void VRPN_CALLBACK handle_tracker(void *userdata, const vrpn_TRACKERCB a);

class FVrpnPlugin : public IVrpnPlugin
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	/** Delegate Method To subscribe to event calls, only supports one listener for now */
	void SetDelegate(VrpnDelegate* newDelegate);
	void RemoveDelegate();

	/** Manual looping, currently called in main thread */
	void VrpnTick(float DeltaTime);

	/** Optional Public API For direct module bind */
	bool IsRemoteEnabled();

	DataCollector *collector;
private:
	
};