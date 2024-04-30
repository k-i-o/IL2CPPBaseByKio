#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>

using namespace Variables;

void DrawMouse() {
	if(!CheatMenuVariables::ShowMouse) return;

	ImColor color = CheatMenuVariables::RainbowMouse ? CheatMenuVariables::RainbowColor : CheatMenuVariables::MouseColor;

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
		ImColor color = CheatMenuVariables::RainbowCrosshair ? CheatMenuVariables::RainbowColor : CheatMenuVariables::CrosshairColor;
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
	ImGuiTheme2();

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
		if (ImGui::Button("Aimbot"))
		{
			tabIndex = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Exploits"))
		{
			tabIndex = 2;
		}
		ImGui::Separator();
		switch (tabIndex) {
			case 0:

				{ // Render Things
					ImGui::Checkbox("Show Watermark", &CheatMenuVariables::Watermark);

					ImGui::Checkbox("Show Inspector", &CheatMenuVariables::ShowInspector);
				}

				{ // Mouse things
					ImGui::Checkbox("Draw mouse", &CheatMenuVariables::ShowMouse);
					ImGui::SameLine();
					ImGui::ColorEdit3("##MouseColor", (float*)&CheatMenuVariables::MouseColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::Checkbox("##RGB1", &CheatMenuVariables::RainbowMouse);
					ImGui::SameLine();
					ImGui::SliderInt("##Mouse type", &CheatMenuVariables::MouseType, 0, 1);
				}

				{ // Crosshair
					ImGui::Checkbox("Crosshair", &CheatMenuVariables::Crosshair);
					ImGui::SameLine();
					ImGui::ColorEdit3("##CrosshairColor", (float*)&CheatMenuVariables::CrosshairColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::SliderFloat("##Crosshair Size", &CheatMenuVariables::CrosshairSize, 0.1f, 10.0f);
					ImGui::SameLine();
					ImGui::Checkbox("##RGB2", &CheatMenuVariables::RainbowCrosshair);
					ImGui::SliderInt("##Crosshair type", &CheatMenuVariables::CrosshairType, 0, 1);
				}

				{ // ESP
					ImGui::Checkbox("Players Snapline", &CheatMenuVariables::PlayersSnapline);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersSnaplineColor", (float*)&CheatMenuVariables::PlayersSnaplineColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::Checkbox("##RGB3", &CheatMenuVariables::RainbowPlayersSnapline);
					ImGui::SliderInt("##PlayersSnaplineType", &CheatMenuVariables::PlayersSnaplineType, 0, 2);
				}

				{ // bot checker
					ImGui::Checkbox("Bot Checker", &CheatMenuVariables::BotChecker);
					ImGui::SameLine();
					ImGui::ColorEdit3("##BotCheckerColor", (float*)&CheatMenuVariables::BotCheckerColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::Checkbox("##RGB4", &CheatMenuVariables::RainbowBotChecker);
					ImGui::SameLine();
					ImGui::Checkbox("Show Bot Text", &CheatMenuVariables::BotCheckerText);
				}

				{ // Players Box
					ImGui::Checkbox("Players Box", &CheatMenuVariables::PlayersBox);
					ImGui::SameLine();
					ImGui::ColorEdit3("##PlayersBoxColor", (float*)&CheatMenuVariables::PlayersBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::Checkbox("##RGB5", &CheatMenuVariables::RainbowPlayersBox);
					ImGui::SameLine();
					ImGui::Checkbox("##Filled", &CheatMenuVariables::PlayersBoxFilled);
				}

				/* { // Players Health
					ImGui::Checkbox("Players Health", &CheatMenuVariables::PlayersHealth);
				}*/

				//{ // Charms
				//	ImGui::Checkbox("Players Chams", &CheatMenuVariables::PlayerChams);
				//}

				/*if(ImGui::Button("UnlockAll")) {
					CheatMenuVariables::UnlockAll = true;
				}*/


				break;
			case 1:

				{ // Aimbot
					ImGui::Checkbox("Enable Aimbot", &CheatMenuVariables::EnableAimbot);
					ImGui::Checkbox("Aimbot FOV Check", &CheatMenuVariables::AimbotFOVCheck);
					ImGui::SliderFloat("Aimbot FOV", &CheatMenuVariables::AimbotFOV, 0.1f, 800.0f);
					ImGui::SliderFloat("Aimbot Smooth", &CheatMenuVariables::AimbotSmoothness, 0.0f, 20.0f);
					ImGui::SliderFloat("Aimbot Height", &CheatMenuVariables::AimbotHeight, 0.0f, 100.0f);

				}
				break;

			case 2:

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
		ImGui::End();
	}
}


void CheatsLoop()
{
	DWORD currentTime = GetTickCount64(); 

	if (!Variables::System::InitIL2Cpp)
		return;

	for (int i = 0; i < CheatVariables::PlayersList.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::PlayersList[i];

		if (!curPlayer)
			continue;

		if (CheatMenuVariables::PlayerChams) { // ATTENTION: some game can crash keeping that in loop enabled

			// MAYBE THERE YOU SHOULD GET ANOTHER COMPONENT INSIDE THE CURRENT PLAYER AND AFTER FOUND THE RENDER LIST FROM THAT SUB GAMEOBJECT

			auto RenderList = curPlayer->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>("GetComponentsInChildren", IL2CPP::Class::GetSystemType(IL2CPP::Class::Find("UnityEngine.Renderer")));

			if (!RenderList)
				continue;

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
				Material->SetPropertyValue<Unity::Color>("color", Unity::Color(CheatMenuVariables::Rainbow.x, CheatMenuVariables::Rainbow.w, CheatMenuVariables::Rainbow.y));
			}
		}

		Unity::Vector3 rootPos = CheatVariables::PlayersList[i]->GetTransform()->GetPosition();
		rootPos.y -= 0.2f;
		Vector2 pos;
		if (Utils::worldtoscreen(rootPos, pos))
		{

			if (CheatMenuVariables::PlayersSnapline)
			{

				ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatMenuVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;
				
				if (CheatMenuVariables::BotChecker) { // ALL OF THAT IS AN EXAMPLE, CAN CRASH IF ENABLED AND NO COMPONENT FOUND
					Unity::CComponent* CPlayer = curPlayer->GetComponent("Player");
					if (!CPlayer) continue;

					if(CPlayer->GetMemberValue<bool>("isbot")) {
						color = CheatMenuVariables::RainbowBotChecker ? CheatMenuVariables::RainbowColor : CheatMenuVariables::BotCheckerColor;
					
						if (CheatMenuVariables::BotCheckerText) {
							Utils::DrawOutlinedTextForeground(gameFont, ImVec2(pos.x, pos.y + 10), 13.0f, color, false, "Bot");
						}
					}
				}

				color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

				ImVec2 screenPos = ImVec2(pos.x, pos.y);
				ImVec2 dest;

				switch(CheatMenuVariables::PlayersSnaplineType)
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
			}

			if (CheatMenuVariables::PlayersHealth) { // edit that by your self, is dirty
				Unity::CComponent* CHealth = curPlayer->GetComponent("Health");
				if (!CHealth) break;

				int health = CHealth->GetMemberValue<int>("health");
				int maxHealth = CHealth->GetMemberValue<int>("maxHealth");

				ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatMenuVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;

				color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

				ImVec2 screenPos = ImVec2(pos.x, pos.y);

				ImVec2 dest = ImVec2(screenPos.x, screenPos.y - 10);

				ImGui::GetForegroundDrawList()->AddText(dest, color, std::to_string(health).c_str());
							
				ImColor hpColor = ImColor(255, 0, 0, 255);
				ImColor hpColor2 = ImColor(0, 255, 0, 255);

				float hpWidth = 50.0f;
				float hpHeight = 10.0f;

				ImVec2 hpTopLeft(pos.x - hpWidth / 2, pos.y - hpHeight / 2);
				ImVec2 hpBottomRight(pos.x + hpWidth / 2, pos.y + hpHeight / 2);

				ImGui::GetBackgroundDrawList()->AddRectFilled(hpTopLeft, hpBottomRight, ImColor(0, 0, 0, 255));
				ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(hpTopLeft.x, hpTopLeft.y), ImVec2(hpTopLeft.x + hpWidth * (health / maxHealth), hpBottomRight.y), ImColor(0, 255, 0, 255));
				ImGui::GetBackgroundDrawList()->AddRect(hpTopLeft, hpBottomRight, ImColor(0, 0, 0, 255));

			}

			if (CheatMenuVariables::PlayersBox) {
				ImColor color = CheatMenuVariables::RainbowPlayersBox ? CheatMenuVariables::RainbowColor : CheatMenuVariables::PlayersBoxColor;

				color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

				float boxWidth = 30;
				float boxHeight = 45;

				ImVec2 boxTopLeft(pos.x - boxWidth / 2, pos.y - boxHeight / 2);
				ImVec2 boxBottomRight(pos.x + boxWidth / 2, pos.y + boxHeight / 2);

				if (CheatMenuVariables::PlayersBoxFilled) {
					color.Value.w = 0.3f;
					ImGui::GetBackgroundDrawList()->AddRectFilled(boxTopLeft, boxBottomRight, color);
					color.Value.w = 1;
					ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
				}
				else {
					ImGui::GetBackgroundDrawList()->AddRect(boxTopLeft, boxBottomRight, color);
				}
			}

			if (CheatMenuVariables::EnableAimbot) {

				Vector2 playerPosToAim = pos;

				if (CheatMenuVariables::AimbotFOVCheck)
				{
					if (playerPosToAim.x > (System::ScreenCenter.x + CheatMenuVariables::AimbotFOV))
						continue;
					if (playerPosToAim.x < (System::ScreenCenter.x - CheatMenuVariables::AimbotFOV))
						continue;
					if (playerPosToAim.y > (System::ScreenCenter.y + CheatMenuVariables::AimbotFOV))
						continue;
					if (playerPosToAim.y < (System::ScreenCenter.y - CheatMenuVariables::AimbotFOV))
						continue;
				}

				if (GetAsyncKeyState(VK_RBUTTON)) {
					if (CheatVariables::TargetPlayer == nullptr) {
						CheatVariables::TargetPlayer = curPlayer;
					}

					if (CheatVariables::TargetPlayer == curPlayer) {
						Utils::mousemove(playerPosToAim.x, playerPosToAim.y-CheatMenuVariables::AimbotHeight, System::ScreenSize.x, System::ScreenSize.y, CheatMenuVariables::AimbotSmoothness);
					}
				} else {
					CheatVariables::TargetPlayer = nullptr;
				}


			}
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

		// here maybe you can check for a localplayer, or edit the Objects Cache method

		Utils::ObjectsCache(&CheatVariables::PlayersList, "UnityEngine.CharacterController");
		
		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(3000);
	}
}