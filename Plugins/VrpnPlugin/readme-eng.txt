# VRPN-Plugin for Unreal Engine

The Virtual-Reality Peripheral Network (VRPN) (http://www.cs.unc.edu/Research/vrpn/) is a set of classes within a library and a set of servers that are designed to implement a network-transparent interface between application programs and the set of physical devices (tracker, etc.) used in a virtual-reality (VR) system. The idea is to have a PC or other host at each VR station that controls the peripherals (tracker, button device, haptic device, analog inputs, sound, etc). VRPN provides connections between the application and all of the devices using the appropriate class-of-service for each type of device sharing this link. The application remains unaware of the network topology. Note that it is possible to use VRPN with devices that are directly connected to the machine that the application is running on, either using separate control programs or running all as a single program.

Although a lot of devices are compatible with VRPN, there is currently no integration with the Unreal Engine. Therefore students at Reutlingen University developed a Unreal Engine 4 plugin in 2015, which allows to receive VRPN tracking data of class vrpn_Tracker via C++ or Blueprints. 

In the Virtual Reality Laboratory in Reutlingen this plugin was used to receive data of a WorldViz-Trackingsystem.

## Requirements

* 64 bit system
* Unreal Engine Version 4.10
* Visual Studio 2015 (for VS 2013 see below)

## Installation

Copy the folder containing this file (VrpnPlugin) to your Unreal Engine project in the subfolder "Plugins". If it doesn't exist create one.

To load a plugin in the engine the project need to contain C++ source code. If you only have Blueprints and Unreal content, you have to create an empty C++ class via menu item "File | Add Code to Project". After restarting the engine the plugin should be listed in the Plugins menu.

To receive data via blueprint, open your characters blueprint. There will be a components list on the left, where you can add the VrpnComponent by clicking on "Add Component" and selecting it. To receive events you also have to add the "Vrpn Inteface" to the blueprint. Do this by open the "Class settings" (previously known as "Blueprint props"), go to "Interfaces" and select "Vrpn Inteface" using the "Add" button. After compiling the blueprint you should be able to add an vrpn events to your blueprint.
After this setup you can receive tracker data from a host by editing the host ip and tracker name in the "VRPN" component properties.

## Use of tracker data

Tracker data is received by the event "On Vrpn Tracker Data". This event gets called for each tracker id individually. By using the id you can differentiate the data of position (FVector) and orientation (FRotator) for each tracker. See example in vrpnEvent-example-blueprint image.

## Using Visual Studio 2013 (untested, because current Unreal release uses VS2015)

When using Visual Studio 2013 the build of the external VRPN library is required from VS2013 too.
A pre build version for VS2013 is included in the zip, but you have to manually switch to this version:

Open VrpnPlugin.Build.cs in Visual Studio and replace "VS2015" with "VS2013" in line 85.

After this change the plugin and your Unreal project should build correctly in VS2013.

## License

This software may be modified and distributed under the terms
of the MIT license. See the LICENSE file for details.

The Software makes use of third party software. Use of some third party materials included in the Software are subject to the third party terms and conditions in the LICENSE-3RD-PARTY file, in addition to the terms and conditions included above.

