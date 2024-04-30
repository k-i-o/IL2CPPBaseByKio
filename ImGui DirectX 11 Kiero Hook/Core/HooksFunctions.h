#pragma once

// includes
#include <string>
#include <intrin.h>

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Libraries/Vectors/vec.h>
#include <Libraries/Vectors/vec2.h>

using namespace Variables;

namespace GameFunctions
{
	UnityEngine_Shader_o* UnityEngine_Shader__Find(Unity::System_String* name)
	{
		UnityEngine_Shader_o* (UNITY_CALLING_CONVENTION t)(Unity::System_String*);
		return reinterpret_cast<decltype(t)>(Offsets::UnityEngineShader__FindShader_Offset)(name);
	}
}

namespace HooksFunctions
{
	// EXAMPLE
	//void(UNITY_CALLING_CONVENTION Health__TakeDamage)(Health_o*, int32_t, UnityEngine_Vector3_o, System_String_o*, uint8_t, System_String_o*);
	//void Health__TakeDamage_hook(Health_o* _this, int32_t _damage, UnityEngine_Vector3_o _position, System_String_o* _name, uint8_t _weaponSpriteIndex, System_String_o* steamIdData)
	//{
	//	if (_this != nullptr) {

	//		if(CheatMenuVariables::GodMode) _damage = 0;
	//	}

	//	return Health__TakeDamage(_this, _damage, _position, _name, _weaponSpriteIndex, steamIdData);
	//}
}