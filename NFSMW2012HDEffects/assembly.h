#pragma once

DWORD RefreshGraphicsCodeCaveExit1 = 0x476F4D;
DWORD RefreshGraphicsCodeCaveExit2 = 0x476F29;
DWORD RefreshGraphicsCodeCaveExit3 = 0x476F32;
DWORD RefreshGraphicsIsSafe, RefreshGraphicsIsComplete;
DWORD sub_8C33A0 = 0x8C33A0;
DWORD ToggleHUDCodeCaveExit = 0x6211A9;
DWORD ToggleScreenDirtExit = 0x5C42FF;

void __declspec(naked) SplashCheckCodeCave()
{
	__asm
	{
		// This code is only active beyond the splash screen. 
		// Writes 0x01 to an address to indicate the game is beyond the splash screen.
		mov byte ptr ds : [RefreshGraphicsIsSafe] , 0x01
		mov byte ptr ds : [edi + 0x80] , cl
		ret
	}
}

void __declspec(naked) RefreshGraphicsCodeCave()
{
	__asm
	{
		cmp byte ptr[RefreshGraphicsIsSafe], 0x00
		je RefreshGraphicsCodeCave_Inactive // jumps if NOT beyond the splash screen (crash prevention)
		cmp byte ptr[RefreshGraphicsIsComplete], 0x01
		je RefreshGraphicsCodeCave_Inactive // jumps if fix has already been applied (crash prevention)
		mov byte ptr[RefreshGraphicsIsComplete], 0x01
		lea ecx, dword ptr ds : [esi + 0x2BC400]
		mov byte ptr ds : [esi + 0x6AF7AE] , 0x00
		call sub_8C33A0 // refreshes graphics, "fixing" the missing vegetation
		mov byte ptr ds : [esi + 0x03] , 0x00
		mov eax, dword ptr ds : [0x10EDACC]
		jmp RefreshGraphicsCodeCaveExit1

		RefreshGraphicsCodeCave_Inactive :
		cmp byte ptr ds : [esi + 0x03] , 0x00
			jne RefreshGraphicsCodeCaveConditional
			jmp RefreshGraphicsCodeCaveExit2

			RefreshGraphicsCodeCaveConditional :
		jmp RefreshGraphicsCodeCaveExit3
	}
}

void __declspec(naked) HotkeyTimerCodeCave()
{
	__asm pushad
	Sleep(100); // 100ms delay
	__asm popad
	__asm ret
}

void __declspec(naked) ToggleHUDCodeCave()
{
	__asm
	{
		cmp byte ptr ds : [ToggleHUD] , 0x01
		je IsEnabled
		jmp IsDisabled

		IsEnabled :
		push dword ptr ds : [ToggleHUDhotkey]
			call GetAsyncKeyState
			cmp ah, 0x80
			jne ExitCode // jumps if hotkey is not pressed
			call HotkeyTimerCodeCave
			mov byte ptr ds : [ToggleHUD] , 0x00
			jmp ExitCode

			IsDisabled :
		push dword ptr ds : [ToggleHUDhotkey]
			call GetAsyncKeyState
			cmp ah, 0x80
			jne ExitCode // jumps if hotkey is not pressed
			call HotkeyTimerCodeCave
			mov byte ptr ds : [ToggleHUD] , 0x01

			ExitCode :
			cmp byte ptr ds : [ToggleHUD] , 0x01
			je EnableHUD
			pop edi
			pop esi
			pop ebx
			mov esp, ebp
			pop ebp
			ret 0x18

			EnableHUD:
		mov dword ptr ds : [esp + 0x1C] , 0x28
			jmp ToggleHUDCodeCaveExit
	}
}

void __declspec(naked) ToggleScreenDirtCodeCave()
{
	__asm
	{
		cmp byte ptr ds : [ToggleScreenDirt] , 0x01
		je IsEnabled
		jmp IsDisabled

		IsEnabled :
		push dword ptr ds : [ToggleScreenDirthotkey]
			call GetAsyncKeyState
			cmp ah, 0x80
			jne ExitCode // jumps if hotkey is not pressed
			call HotkeyTimerCodeCave
			mov byte ptr ds : [ToggleScreenDirt] , 0x00
			jmp ExitCode

			IsDisabled :
		push dword ptr ds : [ToggleScreenDirthotkey]
			call GetAsyncKeyState
			cmp ah, 0x80
			jne ExitCode // jumps if hotkey is not pressed
			call HotkeyTimerCodeCave
			mov byte ptr ds : [ToggleScreenDirt] , 0x01

			ExitCode :
			cmp byte ptr ds : [ToggleScreenDirt] , 0x01
			je EnableScreenDirt
			mov dword ptr ds : [ebx + 0x1A4] , 0x01 // Disables screen dirt

			EnableScreenDirt :
			fld dword ptr ds : [esi + 0x98]
			jmp ToggleScreenDirtExit
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