// includes
#include <iostream>
#include <intrin.h>

// my includes
#include <Utils/includes.h>
#include <Utils/SDK.h>
#include <Utils/Utils.h>
#include <Libraries/Fonts/Font.h>
#include <Libraries/kiero/minhook/include/MinHook.h>
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Libraries/Callback.hpp>
#include <Libraries/Il2cpp_Resolver/Utils/VFunc.hpp>
#include <Libraries/PaternScan.hpp>
#include <Core/HooksFunctions.h>
#include <Core/Cheats.h>
#include <Core/InternalGameFunctions.h>

using namespace Variables;

#pragma region ImGui
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
#pragma endregion

static bool FindSigs() {

	Unity::il2cppClass* UnityEngineShaderClass = IL2CPP::Class::Find("UnityEngine.Shader");
	const char* OriginalMethodName = "MethodName";

	Offsets::UnityEngineShader__FindShader_Offset = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(UnityEngineShaderClass, "Find");
	Unity::il2cppClass* YouOriginalClass = IL2CPP::Class::Find("YouOriginalClass");

	Offsets::YouOriginalClass__MethodName_Offset = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(YouOriginalClass, OriginalMethodName);

	if (DEBUG) {
		Utils::Log(Offsets::UnityEngineShader__FindShader_Offset - SDK::GameAssembly, "Find");
		Utils::Log(Offsets::YouOriginalClass__MethodName_Offset - SDK::GameAssembly, OriginalMethodName);
	}

	return true;
}

static void EnableHooks() {

	// YouOriginalClass__MethodName
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(
		Offsets::YouOriginalClass__MethodName_Offset),
		&HooksFunctions::YouOriginalClass__MethodName_hook,
		(LPVOID*)&HooksFunctions::YouOriginalClass__MethodName) == MH_OK)
	{
		MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::YouOriginalClass__MethodName_Offset));
	}

}

static void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

static void InitVars() {
	if (IL2CPP::Initialize(true)) {
		printf("[ %s ] Il2Cpp initialized\n", Prefix);
	}
	else {
		printf("[ %s ] Il2Cpp initialize failed, quitting...", Prefix);
		Sleep(300);
		exit(0);
	}
	SDK::Base = (uintptr_t)GetModuleHandleA(NULL);
	printf("[ %s ] Base Address: 0x%llX\n", Prefix, SDK::Base);
	SDK::GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
	printf("[ %s ] GameAssembly Base Address: 0x%llX\n", Prefix, SDK::GameAssembly);
	SDK::UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
	printf("[ %s ] UnityPlayer Base Address: 0x%llX\n", Prefix, SDK::UnityPlayer);
	printf("----------------------------------------------------------\n");
	printf("\n");
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (CheatMenuVariables::ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (CheatMenuVariables::ShowMenu)
		return true;

	return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
}

static HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
#pragma region Attach+Init
	void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

	if (!System::init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer = nullptr;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			ImGui::GetIO().Fonts->AddFontDefault();
			ImFontConfig font_cfg;
			font_cfg.GlyphExtraSpacing.x = 1.2;
			gameFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(TTSquaresCondensedBold, 14, 14, &font_cfg);
			ImGui::GetIO().Fonts->AddFontDefault();

			// try to get a default shader
			//CheatVariables::ChamsShader = GameFunctions::UnityEngine_Shader__Find(IL2CPP::String::New("Hidden/Internal-Colored"));

			System::init = true;
		}
		else {
			return oPresent(pSwapChain, SyncInterval, Flags);
		}
	}

	pContext->RSGetViewports(&System::vps, &System::viewport);
	System::ScreenSize = { System::viewport.Width, System::viewport.Height };
	System::ScreenCenter = { System::viewport.Width / 2.0f, System::viewport.Height / 2.0f };
#pragma endregion

#pragma region BeginScene

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#pragma endregion

#pragma region Watermark
	if (CheatMenuVariables::Watermark)
	{
		Utils::DrawOutlinedText(gameFont, ImVec2(System::ScreenCenter.x, System::ScreenSize.y - 20), 13.0f, CheatMenuVariables::RainbowColor, true, Prefix.c_str());
		Utils::DrawOutlinedText(gameFont, ImVec2(System::ScreenCenter.x, 5), 13.0f, CheatMenuVariables::RainbowColor, true, "[ %.1f FPS ]", ImGui::GetIO().Framerate);
	}
#pragma endregion

#pragma region CHEATS

	if (CheatMenuVariables::ShowMenu)
	{
		DrawMouse();

		DrawMenu();
	}

	DrawCrosshair();

	if (CheatMenuVariables::AimbotFOVCheck) {
		ImGui::GetForegroundDrawList()->AddCircle(ImVec2(System::ScreenCenter.x, System::ScreenCenter.y), CheatMenuVariables::AimbotFOV, ImColor(255, 255, 255), 360);
	}

	// Main cheats loop
	try { CheatsLoop(); }
	catch (...) {}

#pragma endregion

#pragma region EndScene
	ImGui::Render();
#pragma endregion

#pragma region Inputs+Deattach
	if (GetAsyncKeyState(KEYS::SHOWMENU_KEY) & 1)
	{
		CheatMenuVariables::ShowMenu = !CheatMenuVariables::ShowMenu;
	}

	if (GetKeyState(KEYS::DEATTACH_KEY) & 1)
	{
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
		CheatMenuVariables::ShowMenu = false;
	}

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	IL2CPP::Thread::Detach(m_pThisThread);

#pragma endregion	

	return oPresent(pSwapChain, SyncInterval, Flags);
}

static void Rainbow() {

	while (true)
	{

#pragma region Side Things
		auto isFrames = ImGui::GetFrameCount();
		static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;
		if (isFrames % 1 == 0)
		{
			if (isGreen == 0.01f && isBlue == 0.0f)
			{
				isRed += 0.01f;

			}
			if (isRed > 0.99f && isBlue == 0.0f)
			{
				isRed = 1.0f;

				isGreen += 0.01f;

			}
			if (isGreen > 0.99f && isBlue == 0.0f)
			{
				isGreen = 1.0f;

				isRed -= 0.01f;

			}
			if (isRed < 0.01f && isGreen == 1.0f)
			{
				isRed = 0.0f;

				isBlue += 0.01f;

			}
			if (isBlue > 0.99f && isRed == 0.0f)
			{
				isBlue = 1.0f;

				isGreen -= 0.01f;

			}
			if (isGreen < 0.01f && isBlue == 1.0f)
			{
				isGreen = 0.0f;

				isRed += 0.01f;

			}
			if (isRed > 0.99f && isGreen == 0.0f)
			{
				isRed = 1.0f;

				isBlue -= 0.01f;

			}
			if (isBlue < 0.01f && isGreen == 0.0f)
			{
				isBlue = 0.0f;

				isRed -= 0.01f;

				if (isRed < 0.01f)
					isGreen = 0.01f;

			}
		}

		GetCursorPos(&System::MousePos);
		ScreenToClient(window, &System::MousePos);

#pragma endregion

#pragma region Rainbow
		CheatMenuVariables::Rainbow = ImVec4(isRed, isGreen, isBlue, 1.0f);
		CheatMenuVariables::RainbowColor = ImColor(CheatMenuVariables::Rainbow.x, CheatMenuVariables::Rainbow.y, CheatMenuVariables::Rainbow.z);
#pragma endregion

		Sleep(50);
	}
}

static void InitChair()
{
	if (DEBUG) {
		Utils::CreateConsole();
		system("cls");
	}
	InitVars();

	IL2CPP::Callback::Initialize();

	FindSigs();
	EnableHooks();

	kiero::bind(8, (void**)&oPresent, hkPresent);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CacheManager, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Rainbow, NULL, NULL, NULL);
}

static DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			InitChair();
			init_hook = true;
			System::initil2cpp = true;
		}
	} while (!init_hook);
	return TRUE;
}

static BOOL WINAPI DllMain(HMODULE mod, DWORD reason, LPVOID lpReserved)
{
	if (reason == 1)
	{
		DisableThreadLibraryCalls(mod);
		CreateThread(nullptr, 0, MainThread, mod, 0, nullptr);
	}
	return TRUE;
}