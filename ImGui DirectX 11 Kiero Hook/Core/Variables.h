#pragma once
#include <cstdint>
#include <Utils/Includes.h>
#include <string>
#include <lua.hpp>

namespace Variables
{
	std::string Prefix = "@kiocode";
	const char* GameAssemblyName = "GameAssembly.dll";
	const char* UnityPlayerName = "UnityPlayer.dll";
	bool DEBUG = true;

	enum MenuTab
	{
		TAB_VISUALS,
		TAB_AIM,
		TAB_EXPLOITS,
		TAB_MISC,
		TAB_DEV
	};

	namespace System 
	{
		bool Init = false;
		bool InitIL2Cpp = false;

		POINT MousePos = { 0, 0 };

		inline static UINT vps = 1;
		Vector2 ScreenSize = { 0, 0 };
		Vector2 ScreenCenter = { 0, 0 };
		D3D11_VIEWPORT Viewport;
	}

	namespace Offsets
	{
		uintptr_t UnityEngineShader__FindShader_Offset = 0x0;
		uintptr_t UnityEngineTime__GetTimeScale_Offset = 0x0;
		uintptr_t UnityEngineTime__SetTimeScale_Offset = 0x0;
		//uintptr_t Health__TakeDamage_Offset = 0x0;
	}

	namespace Lua {
		lua_State* LuaState = nullptr;
		bool ShowEditor = false;

		char LuaScript[9999] = R"(
print("Hello World!")

local test = 0
print(test)

function TestFunction()
	print("TestFunction")
end

TestFunction()

-- call c++ function
testFn()
		)";
	}

	namespace CheatVariables 
	{
		Unity::CGameObject* LocalPlayer = NULL;
		std::vector<Unity::CGameObject*> PlayersList(NULL);

		Unity::CGameObject* TargetPlayer = NULL;
		ImColor TargetPlayerColor = ImColor(255, 0, 0);

		UnityEngine_Shader_o* ChamsShader;

		std::vector<std::pair<int, int>> BonePairs = {
			// left foot
			{2, 1},
			{1, 0},
			{0, 3},
			// right foot
			{31, 30},
			{30, 20},
			{20, 3},
			// Spine
			{3, 4},
			// Right Hand
			{4, 9},
			// Left Hand
			{4, 7},
			// Neck
			{4, 5},
			// Head
			{5, 6},
		};

		ImVec4 Rainbow = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Global rainbow color
		ImColor RainbowColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255); // Global rainbow color

		namespace TestObjects {// developers test scope
			std::vector<Unity::CGameObject*> List(NULL);
			char placeholder[] = "UnityEngine.CapsuleCollider";
			char* Name = placeholder;
			bool Chams = false;
			bool Snapline = false;
			ImColor SnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
			bool Box = false;
			ImColor BoxColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
			bool Aimbot = false;
		}

		static DWORD LastShotTime = 0;
		static DWORD LastTick = 0;
	}

	namespace CheatMenuVariables {

		bool ShowMenu = false;
		bool Watermark = false;

		bool CameraFovChanger = false;
		float CameraCustomFOV = 80.0f;

		bool EnableDeveloperOptions = false;

		bool ShowInspector = false;

		float GameSpeed = 1.0f;
		
		bool ShowMouse = true;
		bool RainbowMouse = false;
		ImColor MouseColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255); 
		int MouseType = 0;

		bool Crosshair = false;
		bool RainbowCrosshair = false;
		ImColor CrosshairColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
		float CrosshairSize = 5.0f;
		int CrosshairType = 0;

		bool PlayersSnapline = false;
		bool RainbowPlayersSnapline = false;
		ImColor PlayersSnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
		int PlayersSnaplineType = 2;

		bool PlayerChams = false;
		bool RainbowPlayerChams = false;

		bool PlayerSkeleton = false;
		bool RainbowPlayerSkeleton = false;
		ImColor PlayerSkeletonColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
		
		bool PlayersBox = false;
		bool RainbowPlayersBox = false;
		ImColor PlayersBoxColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
		bool PlayersBoxFilled = false;

		bool BotChecker = false;
		bool RainbowBotChecker = false;
		ImColor BotCheckerColor = ImColor(0, 0, 255);
		bool BotCheckerText = true;

		bool PlayersHealth = false;

		bool GodMode = false;

		bool NoRecoil = false;

		bool NoSpread = false;

		bool RapidFire = false;

		bool OneShot = false;

		bool InfiniteAmmo = false;

		bool SpeedHack = false;	
		float SpeedValue = 1.0f;

		bool EnableAimbot = false;
		bool AimbotFOVCheck = false;
		float AimbotFOV = 80.0f;
		float AimbotSmoothness = 0.5f;
		float FakeHeadPosDiff = 1;
		float FakeFeetPosDiff = 1;
	}

	namespace KEYS
	{
		inline int SHOWMENU_KEY = VK_INSERT;
		inline int DEATTACH_KEY = VK_F9;
	}
}