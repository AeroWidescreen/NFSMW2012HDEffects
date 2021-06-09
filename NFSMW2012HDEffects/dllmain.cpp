#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>
#include "settings.h"
#include "vehicles.h"
#include "assembly.h"

void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSMW2012HDEffects.ini");

	// Main
	Resolution = iniReader.ReadInteger("MAIN", "Resolution", 1);
	ViewDistance = iniReader.ReadInteger("MAIN", "ViewDistance", 1);
	Gameplay = iniReader.ReadInteger("MAIN", "Gameplay", 1);
	General = iniReader.ReadInteger("MAIN", "General", 1);
	Hotkeys = iniReader.ReadInteger("MAIN", "Hotkeys", 1);

	// Resolution
	VehicleReflectionRes = iniReader.ReadInteger("RESOLUTION", "VehicleReflectionRes", 1024);
	WaterReflectionRes = iniReader.ReadInteger("RESOLUTION", "WaterReflectionRes", 1024);
	ShadowRes = iniReader.ReadInteger("RESOLUTION", "ShadowRes", 2048);
	HeadlightShadowRes = iniReader.ReadInteger("RESOLUTION", "HeadlightShadowRes", 2048);
	ReflectionShadowRes = iniReader.ReadInteger("RESOLUTION", "ReflectionShadowRes", 1024);

	// View Distance
	WorldDistance = iniReader.ReadFloat("VIEW DISTANCE", "WorldDistance", 15000.0f);
	WaterReflectionDistance = iniReader.ReadFloat("VIEW DISTANCE", "WaterReflectionDistance", 1000.0f);
	VehicleReflectionDistance = iniReader.ReadFloat("VIEW DISTANCE", "VehicleReflectionDistance", 400.0f);

	// Gameplay
	EnableHiddenCameras = iniReader.ReadInteger("GAMEPLAY", "EnableHiddenCameras", 1);
	TimeCycleSpeed = iniReader.ReadFloat("GAMEPLAY", "TimeCycleSpeed", 1.0f);
	WreckResistance = iniReader.ReadFloat("GAMEPLAY", "WreckResistance", 1.0f);

	// General
	CinematicMode = iniReader.ReadInteger("GENERAL", "CinematicMode", 1);
	AutoRefreshGraphics = iniReader.ReadInteger("GENERAL", "AutoRefreshGraphics", 1);
	HighShadowRefreshRate = iniReader.ReadInteger("GENERAL", "HighShadowRefreshRate", 1);
	MotionBlurStrength = iniReader.ReadFloat("GENERAL", "MotionBlurStrength", 5.0f);
	SetReflectionShadow = iniReader.ReadInteger("GENERAL", "SetReflectionShadow", -1);
	SetDOF = iniReader.ReadInteger("GENERAL", "SetDOF", -1);

	// Hotkeys
	ToggleHUD = iniReader.ReadInteger("HOTKEYS", "ToggleHUD", 1);
	ToggleHUDhotkey = iniReader.ReadInteger("HOTKEYS", "ToggleHUDhotkey", 117); // F6
	ToggleScreenDirt = iniReader.ReadInteger("GENERAL", "ToggleScreenDirt", 1);
	ToggleScreenDirthotkey = iniReader.ReadInteger("HOTKEYS", "ToggleScreenDirthotkey", 118); // F7
	ToggleTimeOfDayhotkey = iniReader.ReadInteger("HOTKEYS", "ToggleTimeOfDayhotkey", 119); // F8

	{
		injector::MakeJMP(0x4C1EC5, MultiplayerCheckCodeCave, true);
		injector::MakeNOP(0x4C1ECA, 4, true);
	}

	if (Resolution)
	{
		if (VehicleReflectionRes)
		{
			if (VehicleReflectionRes < 128)
			{
				VehicleReflectionRes = 128;
			}
			if (VehicleReflectionRes > 1024)
			{
				VehicleReflectionRes = 1024;
			}

			static int8_t VehicleReflMipmapCount;
			if (VehicleReflectionRes >= 128)
			{
				VehicleReflMipmapCount = 8;
			}
			if (VehicleReflectionRes >= 256)
			{
				VehicleReflMipmapCount = 9;
			}
			if (VehicleReflectionRes >= 512)
			{
				VehicleReflMipmapCount = 10;
			}
			if (VehicleReflectionRes >= 1024)
			{
				VehicleReflMipmapCount = 11;
			}

			// Forces CinematicMode Values
			injector::MakeNOP(0x8B21DE, 2, true);
			injector::MakeNOP(0x8B2457, 2, true);
			// CinematicMode Values
			injector::WriteMemory<uint32_t>(0x8B21E1, VehicleReflectionRes, true);
			injector::WriteMemory<uint32_t>(0x8B245D, VehicleReflectionRes, true);
			injector::WriteMemory<uint8_t>(0x8B21E7, VehicleReflMipmapCount, true);
		}

		if (WaterReflectionRes)
		{
			if (WaterReflectionRes <= 128)
			{
				WaterReflectionRes = 128;
			}

			if (WaterReflectionRes >= 4096)
			{
				WaterReflectionRes = 4096;
			}

			static int8_t WaterReflMipmapCount;
			if (WaterReflectionRes >= 128)
			{
				WaterReflMipmapCount = 5;
			}
			if (WaterReflectionRes >= 256)
			{
				WaterReflMipmapCount = 6;
			}
			if (WaterReflectionRes >= 512)
			{
				WaterReflMipmapCount = 7;
			}
			if (WaterReflectionRes >= 1024)
			{
				WaterReflMipmapCount = 8;
			}
			if (WaterReflectionRes >= 2048)
			{
				WaterReflMipmapCount = 9;
			}
			if (WaterReflectionRes >= 4096)
			{
				WaterReflMipmapCount = 10;
			}

			// Forces High Values
			injector::WriteMemory<uint8_t>(0x8B2635, 0xEB, true);
			injector::MakeNOP(0x8B264B, 2, true);
			// High Values
			injector::WriteMemory<uint32_t>(0x8B264E, WaterReflectionRes, true);
			injector::WriteMemory<uint8_t>(0x8B2656, WaterReflMipmapCount, true);
		}

		if (ShadowRes)
		{
			if (ShadowRes < 128)
			{
				ShadowRes = 128;
			}
			if (ShadowRes > 2730)
			{
				ShadowRes = 2730;
			}

			// Forces CinematicMode Values
			injector::MakeNOP(0x4149B9, 2, true);
			// Forces CinematicMode Values
			injector::WriteMemory<uint32_t>(0x4149C1, ShadowRes, true);
		}

		if (HeadlightShadowRes)
		{
			if (HeadlightShadowRes < 128)
			{
				HeadlightShadowRes = 128;
			}
			if (HeadlightShadowRes > 2730)
			{
				HeadlightShadowRes = 2730;
			}

			injector::WriteMemory<uint32_t>(0x8BAD47, HeadlightShadowRes, true);
			injector::WriteMemory<uint32_t>(0x8BAD4E, HeadlightShadowRes, true);
		}

		if (ReflectionShadowRes)
		{
			if (ReflectionShadowRes < 128)
			{
				ReflectionShadowRes = 128;
			}
			if (ReflectionShadowRes > 2730)
			{
				ReflectionShadowRes = 2730;
			}

			injector::WriteMemory<uint32_t>(0x8A2BC9, ReflectionShadowRes, true);
			injector::WriteMemory<uint32_t>(0x8A2BD0, ReflectionShadowRes, true);
		}
	}

	if (ViewDistance)
	{
		if (WorldDistance)
		{
			if (WorldDistance < 100.0f)
			{
				WorldDistance = 100.0f;
			}
			if (WorldDistance > 15000.0f)
			{
				WorldDistance = 15000.0f;
			}

			injector::WriteMemory(0x41C90B, &WorldDistance, true);
		}

		if (WaterReflectionDistance)
		{
			if (WaterReflectionDistance < 100.0f)
			{
				WaterReflectionDistance = 100.0f;
			}
			if (WaterReflectionDistance > 15000.0f)
			{
				WaterReflectionDistance = 15000.0f;
			}

			injector::WriteMemory(0x8C85E1, &WaterReflectionDistance, true);
		}

		if (VehicleReflectionDistance)
		{
			if (VehicleReflectionDistance < 100.0f)
			{
				VehicleReflectionDistance = 100.0f;
			}
			if (VehicleReflectionDistance > 15000.0f)
			{
				VehicleReflectionDistance = 15000.0f;
			}

			injector::WriteMemory(0x893997, &VehicleReflectionDistance, true);
		}
	}

	if (Gameplay)
	{
		if (EnableHiddenCameras)
		{
			// Discovered by _mRally2
			// Enables Hidden Camera Views
			injector::MakeCALL(0x846922, HiddenCameraCodeCave);
			injector::MakeNOP(0x0846927, 23, true);
			// Corrects Hood Cam Settings
			injector::MakeCALL(0x5C4830, Front_HoodCamSettingsCodeCave);
			injector::MakeNOP(0x5C4835, 13, true);
			injector::MakeCALL(0x5C4854, Rear_HoodCamSettingsCodeCave);
			injector::MakeNOP(0x5C4859, 13, true);
			// Prevents HUD from disappearing when using Hood Cam
			injector::WriteMemory<uint16_t>(0x5DADB2, 0x00B0, true); // mov al,00
			injector::MakeNOP(0x5DADB4, 1, true);
		}

		if (TimeCycleSpeed >= 0)
		{
			TimeCycleMultiplier = TimeCycleSpeed;
			
			if (TimeCycleMultiplier > 8.0f)
			{
				TimeCycleMultiplier = 8.0f;
			}

			injector::MakeCALL(0x8B4C1A, TimeCycleSpeedCodeCave);
			injector::MakeNOP(0x8B4C1F, 1, true);
		}

		if (WreckResistance >= 0)
		{
			WreckResistanceMultiplier = WreckResistance;

			if (WreckResistanceMultiplier > 4.0f)
			{
				WreckResistanceMultiplier = 4.0f;
			}

			injector::MakeCALL(0x8FFBC8, WreckResistanceCodeCave1);
			injector::MakeNOP(0x8FFBCD, 7, true);
			injector::MakeCALL(0x8FFBDB, WreckResistanceCodeCave2);
			injector::MakeNOP(0x8FFBE0, 5, true);
			injector::MakeCALL(0x8FFBEC, WreckResistanceCodeCave3);
			injector::MakeNOP(0x8FFBF1, 5, true);
			injector::MakeCALL(0x8FFC00, WreckResistanceCodeCave4);
			injector::MakeNOP(0x8FFC05, 3, true);
			injector::MakeCALL(0x8FFC08, WreckResistanceCodeCave5);
			injector::MakeNOP(0x8FFC0D, 5, true);
		}
	}

	if (General)
	{
		if (CinematicMode)
		{
			if (CinematicMode >= 2)
			{
				// Improves vehicle reflection LOD
				injector::WriteMemory<uint32_t>(0x8C82FF, 0x04000000, true);
				injector::WriteMemory<uint32_t>(0x8C8304, 0x00008C00, true);
				// Improves water reflection LOD
				injector::WriteMemory<uint32_t>(0x8C8AD7, 0x04000000, true);
				injector::WriteMemory<uint32_t>(0x8C8ADC, 0x00008C00, true);
			}
			
			// CinemeticMode
			static bool CinematicBool = CinematicMode;
			injector::WriteMemory<uint8_t>(0x010EB850, 0x01, true); // Writes directly to "cinematic" address.
			injector::WriteMemory<uint8_t>(0x414E60, 0xEB, true); // Prevents settings from saving
			injector::WriteMemory<uint8_t>(0x415B0B, 0xEB, true); // Disables headlight shadows and overwrites other settings
			injector::WriteMemory<uint8_t>(0x571AC9, 0xEB, true); // Causes freeze when loading a race

			/*
			injector::WriteMemory(0x414901, &CinematicBool, true);
			injector::WriteMemory(0x4149B4, &CinematicBool, true);
			injector::WriteMemory(0x496ECE, &CinematicBool, true);
			injector::WriteMemory(0x4A09EC, &CinematicBool, true);
			injector::WriteMemory(0x538AE1, &CinematicBool, true);
			injector::WriteMemory(0x54F0E7, &CinematicBool, true);
			injector::WriteMemory(0x55002E, &CinematicBool, true);
			injector::WriteMemory(0x8940C8, &CinematicBool, true);
			injector::WriteMemory(0x8940C8, &CinematicBool, true);
			injector::WriteMemory(0x89F28E, &CinematicBool, true);
			injector::WriteMemory(0x8A293B, &CinematicBool, true);
			injector::WriteMemory(0x8A294E, &CinematicBool, true);
			injector::WriteMemory(0x8A2CA8, &CinematicBool, true);
			injector::WriteMemory(0x8A2EC7, &CinematicBool, true);
			injector::WriteMemory(0x8A4260, &CinematicBool, true);
			injector::WriteMemory(0x8A433A, &CinematicBool, true);
			injector::WriteMemory(0x8A441B, &CinematicBool, true);
			injector::WriteMemory(0x8A45DF, &CinematicBool, true);
			injector::WriteMemory(0x8B21DA, &CinematicBool, true);
			injector::WriteMemory(0x8B2453, &CinematicBool, true);
			injector::WriteMemory(0x8B62A8, &CinematicBool, true);
			injector::WriteMemory(0x8BA7BA, &CinematicBool, true);
			injector::WriteMemory(0x8C7732, &CinematicBool, true);
			injector::WriteMemory(0x8CA22C, &CinematicBool, true);
			injector::WriteMemory(0x8CD62B, &CinematicBool, true);
			injector::WriteMemory(0x4143F8, &CinematicBool, true); // Redundant
			injector::WriteMemory(0x4146A2, &CinematicBool, true); // Redundant
			injector::WriteMemory(0x414E58, &CinematicBool, true); // Prevents settings from saving
			injector::WriteMemory(0x415B06, &CinematicBool, true); // Disables headlight shadows and overwrites other settings
			injector::WriteMemory(0x4A1D25, &CinematicBool, true); // Unknown
			injector::WriteMemory(0x571AC3, &CinematicBool, true); // Causes freeze when loading a race
			injector::WriteMemory(0x8E55DE, &CinematicBool, true); // Unknown
			*/
		}

		if (AutoRefreshGraphics)
		{
			injector::MakeCALL(0x4A1D29, SplashCheckCodeCave, true);
			injector::MakeNOP(0x4A1D2E, 1, true);

			injector::MakeJMP(0x476F23, RefreshGraphicsCodeCave, true);
			injector::MakeNOP(0x476F28, 1, true);
		}

		if (HighShadowRefreshRate)
		{
			injector::MakeNOP(0x8A8FA7, 2, true);
		}

		if (MotionBlurStrength >= 0)
		{
			MotionBlurMultiplier = (MotionBlurStrength * 0.1f);

			if (MotionBlurMultiplier > 1.0f)
			{
				MotionBlurMultiplier = 1.0f;
			}

			injector::MakeNOP(0x8CA3B6, 6, true); // Makes motion blur consistent
			injector::MakeCALL(0x8CA3BC, MotionBlurStrengthCodeCave, true);
			injector::MakeNOP(0x8CA3C1, 6, true);
		}

		if (SetReflectionShadow >= 0)
		{
			if (SetReflectionShadow == 0)
			{
				injector::WriteMemory<uint8_t>(0x8CFE8E, 0xEB, true); // jmp 0x8CFEC6 (DISABLE)
			}
			else
			{
				injector::MakeNOP(0x8CFE8E, 2, true); // nop nop (ENABLE)
			}
		}

		if (SetDOF >= 0)
		{
			if (SetDOF == 0)
			{
				injector::WriteMemory<uint16_t>(0x8CD9B9, 0x00B0, true); // mov al, 0x00 (DISABLE)
				injector::MakeNOP(0x8CD9BB, 1, true);
			}
			else
			{
				injector::WriteMemory<uint16_t>(0x8CD9B9, 0x01B0, true); // mov al, 0x01 (ENABLE)
				injector::MakeNOP(0x8CD9BB, 1, true);
			}
		}
	}

	if (Hotkeys)
	{
		// if (ToggleHUD)
		{
			injector::MakeJMP(0x6211A1, ToggleHUDCodeCave, true);
			injector::MakeNOP(0x6211A6, 3, true);
		}

		// if (ToggleScreenDirt)
		{
			injector::MakeJMP(0x5C12C0, ToggleScreenDirtCodeCave, true);
			injector::MakeNOP(0x5C12C5, 10, true);
		}

		// if (ToggleTimeOfDay)
		{
			injector::MakeCALL(0x8B4C0A, ToggleTimeOfDayCodeCave, true);
			injector::MakeNOP(0x8B4C0F, 3, true);
		}
	}
}
	
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		// 0xEE279B = Disc 
		// 0x11DD830 = Steam & Origin
		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0xEE279B || (base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x11DD830) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use Origin or Steam v1.5 NFS13.exe (13,0 MB (13,684,056 bytes)).", "NFSMW2012 HD Effects by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}