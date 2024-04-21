#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>
#include <Libraries/Fonts/Font.h>

using namespace Variables;

void DrawMouse() {
	if (!CheatMenuVariables::ShowMouse) return;

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
	static int page = 0;
	if (ImGui::Begin(Prefix.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(550, 300), ImGuiCond_Once);
		static int tabb = 0;
		ImGui::SameLine();
		if (ImGui::Button("Visual"))
		{
			tabb = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Aimbot"))
		{
			tabb = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Exploits"))
		{
			tabb = 2;
		}
		ImGui::Separator();
		switch (tabb) {
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

		{ // Players Box
			ImGui::Checkbox("Players Box", &CheatMenuVariables::PlayersBox);
			ImGui::SameLine();
			ImGui::ColorEdit3("##PlayersBoxColor", (float*)&CheatMenuVariables::PlayersBoxColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::Checkbox("##RGB5", &CheatMenuVariables::RainbowPlayersBox);
			ImGui::SameLine();
			ImGui::Checkbox("##Filled", &CheatMenuVariables::PlayersBoxFilled);
		}

		{ // Players Health
			ImGui::Checkbox("Players Health", &CheatMenuVariables::PlayersHealth);
		}

		{ // Charms
			ImGui::Checkbox("Players Chams", &CheatMenuVariables::PlayerChams);
		}

		break;
		case 1:

		{ // Aimbot
			ImGui::Checkbox("Enable Aimbot", &CheatMenuVariables::EnableAimbot);
			ImGui::Checkbox("Aimbot FOV Check", &CheatMenuVariables::AimbotFOVCheck);
			ImGui::SliderFloat("Aimbot FOV", &CheatMenuVariables::AimbotFOV, 0.1f, 800.0f);
			ImGui::SliderFloat("Aimbot Smooth", &CheatMenuVariables::AimbotSmoothness, 0.0f, 20.0f);
		}
		break;

		case 2:
		break;
		}
		ImGui::End();
	}
}

#include "CheatFns.h"
void CheatsLoop()
{
	DWORD currentTime = GetTickCount64(); // Get current time in milliseconds

	if (!Variables::System::initil2cpp)
		return;

	for (int i = 0; i < CheatVariables::PlayersList.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::PlayersList[i];

		if (!curPlayer)
			continue;

		if (CheatMenuVariables::PlayerChams) {
			RenderChams(curPlayer);
		}

		Unity::Vector3 rootPos = CheatVariables::PlayersList[i]->GetTransform()->GetPosition();
		rootPos.y -= 0.2f;
		Vector2 pos;
		if (Utils::worldtoscreen(rootPos, pos))
		{

			if (CheatMenuVariables::PlayersSnapline)
			{
				DrawSnapline(pos, curPlayer);
			}

			/*if (CheatMenuVariables::PlayersHealth) {

				if(DEBUG) {
					printf("Health Fn: %b\n", DrawHealthBar(pos, curPlayer));
				} else {
					DrawHealthBar(pos, curPlayer);
				}
			}*/

			if (CheatMenuVariables::PlayersBox) {
				DrawBox(pos, curPlayer);
			}

			if (CheatMenuVariables::EnableAimbot) {
				Aimbot(pos, curPlayer);
			}
		}
	}

	if (currentTime - CheatVariables::lasttick > 5) //  5 ms Timer For Whatever you want to do
	{
		CheatVariables::lasttick = currentTime;
	}
	
}


void CacheManager()
{
	while (true)
	{
		if (!Variables::System::initil2cpp)
			continue;

		void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
		if (!m_pThisThread) continue;

		Utils::ObjectsCache(&CheatVariables::PlayersList, "UnityEngine.CharacterController"); // That's a generic name for players you can change it 
		//ObjectCache(&TestList, "ANOTHER OBJECT EXAMPLE);
		
		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(1000);
	}
}