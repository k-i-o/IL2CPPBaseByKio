#pragma once
#include <cstdint>
#include <Utils/includes.h>
#include <string>

namespace Variables
{
	std::string Prefix = "@kiocode";

	namespace System 
	{
		bool init = false;
		bool initil2cpp = false;

		POINT MousePos = { 0, 0 };

		inline static UINT vps = 1;
		Vector2 screenSize = { 0, 0 };
		Vector2 screenCenter = { 0, 0 };
		D3D11_VIEWPORT viewport;
	}

	namespace Offsets
	{
		uintptr_t YouOriginalClass__MethodName_Offset = 0x0;
	}

	namespace CheatVariables 
	{
		Unity::CGameObject* LocalPlayer = NULL;
		std::vector<Unity::CGameObject*> PlayersList(NULL);

		static DWORD lastShotTime = 0;
		static DWORD lasttick = 0;
	}

	namespace CheatMenuVariables {

		bool ShowMenu = false;

		ImVec4 Rainbow = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Global rainbow color
		ImColor RainbowColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255); // Global rainbow color

		bool ShowInspector = false;
		
		bool ShowMouse = true;
		bool RainbowMouse = false;
		ImColor MouseColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255); 
		int MouseType = 0;

		// Variables pattern
		bool EnableSomething = false;
		bool RainbowSomething = false;
		ImColor SomethingColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);

		// Examples
		/*
		bool EnableAimbot = false;
		bool RainbowTarget = false;
		ImColor TargetColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
		
		bool EnableESP = false;
		bool RainbowESP = false;
		ImColor ESPColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);

		or just

		bool EnableAimbot = false;

		bool EnableESP = false;

		bool EnableInfinitePoints = false;
		*/

	}

	namespace KEYS
	{
		inline int SHOWMENU_KEY = VK_INSERT;
		inline int DEATTACH_KEY = VK_F9;
	}
}