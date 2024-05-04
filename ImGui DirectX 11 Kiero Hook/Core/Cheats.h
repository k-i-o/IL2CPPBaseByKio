#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>

using namespace Variables;

namespace Fns {

	bool RenderChams(Unity::CGameObject* obj) {
		// MAYBE THERE YOU SHOULD GET ANOTHER COMPONENT INSIDE THE CURRENT PLAYER AND AFTER FOUND THE RENDER LIST FROM THAT SUB GAMEOBJECT

		auto RenderList = obj->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>("GetComponentsInChildren", IL2CPP::Class::GetSystemType(IL2CPP::Class::Find("UnityEngine.Renderer")));

		if (!RenderList)
			return false;

		// THERE YOU SHOULD SKIP SOME RENDERERS, LIKE WEAPONS, OR OTHER STUFF

		for (int t = 0; t < RenderList->m_uMaxLength; t++)
		{
			if (!RenderList->operator[](t))
				continue;

			if (!RenderList->operator[](t)->GetPropertyValue<bool>("enabled"))
				continue;

			auto Material = RenderList->operator[](t)->CallMethodSafe<Unity::CComponent*>("GetMaterial");
			if (!Material)
				continue;

			Material->CallMethodSafe<void*>("set_shader", CheatVariables::ChamsShader);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_Cull"), 0);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_ZTest"), 8);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_ZWrite"), 0);
			Material->SetPropertyValue<Unity::Color>("color", Unity::Color(CheatVariables::Rainbow.x, CheatVariables::Rainbow.y, CheatVariables::Rainbow.z, CheatVariables::Rainbow.w));

		}

		return true;
	}

	bool RenderESPSnapline(ImColor color, Vector2 origin) {

		ImVec2 screenPos = ImVec2(origin.x, origin.y);
		ImVec2 dest;

		switch (CheatMenuVariables::PlayersSnaplineType)
		{
		case 0:
			dest = ImVec2(System::ScreenSize.x / 2, System::ScreenSize.y);
			break;
		case 1:
			dest = ImVec2(System::ScreenSize.x / 2, System::ScreenSize.y / 2);
			break;
		case 2:
			dest = ImVec2(System::ScreenSize.x / 2, 0);
			break;
		}
		ImGui::GetForegroundDrawList()->AddLine(dest, screenPos, color, 1.5f);

		return true;
	}

	bool RenderESPBox(ImColor color, Vector2 bottom, Vector2 top) {

		const float height = bottom.y - top.y;
		const float width = height * 0.2f;
		const auto left = static_cast<int>(top.x - width);
		const auto right = static_cast<int>(top.x + width);

		if (CheatMenuVariables::PlayersBoxFilled) {
			color.Value.w = 0.3f;
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(left, top.y), ImVec2(right, bottom.y), color);
			color.Value.w = 1;
			ImGui::GetForegroundDrawList()->AddRect(ImVec2(left, top.y), ImVec2(right, bottom.y), color);
		}
		else
		{
			ImGui::GetForegroundDrawList()->AddRect(ImVec2(left, top.y), ImVec2(right, bottom.y), color, 0.0f, 0, 1.0f);
		}
		
		/*
		float boxWidth = 30;
		float boxHeight = 45;

		ImVec2 boxTopLeft(origin.x - boxWidth / 2, origin.y - boxHeight / 2);
		ImVec2 boxBottomRight(origin.x + boxWidth / 2, origin.y + boxHeight / 2);

		if (CheatMenuVariables::PlayersBoxFilled) {
			color.Value.w = 0.3f;
			ImGui::GetBackgroundDrawList()->AddRectFilled(boxTopLeft, boxBottomRight, color);
			color.Value.w = 1;
			ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
		}
		else {
			ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
		}*/

		return true;
	}

	bool RenderHealthBar(Unity::CGameObject* obj, Vector2 origin) {
		Unity::CComponent* CHealth = obj->GetComponent("Health");
		if (!CHealth) return false;

		int health = CHealth->GetMemberValue<int>("health");
		int maxHealth = CHealth->GetMemberValue<int>("maxHealth");

		ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;

		color = CheatVariables::TargetPlayer == obj ? CheatVariables::TargetPlayerColor : color;

		ImVec2 screenPos = ImVec2(origin.x, origin.y);

		ImVec2 dest = ImVec2(screenPos.x, screenPos.y - 10);

		ImGui::GetForegroundDrawList()->AddText(dest, color, std::to_string(health).c_str());

		ImColor hpColor = ImColor(255, 0, 0, 255);
		ImColor hpColor2 = ImColor(0, 255, 0, 255);

		float hpWidth = 50.0f;
		float hpHeight = 10.0f;

		ImVec2 hpTopLeft(origin.x - hpWidth / 2, origin.y - hpHeight / 2);
		ImVec2 hpBottomRight(origin.x + hpWidth / 2, origin.y + hpHeight / 2);

		ImGui::GetBackgroundDrawList()->AddRectFilled(hpTopLeft, hpBottomRight, ImColor(0, 0, 0, 255));
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(hpTopLeft.x, hpTopLeft.y), ImVec2(hpTopLeft.x + hpWidth * (health / maxHealth), hpBottomRight.y), ImColor(0, 255, 0, 255));
		ImGui::GetBackgroundDrawList()->AddRect(hpTopLeft, hpBottomRight, ImColor(0, 0, 0, 255));

		return true;
	}

	bool ExecAimbot(Unity::CGameObject* target, Vector2 playerHead) {

		if (CheatMenuVariables::AimbotFOVCheck)
		{
			if (playerHead.x > (System::ScreenCenter.x + CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.x < (System::ScreenCenter.x - CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.y > (System::ScreenCenter.y + CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.y < (System::ScreenCenter.y - CheatMenuVariables::AimbotFOV))
				return false;
		}

		if (GetAsyncKeyState(VK_RBUTTON)) {
			if (CheatVariables::TargetPlayer == nullptr) {
				CheatVariables::TargetPlayer = target;
			}

			if (CheatVariables::TargetPlayer == target) {
				Utils::MouseMove(playerHead.x, playerHead.y, System::ScreenSize.x, System::ScreenSize.y, CheatMenuVariables::AimbotSmoothness);
			}
		}
		else {
			CheatVariables::TargetPlayer = nullptr;
		}

		return true;
	}
}

void DrawMouse() {
	if(!CheatMenuVariables::ShowMouse) return;

	ImColor color = CheatMenuVariables::RainbowMouse ? CheatVariables::RainbowColor : CheatMenuVariables::MouseColor;

	switch (CheatMenuVariables::MouseType) {
	case 0:
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), 4, color);
		break;
	case 1:
		Utils::DrawOutlinedTextForeground(gameFont, ImVec2(System::MousePos.x, System::MousePos.y), 13.0f, color, false, "X");
		break;
	case 2:
		if (!ImGui::GetIO().MouseDrawCursor) {
			ImGui::GetIO().MouseDrawCursor = true;
		}
		break;
	}
}

void DrawCrosshair() {
	if (CheatMenuVariables::Crosshair) {
		ImColor color = CheatMenuVariables::RainbowCrosshair ? CheatVariables::RainbowColor : CheatMenuVariables::CrosshairColor;
		switch (CheatMenuVariables::CrosshairType)
		{
		case 0:
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(System::ScreenCenter.x - CheatMenuVariables::CrosshairSize, System::ScreenCenter.y), ImVec2((System::ScreenCenter.x - CheatMenuVariables::CrosshairSize) + (CheatMenuVariables::CrosshairSize * 2), System::ScreenCenter.y), color, 1.2f);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(System::ScreenCenter.x, System::ScreenCenter.y - CheatMenuVariables::CrosshairSize), ImVec2(System::ScreenCenter.x, (System::ScreenCenter.y - CheatMenuVariables::CrosshairSize) + (CheatMenuVariables::CrosshairSize * 2)), color, 1.2f);
			break;
		case 1:
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(System::ScreenCenter.x, System::ScreenCenter.y), CheatMenuVariables::CrosshairSize, color, 100, 1.2f);
			break;
		}
	}
}

void DrawMenu()
{
	Themes::ImGuiThemeKio(true);

	if (CheatMenuVariables::ShowInspector) {
		Utils::DrawInspector();
	}

	if (ImGui::Begin(Prefix.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(600, 300), ImGuiCond_Once);
		static int tabIndex = 0;
		ImGui::SameLine();
		if (ImGui::Button("Visual"))
		{
			tabIndex = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Aim"))
		{
			tabIndex = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Exploits"))
		{
			tabIndex = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Misc"))
		{
			tabIndex = 3;
		}
		ImGui::Separator();
		ImGui::Spacing();
		switch (tabIndex) {
			case 0: {

				{ // ESP
					ImGui::Checkbox("Players Snapline", &CheatMenuVariables::PlayersSnapline);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersSnaplineColor", (float*)&CheatMenuVariables::PlayersSnaplineColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine(); Utils::HelpMarker("Color of the players snapline");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB3", &CheatMenuVariables::RainbowPlayersSnapline);
					ImGui::SameLine(); Utils::HelpMarker("Toggle rainbow color on the players snapline");
					ImGui::Text("Snapline Type");
					ImGui::SameLine();
					ImGui::SliderInt("##PlayersSnaplineType", &CheatMenuVariables::PlayersSnaplineType, 0, 2);
				}

				{ // bot checker
					ImGui::Checkbox("Bot Checker", &CheatMenuVariables::BotChecker);
					ImGui::SameLine();
					ImGui::ColorEdit3("##BotCheckerColor", (float*)&CheatMenuVariables::BotCheckerColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine(); Utils::HelpMarker("Color of the bot checker");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB4", &CheatMenuVariables::RainbowBotChecker);
					ImGui::SameLine(); Utils::HelpMarker("Toggle rainbow color on the bot checker");
					ImGui::SameLine();
					ImGui::Checkbox("Show Bot Text", &CheatMenuVariables::BotCheckerText);
					ImGui::SameLine(); Utils::HelpMarker("Show a text on the bot if the player is a bot");
				}

				{ // Players Box
					ImGui::Checkbox("Players Box", &CheatMenuVariables::PlayersBox);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersBoxColor", (float*)&CheatMenuVariables::PlayersBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine(); Utils::HelpMarker("Color of the players box");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB5", &CheatMenuVariables::RainbowPlayersBox);
					ImGui::SameLine(); Utils::HelpMarker("Toggle rainbow color on the players box");
					ImGui::SameLine();
					ImGui::Checkbox("##Filled", &CheatMenuVariables::PlayersBoxFilled);
					ImGui::SameLine(); Utils::HelpMarker("Toggle filled box on the players box");
				}

				{ // Charms
					ImGui::Checkbox("Players Chams", &CheatMenuVariables::PlayerChams);
				}

				/* { // Players Health
					ImGui::Checkbox("Players Health", &CheatMenuVariables::PlayersHealth);
				}*/

				/*if(ImGui::Button("UnlockAll")) {
					CheatMenuVariables::UnlockAll = true;
				}*/


				break;
			}
			case 1: {

				{ // Aimbot
					ImGui::Text("Aimbot Height");
					ImGui::SameLine();
					ImGui::SliderFloat("##Aim Height", &CheatMenuVariables::AimbotHeight, 0.0f, 100.0f);

					ImGui::Separator();
					ImGui::Spacing();

					ImGui::Checkbox("Enable Aimbot", &CheatMenuVariables::EnableAimbot);
					ImGui::Checkbox("Aimbot FOV Check", &CheatMenuVariables::AimbotFOVCheck);
					ImGui::Text("Aimbot FOV");
					ImGui::SameLine();
					ImGui::SliderFloat("##Aimbot FOV", &CheatMenuVariables::AimbotFOV, 0.1f, 800.0f);
					ImGui::Text("Aimbot Smoothness");
					ImGui::SameLine();
					ImGui::SliderFloat("##Aimbot Smooth", &CheatMenuVariables::AimbotSmoothness, 0.0f, 30.0f);

				}
				break;
			}
			case 2: {

				//{ // GodMode
				//	ImGui::Checkbox("GodMode (for you and bots)", &CheatMenuVariables::GodMode);
				//}

				//{ // No Recoil
				//	ImGui::Checkbox("No Recoil", &CheatMenuVariables::NoRecoil);
				//}

				//{ // No Spread
				//	ImGui::Checkbox("No Spread", &CheatMenuVariables::NoSpread);
				//}

				//{ // One Hit Kill
				//	ImGui::Checkbox("One Hit Kill", &CheatMenuVariables::OneShot);
				//}

				//{ // Rapid Fire
				//	ImGui::Checkbox("Rapid Fire", &CheatMenuVariables::RapidFire);
				//}

				//{ // Infinite Ammo
				//	ImGui::Checkbox("Infinite Ammo", &CheatMenuVariables::InfiniteAmmo);
				//}

				//{ // Speed Hack
				//	ImGui::Checkbox("Speed Hack", &CheatMenuVariables::SpeedHack);
				//	ImGui::SliderFloat("Speed", &CheatMenuVariables::SpeedValue, 0.1f, 1000.0f);
				//}
				break;
			}
			case 3: {

				{ // Render Things
					ImGui::Checkbox("Show Watermark", &CheatMenuVariables::Watermark);
				}

				{ // Mouse things
					ImGui::Checkbox("Draw mouse", &CheatMenuVariables::ShowMouse);
					ImGui::SameLine();
					ImGui::ColorEdit3("##MouseColor", (float*)&CheatMenuVariables::MouseColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine(); Utils::HelpMarker("Color of the mouse");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB1", &CheatMenuVariables::RainbowMouse);
					ImGui::SameLine(); Utils::HelpMarker("Toggle rainbow color on the mouse");
					ImGui::Text("Mouse Type");
					ImGui::SameLine();
					ImGui::SliderInt("##Mouse type", &CheatMenuVariables::MouseType, 0, 1);
				}

				{ // Crosshair
					ImGui::Checkbox("Crosshair", &CheatMenuVariables::Crosshair);
					ImGui::SameLine();
					ImGui::ColorEdit3("##CrosshairColor", (float*)&CheatMenuVariables::CrosshairColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine(); Utils::HelpMarker("Color of the crosshair");
					ImGui::SameLine();
					ImGui::Checkbox("##RGB2", &CheatMenuVariables::RainbowCrosshair);
					ImGui::SameLine(); Utils::HelpMarker("Toggle rainbow color on the crosshair");
					ImGui::Text("Crosshair Size");
					ImGui::SameLine();
					ImGui::SliderFloat("##Crosshair Size", &CheatMenuVariables::CrosshairSize, 0.1f, 10.0f);
					ImGui::Text("Crosshair Type");
					ImGui::SameLine();
					ImGui::SliderInt("##Crosshair type", &CheatMenuVariables::CrosshairType, 0, 1);
				}

				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Text("Developer Options");

				ImGui::Checkbox("Enable Developer Options", &CheatMenuVariables::EnableDeveloperOptions);

				if(CheatMenuVariables::EnableDeveloperOptions)
				{ 
					ImGui::Checkbox("Show Inspector", &CheatMenuVariables::ShowInspector);

					{ // test things
						ImGui::Text("Test Objects");
						ImGui::SameLine();
						ImGui::InputTextWithHint("##SearchObject", "Name of a component...", CheatVariables::TestObjects::Name, 200);

						ImGui::Checkbox("Test Objects Chams", &CheatVariables::TestObjects::Chams);
						ImGui::Checkbox("Test Objects Snapline", &CheatVariables::TestObjects::Snapline);
						ImGui::SameLine();
						ImGui::Checkbox("Test Objects Rainbow Snapline", &CheatVariables::TestObjects::RainbowSnapline);
						ImGui::Checkbox("Test Objects Box", &CheatVariables::TestObjects::Box);
						ImGui::Checkbox("Test Objects Rainbow Box", &CheatVariables::TestObjects::RainbowBox);
						ImGui::Checkbox("Test Objects Aimbot", &CheatVariables::TestObjects::Aimbot);
					}
				}
				break;
			}
		}
		ImGui::End();
	}
}


void CheatsLoop()
{
	DWORD currentTime = GetTickCount64(); 

	if (!Variables::System::InitIL2Cpp) return;

	if (CheatMenuVariables::EnableDeveloperOptions)	{

		for (int i = 0; i < CheatVariables::TestObjects::List.size(); i++) {
			Unity::CGameObject* curObject = CheatVariables::TestObjects::List[i];
			if (!curObject) continue;

			if (CheatVariables::TestObjects::Chams) { // ATTENTION: some game can crash keeping that in loop enabled
				Fns::RenderChams(curObject);
			}

			Unity::Vector3 rootPos = curObject->GetTransform()->GetPosition();
			Unity::Vector3 headPos = rootPos; // HEAD OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE HEAD POSITION
			headPos.y += CheatMenuVariables::AimbotHeight;
			rootPos.y -= 0.2f;

			Vector2 top, bottom;
			if (!Utils::WorldToScreen(rootPos, bottom)) continue;

			if (CheatVariables::TestObjects::Snapline)
			{
				ImColor color = CheatVariables::TestObjects::RainbowSnapline ? CheatVariables::RainbowColor : CheatVariables::TestObjects::SnaplineColor;

				Fns::RenderESPSnapline(color, bottom);
			}

			if (CheatVariables::TestObjects::Box) {
				ImColor color = CheatVariables::TestObjects::RainbowBox ? CheatVariables::RainbowColor : CheatVariables::TestObjects::BoxColor;

				if (!Utils::WorldToScreen(headPos, top)) continue;
				Fns::RenderESPBox(color, bottom, top);
			}

			if (CheatVariables::TestObjects::Aimbot) {
				if (!Utils::WorldToScreen(headPos, top)) continue;
				Fns::ExecAimbot(curObject, top);
			}

		}
	}

	for (int i = 0; i < CheatVariables::PlayersList.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::PlayersList[i];
		if (!curPlayer) continue;

		if (CheatMenuVariables::PlayerChams) { // ATTENTION: some game can crash keeping that in loop enabled
			Fns::RenderChams(curPlayer);
		}

		Unity::Vector3 rootPos = curPlayer->GetTransform()->GetPosition();
		Unity::Vector3 headPos = rootPos; // HEAD OF THE PLAYER <-- THIS TRICK IS USELESS IF YOU KNOW THE HEAD POSITION
		headPos.y += CheatMenuVariables::AimbotHeight;
		rootPos.y -= 0.2f;

		Vector2 top, bottom;
		if (!Utils::WorldToScreen(rootPos, bottom)) continue;

		if (CheatMenuVariables::PlayersSnapline)
		{
			ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;

			if (CheatMenuVariables::BotChecker) { // ALL OF THAT IS AN EXAMPLE, CAN CRASH IF ENABLED AND NO COMPONENT FOUND
				Unity::CComponent* CPlayer = curPlayer->GetComponent("Player");
				if (!CPlayer) continue;

				if (CPlayer->GetMemberValue<bool>("isbot")) {
					color = CheatMenuVariables::RainbowBotChecker ? CheatVariables::RainbowColor : CheatMenuVariables::BotCheckerColor;

					if (CheatMenuVariables::BotCheckerText) {
						Utils::DrawOutlinedTextForeground(gameFont, ImVec2(bottom.x, bottom.y + 10), 13.0f, color, false, "Bot");
					}
				}
			}

			// another color if target of aimbot
			color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

			Fns::RenderESPSnapline(color, bottom);
		}

		if (CheatMenuVariables::PlayersBox) {
			ImColor color = CheatMenuVariables::RainbowPlayersBox ? CheatVariables::RainbowColor : CheatMenuVariables::PlayersBoxColor;

			color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

			if (Utils::WorldToScreen(headPos, top)) {
				Fns::RenderESPBox(color, bottom, top);
			}
		}

		if (CheatMenuVariables::PlayersHealth) { // edit that by your self, is dirty
			Fns::RenderHealthBar(curPlayer, bottom);
		}

		if (CheatMenuVariables::EnableAimbot) {
			if (!Utils::WorldToScreen(headPos, top)) continue;
			Fns::ExecAimbot(curPlayer, top);
		}
	}

	if (currentTime - CheatVariables::LastTick > 5)
	{
		CheatVariables::LastTick = currentTime;
	}
}


void CacheManager()
{
	while (true)
	{
		if (!Variables::System::InitIL2Cpp)
			continue;

		void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
		if (!m_pThisThread) continue;

		// here maybe you can check for a localplayer, or look the Objects Cache method

		if (CheatMenuVariables::EnableDeveloperOptions) {
			Utils::ObjectsCache(&CheatVariables::TestObjects::List, CheatVariables::TestObjects::Name);
		}
		// check the method in Utils.cpp to understand how find localplayer with that variable
		Utils::ObjectsCache(&CheatVariables::PlayersList, "UnityEngine.CharacterController");
		
		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(2000);
	}
}