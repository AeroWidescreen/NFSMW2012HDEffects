#pragma once

DWORD IsMultiplayer;

void __declspec(naked) MultiplayerCheckCodeCave()
{
	__asm
	{
		// al 0x01 = Multiplayer
		// al 0x00 = Singleplayer
		cmp dword ptr ds : [eax + 0x6A20], ecx
		setne al
		mov byte ptr ds : [IsMultiplayer], al
		ret
	}
}

DWORD RefreshGraphicsIsSafe;

void __declspec(naked) SplashCheckCodeCave()
{
	__asm
	{
		// This code is only active beyond the splash screen. 
		// Writes 0x01 to an address to indicate the game is beyond the splash screen.
		mov byte ptr ds : [RefreshGraphicsIsSafe] , 0x01
		mov byte ptr ds : [edi + 0x80], cl
		ret
	}
}

DWORD RefreshGraphicsCodeCaveExit1 = 0x476F4D;
DWORD RefreshGraphicsCodeCaveExit2 = 0x476F29;
DWORD RefreshGraphicsCodeCaveExit3 = 0x476F32;
DWORD sub_8C33A0 = 0x8C33A0;
DWORD RefreshGraphicsIsComplete;

void __declspec(naked) RefreshGraphicsCodeCave()
{
	__asm
	{
		cmp byte ptr ds : [RefreshGraphicsIsSafe], 0x00
		je RefreshGraphicsCodeCave_Inactive // jumps if NOT beyond the splash screen (crash prevention)
		cmp byte ptr ds : [RefreshGraphicsIsComplete], 0x01
		je RefreshGraphicsCodeCave_Inactive // jumps if fix has already been applied (crash prevention)
		mov byte ptr ds : [RefreshGraphicsIsComplete], 0x01
		lea ecx, dword ptr ds : [esi + 0x2BC400]
		mov byte ptr ds : [esi + 0x6AF7AE], 0x00
		call sub_8C33A0 // refreshes graphics, "fixing" the missing vegetation
		mov byte ptr ds : [esi + 0x03], 0x00
		mov eax, dword ptr ds : [0x10EDACC]
		jmp RefreshGraphicsCodeCaveExit1

	RefreshGraphicsCodeCave_Inactive:
		cmp byte ptr ds : [esi + 0x03], 0x00
		jne RefreshGraphicsCodeCaveConditional
		jmp RefreshGraphicsCodeCaveExit2

	RefreshGraphicsCodeCaveConditional:
		jmp RefreshGraphicsCodeCaveExit3
	}
}

DWORD ToggleHUDCodeCaveExit = 0x6211A9;
DWORD ToggleHUDhotkeyPressed;

void __declspec(naked) ToggleHUDCodeCave()
{
	__asm
	{
		cmp byte ptr ds : [ToggleHUD], 0x01
		je HUDToggleCheck_Enabled
		jmp HUDToggleCheck_Disabled

	HUDToggleCheck_Enabled:
		cmp byte ptr ds : [ToggleHUDhotkeyPressed], 0x01
		je HUDTogglePressed_Enabled
		push dword ptr ds : [ToggleHUDhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleHUDhotkeyPressed], 0x01
		
	HUDTogglePressed_Enabled:
		push dword ptr ds : [ToggleHUDhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleHUD], 0x00
		mov byte ptr ds : [ToggleHUDhotkeyPressed], 0x00
		jmp ExitCode

	HUDToggleCheck_Disabled:
		cmp byte ptr ds : [ToggleHUDhotkeyPressed], 0x01
		je HUDTogglePressed_Disabled
		push dword ptr ds : [ToggleHUDhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleHUDhotkeyPressed], 0x01
		
	HUDTogglePressed_Disabled:
		push dword ptr ds : [ToggleHUDhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleHUD], 0x01
		mov byte ptr ds : [ToggleHUDhotkeyPressed], 0x00

	ExitCode :
		cmp byte ptr ds : [ToggleHUD], 0x01
		je EnableHUD
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 0x18

	EnableHUD:
		mov dword ptr ds : [esp + 0x1C], 0x28
		jmp ToggleHUDCodeCaveExit
	}
}

DWORD ToggleScreenDirtExit = 0x5C42FF;
DWORD ToggleScreenDirthotkeyPressed;

void __declspec(naked) ToggleScreenDirtCodeCave()
{
	__asm
	{
		pushad
		cmp byte ptr ds : [ToggleScreenDirt], 0x01
		je ScreenDirtToggleCheck_Enabled
		jmp ScreenDirtToggleCheck_Disabled

	ScreenDirtToggleCheck_Enabled:
		cmp byte ptr ds : [ToggleScreenDirthotkeyPressed], 0x01
		je ScreenDirtTogglePressed_Enabled
		push dword ptr ds : [ToggleScreenDirthotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleScreenDirthotkeyPressed], 0x01
	
	ScreenDirtTogglePressed_Enabled:
		push dword ptr ds : [ToggleScreenDirthotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has been been released
		mov byte ptr ds : [ToggleScreenDirt], 0x00
		mov byte ptr ds : [ToggleScreenDirthotkeyPressed], 0x00
		jmp ExitCode

	ScreenDirtToggleCheck_Disabled:
		cmp byte ptr ds : [ToggleScreenDirthotkeyPressed] , 0x01
		je ScreenDirtTogglePressed_Disabled
		push dword ptr ds : [ToggleScreenDirthotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleScreenDirthotkeyPressed], 0x01
	
	ScreenDirtTogglePressed_Disabled:
		push dword ptr ds : [ToggleScreenDirthotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has been been released
		mov byte ptr ds : [ToggleScreenDirt], 0x01
		mov byte ptr ds : [ToggleScreenDirthotkeyPressed] , 0x00

	ExitCode:
		cmp byte ptr ds : [ToggleScreenDirt], 0x01
		je EnableScreenDirt
		popad
		mov eax, dword ptr ds : [ecx + 0x04]
		xor ecx, ecx
		cmp dword ptr ds : [eax + 0x1A4], ecx
		mov al, 0x00
		ret

	EnableScreenDirt:
		popad
		mov eax, dword ptr ds : [ecx + 0x04]
		xor ecx, ecx
		cmp dword ptr ds : [eax + 0x1A4], ecx
		sete al
		ret
	}
}

float Midnight = 0.0f;
float Dawn = 14700.0f;
float Sunrise = 17500.0f;
float Midday = 50000.0f;
float Sunset = 73000.0f;
float Dusk = 85000.0f;
DWORD ToggleTimeOfDayhotkeyPressed;

void __declspec(naked) ToggleTimeOfDayCodeCave()
{
	__asm
	{
		pushad
		cmp byte ptr ds : [ToggleTimeOfDay], 0x00
		je MidnightToggleCheck
		cmp byte ptr ds : [ToggleTimeOfDay], 0x01
		je DawnToggleCheck
		cmp byte ptr ds : [ToggleTimeOfDay], 0x02
		je SunriseToggleCheck
		cmp byte ptr ds : [ToggleTimeOfDay], 0x03
		je MiddayToggleCheck
		cmp byte ptr ds : [ToggleTimeOfDay], 0x04
		je SunsetToggleCheck
		jmp DuskToggleCheck

	MidnightToggleCheck:
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je MidnightTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01

	MidnightTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x01
		mov eax, dword ptr ds : [Midnight]
		mov dword ptr ds : [esi + 0x2A84], eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x00
		jmp ExitCode

	DawnToggleCheck:
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je DawnTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01

	DawnTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x02
		mov eax, dword ptr ds : [Dawn]
		mov dword ptr ds : [esi + 0x2A84], eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x00
		jmp ExitCode

	SunriseToggleCheck :
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je SunriseTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01

	SunriseTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x03
		mov eax, dword ptr ds : [Sunrise]
		mov dword ptr ds : [esi + 0x2A84] , eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x00
		jmp ExitCode

	MiddayToggleCheck:
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je MiddayTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed] , 0x01

	MiddayTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x04
		mov eax, dword ptr ds : [Midday]
		mov dword ptr ds : [esi + 0x2A84], eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x00
		jmp ExitCode

	SunsetToggleCheck:
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je SunsetTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01

	SunsetTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x05
		mov eax, dword ptr ds : [Sunset]
		mov dword ptr ds : [esi + 0x2A84] , eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed] , 0x00
		jmp ExitCode

	DuskToggleCheck:
		cmp byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01
		je DuskTogglePressed
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x80
		jne ExitCode // jumps if hotkey is not pressed
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x01

	DuskTogglePressed:
		push dword ptr ds : [ToggleTimeOfDayhotkey]
		call GetAsyncKeyState
		cmp ah, 0x00
		jne ExitCode // jumps if hotkey has not been released
		mov byte ptr ds : [ToggleTimeOfDay], 0x00
		mov eax, dword ptr ds : [Dusk]
		mov dword ptr ds : [esi + 0x2A84] , eax
		mov byte ptr ds : [ToggleTimeOfDayhotkeyPressed], 0x00

	ExitCode:
		popad
		movss xmm1, dword ptr ds : [esi + 0x2A84]
		ret
	}
}

void __declspec(naked) MotionBlurStrengthCodeCave()
{
	__asm
	{
		mulss xmm0, dword ptr ds : [MotionBlurMultiplier]
		shufps xmm0, xmm0, 0x00
		lea ecx, dword ptr ds : [esp + 0x414]
		ret
	}
}

void __declspec(naked) TimeCycleSpeedCodeCave()
{
	__asm
	{
		mulss xmm3, dword ptr ds : [TimeCycleMultiplier]
		mulss xmm7, dword ptr ds : [TimeCycleMultiplier]
		xorps xmm6, xmm6
		movaps xmm0, xmm7
		ret
	}
}

void __declspec(naked) HiddenCameraCodeCave()
{
	__asm
	{
		mov eax, dword ptr ds : [esi + 0x04]
		cmp eax, 0x00
		je HeliCam
		cmp eax, 0x01
		je HoodCam
		cmp eax, 0x02
		je BumperCam
		cmp eax, 0x03
		je ChaseCam

	ChaseCam:
		mov dword ptr ds  : [esi + 0x04], 0x00
		ret

	HeliCam:
		cmp byte ptr ds : [EnableHiddenCameras], 0x02
		jl HoodCam
		mov dword ptr ds : [esi + 0x04], 0x01
		ret

	HoodCam:
		mov dword ptr ds : [esi + 0x04], 0x02
		ret

	BumperCam :
		mov dword ptr ds : [esi + 0x04], 0x03
		ret
	}
}

// Y-Axis
float Y_Axis_Front_1 = 1.72f;
float Y_Axis_Front_2 = 1.42f;
float Y_Axis_Front_3 = 1.21f;
float Y_Axis_Front_4 = 1.085f;
float Y_Axis_Front_5 = 0.97f;
float Y_Axis_Front_6 = 0.85f;
float Y_Axis_Front_7 = 0.78f;
// Z-Axis
float Z_Axis_Front_1 = 1.4f;
float Z_Axis_Front_2 = 0.825f;

void __declspec(naked) Front_HoodCamSettingsCodeCave()
{
	__asm
	{
		movss xmm0, dword ptr ds : [esi + edx + 0x24] // X-Axis
		movss xmm2, dword ptr ds : [esi + edx + 0x2C] // Z-Axis
		movss xmm3, dword ptr ds : [esi + edx + 0x28] // Y-Axis 
		push esi
		mov esi, dword ptr ds : [CAR_ID_POINTER]
		add esi, 0x04
		mov esi, dword ptr ds : [esi]
		cmp dword ptr ds : [esi + 0xEC] , ALFA_ROMEO_MITO_QV
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , ALFA_ROMEO_4C_CONCEPT
		je Custom6
		cmp dword ptr ds : [esi + 0xEC] , ARIEL_ATOM_V8
		je Custom7
		cmp dword ptr ds : [esi + 0xEC] , AUDI_A1_CLUBSPORT_QUATTRO
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , AUDI_RS3_SPORTBACK
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , BAC_MONO
		je Custom7
		cmp dword ptr ds : [esi + 0xEC] , BENTLEY_SUPERSPORTS_ISR
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , BMW_1M_COUPE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , BMW_M3_COUPE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , CATERHAM_SUPERLIGHT_R500
		je Custom7
		cmp dword ptr ds : [esi + 0xEC] , CATERHAM_SUPERLIGHT_R500_PO
		je Custom7
		cmp dword ptr ds : [esi + 0xEC] , CHEVROLET_CAMARO_ZL1
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , DODGE_CHARGER_RT
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , DODGE_CHARGER_SRT8
		je Custom2
		cmp dword ptr ds : [esi + 0xEC] , FORD_F150_SVT_RAPTOR
		je Custom1
		cmp dword ptr ds : [esi + 0xEC] , FORD_F150_SVT_RAPTOR_PO
		je Custom1
		cmp dword ptr ds : [esi + 0xEC] , FORD_FIESTA_ST
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , FORD_FOCUS_RS500
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , FORD_MUSTANG_BOSS_302
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , HENNESSEY_VENOM_GT_SPYDER
		je Custom6
		cmp dword ptr ds : [esi + 0xEC] , HUMMER_H1_ALPHA
		je Custom2_1
		cmp dword ptr ds : [esi + 0xEC] , JEEP_GRAND_CHEROKEE_SRT8
		je Custom1
		cmp dword ptr ds : [esi + 0xEC] , KOENIGSEGG_AGERA_R
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , LAMBORGHINI_COUNTACH
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , LAMBORGHINI_DIABLO_SV
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , RANGE_ROVER_EVOQUE
		je Custom2
		cmp dword ptr ds : [esi + 0xEC] , MARUSSIA_B2
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , MCLAREN_F1_LM
		je Custom6_1
		cmp dword ptr ds : [esi + 0xEC] , MITSUBISHI_LANCER_EVO_X
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , NISSAN_GTR_EGOIST
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , PORSCHE_918_SPYDER_CONCEPT
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , PORSCHE_PANAMERA_TURBO_S
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , SHELBY_COBRA_427
		je Custom5
		cmp dword ptr ds : [esi + 0xEC] , SHELBY_GT500
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , DODGE_CHALLENGER_SRT8
		je Custom3
		cmp dword ptr ds : [esi + 0xEC] , SUBARU_COSWORTH_IMPREZA
		je Custom4
		cmp dword ptr ds : [esi + 0xEC] , TESLA_ROADSTER_SPORT
		je Custom6
		pop esi
		ret

	Custom1:
		pop esi
		movss xmm3, [Y_Axis_Front_1] // Y 1.72
		ret

	Custom2 :
		pop esi
		movss xmm3, [Y_Axis_Front_2] // Y 1.42
		ret

	Custom2_1 :
		pop esi
		movss xmm2, [Z_Axis_Front_1] // Z 4.0
		movss xmm3, [Y_Axis_Front_2] // Y 1.72
		ret

	Custom3 :
		pop esi
		movss xmm3, [Y_Axis_Front_3] // Y 1.21
		ret

	Custom4 :
		pop esi
		movss xmm3, [Y_Axis_Front_4] // Y 1.15
		ret

	Custom5 :
		pop esi
		movss xmm3, [Y_Axis_Front_5] // Y 0.97
		ret

	Custom6 :
		pop esi
		movss xmm3, [Y_Axis_Front_6] // Y 0.85
		ret

	Custom6_1 :
		pop esi
		movss xmm2, [Z_Axis_Front_2] // Z 0.825
		movss xmm3, [Y_Axis_Front_6] // Y 0.97
		ret

	Custom7 :
		pop esi
		movss xmm3, [Y_Axis_Front_7] // Y 0.78
		ret
	}
}

// Y-Axis
float Y_Axis_Rear_1 = 2.3f;
float Y_Axis_Rear_2 = 2.1f;
float Y_Axis_Rear_3 = 1.65f;
float Y_Axis_Rear_4 = 1.54f;
float Y_Axis_Rear_5 = 1.4f;
float Y_Axis_Rear_6 = 1.25f;

void __declspec(naked) Rear_HoodCamSettingsCodeCave()
{
	__asm
	{
		movss xmm0, dword ptr ds : [esi + edx + 0x18] // X-Axis
		movss xmm2, dword ptr ds : [esi + edx + 0x20] // Z-Axis
		movss xmm3, dword ptr ds : [esi + edx + 0x1C] // Y-Axis
		push esi
		mov esi, dword ptr ds : [CAR_ID_POINTER]
		add esi, 0x04
		mov esi, dword ptr ds : [esi]
		cmp dword ptr ds : [esi + 0xEC], ALFA_ROMEO_MITO_QV
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], ASTON_MARTIN_DB5
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], AUDI_A1_CLUBSPORT_QUATTRO
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], AUDI_RS3_SPORTBACK
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], BMW_1M_COUPE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], BMW_M3_COUPE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], BMW_M3_GTR
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], CHEVROLET_CAMARO_ZL1
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], DODGE_CHARGER_RT
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], DODGE_CHARGER_SRT8
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], FORD_F150_SVT_RAPTOR
		je Custom2
		cmp dword ptr ds : [esi + 0xEC], FORD_F150_SVT_RAPTOR_PO
		je Custom2
		cmp dword ptr ds : [esi + 0xEC], FORD_FIESTA_ST
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], FORD_FOCUS_RS500
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], FORD_MUSTANG_BOSS_302
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], HUMMER_H1_ALPHA
		je Custom1
		cmp dword ptr ds : [esi + 0xEC], JAGUAR_XKR
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], JEEP_GRAND_CHEROKEE_SRT8
		je Custom2
		cmp dword ptr ds : [esi + 0xEC], LANCIA_DELTA_HF_INTEGRALE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], RANGE_ROVER_EVOQUE
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], MASERATI_MC_STRADALE
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], MASERATI_MC_STRADALE_PO
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], MERCEDES_BENZ_SL65
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], MERCEDES_BENZ_SL65_PO
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], MERCEDES_BENZ_SLS
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], MITSUBISHI_LANCER_EVO_X
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], NISSAN_350Z
		je Custom5
		cmp dword ptr ds : [esi + 0xEC], NISSAN_GTR_EGOIST
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], NISSAN_SKYLINE_GTR
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], PORSCHE_911_CARERRA_S
		je Custom5
		cmp dword ptr ds : [esi + 0xEC], PORSCHE_911_CARERRA_S_PO
		je Custom5
		cmp dword ptr ds : [esi + 0xEC], PORSCHE_911_TURBO_3
		je Custom5
		cmp dword ptr ds : [esi + 0xEC], PORSCHE_PANAMERA_TURBO_S
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], SHELBY_GT500
		je Custom4
		cmp dword ptr ds : [esi + 0xEC], DODGE_CHALLENGER_SRT8
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], DODGE_VIPER_GTS
		je Custom5
		cmp dword ptr ds : [esi + 0xEC], SUBARU_COSWORTH_IMPREZA
		je Custom3
		cmp dword ptr ds : [esi + 0xEC], TESLA_ROADSTER_SPORT
		je Custom6
		pop esi
		ret

	Custom1 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_1] // Y 2.5
		ret

	Custom2 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_2] // Y 2.1
		ret

	Custom3 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_3] // Y 1.65
		ret

	Custom4 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_4] // Y 1.54
		ret

	Custom5 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_5] // Y 1.4
		ret

	Custom6 :
		pop esi
		movss xmm3, dword ptr ds : [Y_Axis_Rear_6] // Y 1.25
		ret
	}
}

DWORD Wreck_Resistance_Result;
DWORD Wreck_Resistance_1;
DWORD Wreck_Resistance_2;
DWORD Wreck_Resistance_3;

void __declspec(naked) WreckResistanceCodeCave1()
{
	__asm
	{
		movzx eax, word ptr ds : [edi + 0x0C]
		movzx edx, word ptr ds : [edi + 0x12]
		movzx ecx, word ptr ds : [edi + 0x14]
		cmp byte ptr ds : [IsMultiplayer], 0x01
		je ExitCode

		mov dword ptr ds : [Wreck_Resistance_1], eax
		mov dword ptr ds : [Wreck_Resistance_2], edx
		mov dword ptr ds : [Wreck_Resistance_3], ecx
		// Wreck Resistance 1 Calculations
		fild dword ptr ds : [Wreck_Resistance_1]
		fmul dword ptr ds :  [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov eax, dword ptr ds : [Wreck_Resistance_Result]
		// Wreck Resistance 2 Calculations
		fild dword ptr ds : [Wreck_Resistance_2]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov edx, dword ptr ds : [Wreck_Resistance_Result]
		// Wreck Resistance 3 Calculations
		fild dword ptr ds : [Wreck_Resistance_3]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov ecx, dword ptr ds : [Wreck_Resistance_Result]

	ExitCode:
		ret
	}
}

DWORD Wreck_Resistance_4;

void __declspec(naked) WreckResistanceCodeCave2()
{
	__asm
	{
		movzx eax, word ptr ds : [edi + 0x0E]
		movss dword ptr ds  : [esp + 0x10], xmm0
		cmp byte ptr ds : [IsMultiplayer], 0x01
		je ExitCode

		mov dword ptr ds : [Wreck_Resistance_4], eax
		// Wreck Resistance Calculations
		fild dword ptr ds : [Wreck_Resistance_4]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov eax, dword ptr ds : [Wreck_Resistance_Result]

	ExitCode:
		ret
	}
}

DWORD Wreck_Resistance_5;

void __declspec(naked) WreckResistanceCodeCave3()
{
	__asm
	{
		movzx ecx, word ptr ds : [edi + 0x16]
		movss dword ptr ds : [esp + 0x14], xmm0
		cmp byte ptr ds : [IsMultiplayer], 0x01
		je ExitCode

		mov dword ptr ds : [Wreck_Resistance_5], eax
		// Wreck Resistance Calculations
		fild dword ptr ds : [Wreck_Resistance_5]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov ecx, dword ptr ds : [Wreck_Resistance_Result]

	ExitCode:
		ret
	}
}

DWORD Wreck_Resistance_6;

void __declspec(naked) WreckResistanceCodeCave4()
{
	__asm
	{
		movzx edx, word ptr ds : [edi + 0x10]
		cmp byte ptr ds : [IsMultiplayer], 0x01
		je ExitCode

		mov dword ptr ds : [Wreck_Resistance_6], eax
		// Wreck Resistance Calculations
		fild dword ptr ds : [Wreck_Resistance_6]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov edx, dword ptr ds : [Wreck_Resistance_Result]

	ExitCode:
		cvtsi2ss xmm0, edx
		ret
	}
}

DWORD Wreck_Resistance_7;

void __declspec(naked) WreckResistanceCodeCave5()
{
	__asm
	{
		movzx edx, byte ptr ds : [edi + 0x18]
		movss dword ptr ds : [esp + 0x18], xmm0
		cmp byte ptr ds : [IsMultiplayer], 0x01
		je ExitCode

		mov dword ptr ds : [Wreck_Resistance_7], edx
		// Wreck Resistance Calculations
		fild dword ptr ds : [Wreck_Resistance_7]
		fmul dword ptr ds : [WreckResistanceMultiplier]
		fistp dword ptr ds : [Wreck_Resistance_Result]
		mov edx, dword ptr ds : [Wreck_Resistance_Result]

	ExitCode:
		cmp dword ptr ds : [esi + 0x1768], -01
		ret
	}
}