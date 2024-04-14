#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>
#include <Utils/Rendering.hpp>

using namespace Variables;

void DrawMouse() {
	if(!CheatMenuVariables::ShowMouse) return;

	ImColor color = CheatMenuVariables::RainbowMouse ? CheatMenuVariables::RainbowColor : CheatMenuVariables::MouseColor;

	switch (CheatMenuVariables::MouseType) {
	case 0:
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), 4, color);
		break;
	case 1:
		//Render::DrawOutlinedTextForeground(gameFont, ImVec2(System::MousePos.x, System::MousePos.y), 13.0f, color, false, "X");
		break;
	case 2:
		if (!ImGui::GetIO().MouseDrawCursor) {
			ImGui::GetIO().MouseDrawCursor = true;
		}
		break;
	}
}


void DrawMenu()
{
	ImGuiTheme2();
	Utils::DrawInspector();
	static int page = 0;
	if (ImGui::Begin(Prefix.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(375, 400), ImGuiCond_Once);
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
			ImGui::Checkbox("Draw mouse", &CheatMenuVariables::ShowMouse);
			ImGui::SameLine();
			ImGui::ColorEdit3("##MouseColor", (float*)&CheatMenuVariables::MouseColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::Checkbox("##RGB3", &CheatMenuVariables::RainbowMouse);
			ImGui::SameLine();
			ImGui::SliderInt("##Mouse type", &CheatMenuVariables::MouseType, 0, 1);
		}
		ImGui::End();
	}
}


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

		Unity::Vector3 rootPos = CheatVariables::PlayersList[i]->GetTransform()->GetPosition();
		rootPos.y -= 0.2f;
		Vector2 pos;
		if (Utils::worldtoscreen(rootPos, pos))
		{

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