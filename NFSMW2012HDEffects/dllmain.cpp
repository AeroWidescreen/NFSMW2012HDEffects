#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>
#include "settings.h""
#include "assembly.h"

void Init()
{

	// Read values from .ini
	CIniReader iniReader("NFSMW2012HDEffects.ini");

	// Debug
	Resolution = iniReader.ReadInteger("DEBUG", "Resolution", 1);
	ViewDistance = iniReader.ReadInteger("DEBUG", "ViewDistance", 1);
	General = iniReader.ReadInteger("DEBUG", "General", 1);
	Hotkeys = iniReader.ReadInteger("DEBUG", "Hotkeys", 1);

	// Resolution
	VehicleReflectionRes = iniReader.ReadInteger("RESOLUTION", "VehicleReflectionRes", 1024);
	WaterReflectionRes = iniReader.ReadInteger("RESOLUTION", "WaterReflectionRes", 1024);
	ShadowRes = iniReader.ReadInteger("RESOLUTION", "ShadowRes", 2048);
	HeadlightShadowRes = iniReader.ReadInteger("RESOLUTION", "HeadlightShadowRes", 2048);
	ReflectionShadowRes = iniReader.ReadInteger("RESOLUTION", "ReflectionShadowRes", 1024);

	// View Distance
	WorldDistance = iniReader.ReadFloat("VIEW DISTANCE", "WorldDistance", 15000.0f);
	WaterReflectionDistance = iniReader.ReadFloat("VIEW DISTANCE", "WaterReflectionDistance", 1000.0f);
	VehicleReflectionDistance = iniReader.ReadFloat("VIEW DISTANCE", "VehicleReflectionDistance", 500.0f);

	// General
	CinematicMode = iniReader.ReadInteger("GENERAL", "CinematicMode", 1);
	AutoRefreshGraphics = iniReader.ReadInteger("GENERAL", "AutoRefreshGraphics", 1);
	MotionBlurStrength = iniReader.ReadFloat("GENERAL", "MotionBlurStrength", 5.0f);
	SetReflectionShadow = iniReader.ReadInteger("GENERAL", "SetReflectionShadow", -1);
	SetDOF = iniReader.ReadInteger("GENERAL", "SetDOF", -1);

	// Hotkeys
	ToggleHUD = iniReader.ReadInteger("HOTKEYS", "ToggleHUD", 1);
	ToggleHUDhotkey = iniReader.ReadInteger("HOTKEYS", "ToggleHUDhotkey", 117); // F6
	ToggleScreenDirt = iniReader.ReadInteger("GENERAL", "ToggleScreenDirt", 1);
	ToggleScreenDirthotkey = iniReader.ReadInteger("HOTKEYS", "ToggleScreenDirthotkey", 118); // F7

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

	if (General)
	{
		if (CinematicMode)
		{
			injector::WriteMemory<uint32_t>(0x8C82FF, 0x00000000, true); // Improves vehicle reflection LOD
			static BYTE CinematicBool = CinematicMode;
			/*
			injector::WriteMemory<uint8_t>(0x010EB850, 0x01, true); // Writes directly to "cinematic" address. Don't do this.
			injector::WriteMemory(0x4143F8, &CinematicBool, true); // Redundant
			injector::WriteMemory(0x4146A2, &CinematicBool, true); // Redundant
			injector::WriteMemory(0x415B06, &CinematicBool, true); // Disables headlightt shadows and overwrites other settings
			injector::WriteMemory(0x571AC3, &CinematicBool, true); // Causes freeze when loading a race and some cars
			*/
			injector::WriteMemory(0x414901, &CinematicBool, true);
			injector::WriteMemory(0x4149B4, &CinematicBool, true);
			injector::WriteMemory(0x414E58, &CinematicBool, true);
			injector::WriteMemory(0x496ECE, &CinematicBool, true);
			injector::WriteMemory(0x4A09EC, &CinematicBool, true);
			injector::WriteMemory(0x4A1D25, &CinematicBool, true);
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
			injector::WriteMemory(0x8E55DE, &CinematicBool, true);
		}

		if (AutoRefreshGraphics)
		{
			injector::MakeCALL(0x4A1D29, SplashCheckCodeCave, true);
			injector::MakeNOP(0x4A1D2E, 1, true);

			injector::MakeJMP(0x476F23, RefreshGraphicsCodeCave, true);
			injector::MakeNOP(0x476F28, 1, true);
		}

		if (MotionBlurStrength >= 0)
		{
			MotionBlurMultiplier = MotionBlurStrength * 0.1f;

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
			injector::MakeJMP(0x5C42F9, ToggleScreenDirtCodeCave, true);
			injector::MakeNOP(0x5C42FE, 1, true);
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
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use Origin v1.5 NFS13.exe (13,0 MB (13,684,056 bytes)).", "NFSMW2012 HD Effects By Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}