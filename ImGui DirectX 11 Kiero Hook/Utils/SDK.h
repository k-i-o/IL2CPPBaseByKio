#pragma once

// includes
#include <vector>
#include <utility> // for std::pair

// my includes
#include <DumpedFiles/il2cpp.h>
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>
#include <Libraries/Vectors/vec2.h>
#include <Libraries/Vectors/vec.h>



namespace mem
{
	template<typename T> T read(uintptr_t address) {
		try { return *(T*)address; }
		catch (...) { return T(); }
	}

	template<typename T> void write(uintptr_t address, T value) {
		try { *(T*)address = value; }
		catch (...) { return; }
	}
}

namespace SDK
{
	uintptr_t Base;
	uintptr_t GameAssembly;
	uintptr_t UnityPlayer;

}

