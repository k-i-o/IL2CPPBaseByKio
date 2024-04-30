#pragma once

// includes
#include <Windows.h>
#include <string>
#include <sstream>

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Libraries/Vectors/vec2.h>
#include <Libraries/Dumper.hpp>
#include <Core/Variables.h>
#include <Utils/SDK.h>



namespace Utils
{
	void CreateConsole() {
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		SetConsoleTitle(Variables::Prefix.c_str());
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
	}


	bool worldtoscreen(Unity::Vector3 world, Vector2& screen)
	{
		Unity::CCamera* CameraMain = Unity::Camera::GetMain();
		if (!CameraMain) {
			return false;
		}

		Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>("WorldToScreenPoint", world, Unity::eye::mono);

		if (buffer.x > Variables::System::ScreenSize.x || buffer.y > Variables::System::ScreenSize.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0)
		{
			return false;
		}

		if (buffer.z > 0.0f) 
		{
			screen = Vector2(buffer.x, Variables::System::ScreenSize.y - buffer.y);
		}

		if (screen.x > 0 || screen.y > 0)
		{
			return true;
		}
	}


	void mousemove(float tarx, float tary, float X, float Y, int smooth)
	{
		float ScreenCenterX = (X / 2);
		float ScreenCenterY = (Y / 2);
		float TargetX = 0;
		float TargetY = 0;

		smooth = smooth + 3;

		if (tarx != 0)
		{
			if (tarx > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - tarx);
				TargetX /= smooth;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (tarx < ScreenCenterX)
			{
				TargetX = tarx - ScreenCenterX;
				TargetX /= smooth;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}

		if (tary != 0)
		{
			if (tary > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - tary);
				TargetY /= smooth;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (tary < ScreenCenterY)
			{
				TargetY = tary - ScreenCenterY;
				TargetY /= smooth;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);
	}


	void Log(uintptr_t address, const char* name) {
		printf("[ LOG ] %s: 0x%llX\n", name, address);
	}


	float GetDistance(Unity::Vector3 a, Unity::Vector3 b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	}


	Unity::CGameObject* GetNearestPlayer(std::vector<Unity::CGameObject*> list, Unity::CGameObject* localplayer)
	{
		Unity::CGameObject* nearestPlayer = nullptr;
		float nearestDistance = FLT_MAX;

		for (int i = 0; i < list.size(); i++)
		{
			Unity::CGameObject* currentPlayer = list[i];

			if (!currentPlayer)
				continue;

			float distance = GetDistance(localplayer->GetTransform()->GetPosition(), currentPlayer->GetTransform()->GetPosition());
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestPlayer = currentPlayer;
			}
		}

		return nearestPlayer;
	}

	// imgui helper functions
	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("[?]");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void AddText(ImVec2 pos, char* text, ImColor color)
	{
		auto DrawList = ImGui::GetForegroundDrawList();
		auto wText = text;

		auto Size = ImGui::CalcTextSize(wText);
		pos.x -= Size.x / 2.f;
		pos.y -= Size.y / 2.f;

		//	ImGui::PushFont(m_font);

		DrawList->AddText(ImVec2(pos.x + 1, pos.y + 1), ImColor(0, 0, 0, 255), wText);
		DrawList->AddText(ImVec2(pos.x, pos.y), color, wText);

		//	ImGui::PopFont();
	}

	float DrawOutlinedText(ImFont* pFont, const ImVec2& pos, float size, ImU32 color, bool center, const char* text, ...)
	{
		va_list(args);
		va_start(args, text);

		CHAR wbuffer[256] = { };
		vsprintf_s(wbuffer, text, args);

		va_end(args);

		auto DrawList = ImGui::GetBackgroundDrawList();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, wbuffer);

			if (center)
			{
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);

				DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}
			else
			{
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);

				DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}

	float DrawOutlinedTextForeground(ImFont* pFont, const ImVec2& pos, float size, ImU32 color, bool center, const char* text, ...)
	{
		va_list(args);
		va_start(args, text);

		CHAR wbuffer[256] = { };
		vsprintf_s(wbuffer, text, args);

		va_end(args);

		auto DrawList = ImGui::GetForegroundDrawList();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, wbuffer);

			if (center)
			{
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);

				DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}
			else
			{
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);

				DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}


	void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
	}


	void HealthBar(float x, float y, float w, float h, int phealth, ImColor col)
	{
		auto vList = ImGui::GetBackgroundDrawList();

		int healthValue = max(0, min(phealth, 100));

		int barColor = ImColor(min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255), 25, 255);
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), col);
		RectFilled(x, y, x + w, y + (((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
	}

	void DrawInspector() {
		ImGui::SetNextWindowSize(ImVec2(600.000f, 1000.000f), ImGuiCond_Once);
		if (!ImGui::Begin("Inspector", nullptr, 2)) {
			ImGui::End();
			return;
		}

		static std::vector<std::string> components;
		static std::vector<std::string> classes;
		static std::vector<std::string> methods;
		static std::string current_comp = "";

		ImGui::Text("Components");
		if (ImGui::Button("Update##comp")) {
			components = Dumper::DumpComponentsString();
		}
		ImGui::SetNextItemWidth(150.000f);
		static int component_current_idx = 0; // Here we store our selection data as an index.
		static ImGuiTextFilter c_filter;
		c_filter.Draw("Search##compfilter");
		if (ImGui::BeginListBox("##Components", ImVec2(-FLT_MIN, 200)))
		{
			for (size_t n = 0; n < components.size(); n++)
			{
				if (!c_filter.PassFilter(components[n].c_str())) {
					continue;
				}
				const bool comp_is_selected = (component_current_idx == (int)n);
				if (ImGui::Selectable(components[n].c_str(), comp_is_selected))
					component_current_idx = (int)n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (comp_is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::Spacing();
		ImGui::Text("Classes");
		if (ImGui::Button("Update##class")) {
			classes = Dumper::DumpClassesString(components[component_current_idx]);
			current_comp = components[component_current_idx];
		}

		ImGui::SetNextItemWidth(150.000f);
		static int class_current_idx = 0; // Here we store our selection data as an index.
		static ImGuiTextFilter cl_filter;
		cl_filter.Draw("Search##classfilter");
		if (ImGui::BeginListBox("##Class", ImVec2(-FLT_MIN, 200)))
		{
			for (size_t n = 0; n < classes.size(); n++)
			{
				if (!cl_filter.PassFilter(classes[n].c_str())) {
					continue;
				}
				const bool class_is_selected = (class_current_idx == (int)n);
				if (ImGui::Selectable(classes[n].c_str(), class_is_selected)) {
					class_current_idx = (int)n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (class_is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::Spacing();
		ImGui::Text("Methods");
		if (ImGui::Button("Update##Methods")) {
			methods = Dumper::DumpMethodsString(current_comp, classes[class_current_idx]);
		}

		ImGui::SetNextItemWidth(150.000f);
		static int method_current_idx = 0; // Here we store our selection data as an index.
		static ImGuiTextFilter me_filter;
		me_filter.Draw("Search##methodfilter");
		if (ImGui::BeginListBox("##Methods", ImVec2(-FLT_MIN, 200)))
		{
			for (size_t n = 0; n < methods.size(); n++)
			{
				if (!me_filter.PassFilter(methods[n].c_str())) {
					continue;
				}
				const bool meth_is_selected = (method_current_idx == (int)n);
				if (ImGui::Selectable(methods[n].c_str(), meth_is_selected))
					method_current_idx = (int)n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (meth_is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::End();
	}

	void ObjectsCache(std::vector<Unity::CGameObject*>* originalList, const char* CName)
	{
		originalList->clear();

		auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(CName);
		if (!list) return;
		for (int i = 0; i < list->m_uMaxLength; i++)
		{
			if (!list->operator[](i)) {
				continue;
			}

			originalList->push_back(list->operator[](i)->GetGameObject());
		}

	}
}