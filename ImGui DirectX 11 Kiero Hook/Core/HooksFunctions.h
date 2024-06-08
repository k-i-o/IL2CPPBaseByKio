#pragma once

// includes
#include <string>
#include <intrin.h>

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>

using namespace Variables;

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