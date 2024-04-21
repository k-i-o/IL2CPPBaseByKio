#pragma once


bool RenderChams(Unity::CGameObject* curPlayer) {
	if (!curPlayer) return false;

	auto RenderList = curPlayer->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>("GetComponentsInChildren", IL2CPP::Class::GetSystemType(IL2CPP::Class::Find("UnityEngine.Renderer")));

	if (!RenderList)
		return false;

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

	return true;
}

bool DrawSnapline(Vector2 pos, Unity::CGameObject* curPlayer) {
	if (!curPlayer) return false;

	ImColor color = CheatMenuVariables::RainbowPlayersSnapline ? CheatMenuVariables::RainbowColor : CheatMenuVariables::PlayersSnaplineColor;

	color = CheatVariables::TargetPlayer == curPlayer ? CheatVariables::TargetPlayerColor : color;

	ImVec2 screenPos = ImVec2(pos.x, pos.y);
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

bool DrawHealthBar(Vector2 pos, Unity::CGameObject* curPlayer) { // dirty
	if (!curPlayer) return false;

	Unity::CComponent* CHealth = curPlayer->GetComponent("Health");
	if (!CHealth) return false;

	int health = CHealth->GetMemberValue<int>("health");
	int maxHealth = CHealth->GetMemberValue<int>("maxHealth");

	printf("Health: %d/%d\n", health, maxHealth);

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

	return true;
}

bool DrawBox(Vector2 pos, Unity::CGameObject* curPlayer) {
	if (!curPlayer) return false;

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

	return true;
}

bool Aimbot(Vector2 playerPosToAim, Unity::CGameObject* curPlayer) {
	if (!curPlayer) return false;

	if (CheatMenuVariables::AimbotFOVCheck)
	{
		if (playerPosToAim.x > (System::ScreenCenter.x + CheatMenuVariables::AimbotFOV))
			return false;
		if (playerPosToAim.x < (System::ScreenCenter.x - CheatMenuVariables::AimbotFOV))
			return false;
		if (playerPosToAim.y > (System::ScreenCenter.y + CheatMenuVariables::AimbotFOV))
			return false;
		if (playerPosToAim.y < (System::ScreenCenter.y - CheatMenuVariables::AimbotFOV))
			return false;
	}

	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (CheatVariables::TargetPlayer == nullptr) {
			CheatVariables::TargetPlayer = curPlayer;
		}

		if (CheatVariables::TargetPlayer == curPlayer) {
			Utils::mousemove(playerPosToAim.x, playerPosToAim.y - 22.0f, System::ScreenSize.x, System::ScreenSize.y, CheatMenuVariables::AimbotSmoothness);
		}
	}
	else {
		CheatVariables::TargetPlayer = nullptr;
	}

	return true;

}
