#pragma once

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Utils/SDK.h>
#include <Core/Variables.h>

using namespace Variables;

namespace GameFunctions {
	Unity::CCamera* GetMainCamera()
	{
		Unity::CCamera* (UNITY_CALLING_CONVENTION get_main)() = nullptr;
		return reinterpret_cast<decltype(get_main)>(SDK::GameAssembly + 0x0)();
	}

	UnityEngine_Shader_o* UnityEngine_Shader__Find(Unity::System_String* name)
	{
		UnityEngine_Shader_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*) = nullptr;
		return reinterpret_cast<decltype(t)>(Offsets::UnityEngineShader__FindShader_Offset)(name);
	}

	float UnityEngine_Time__get_timeScale()
	{
		float (UNITY_CALLING_CONVENTION t)() = nullptr;
		return reinterpret_cast<decltype(t)>(Offsets::UnityEngineTime__GetTimeScale_Offset)();
	}

	void UnityEngine_Time__set_timeScale(float value)
	{
		void (UNITY_CALLING_CONVENTION t)(float) = nullptr;
		reinterpret_cast<decltype(t)>(Offsets::UnityEngineTime__SetTimeScale_Offset)(value);
	}

}