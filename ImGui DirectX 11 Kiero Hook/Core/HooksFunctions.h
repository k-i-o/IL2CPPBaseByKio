#pragma once

// includes
#include <string>
#include <intrin.h>

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Libraries/Vectors/vec.h>
#include <Libraries/Vectors/vec2.h>



// DELETE THAT IF DO YOU WANT ITS JUST AN EXAMPLE
class FieldExample
{
public:
	float speed;
	float health;
};

class ExampleClass
{
public: 
	FieldExample fields;
};


namespace HooksFunctions
{
	void(UNITY_CALLING_CONVENTION YouOriginalClass__MethodName)(ExampleClass*);
	void YouOriginalClass__MethodName_hook(ExampleClass* _this)
	{
		if (_this != nullptr) {
			// if SuperSpeed is enabled for example
			_this->fields.speed = 100.0f;

			// if GodMode is enabled for example
			_this->fields.health = 10000.0f;
		}

		return YouOriginalClass__MethodName(_this);
	}
}