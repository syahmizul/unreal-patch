#pragma once

#pragma push_macro("ensure")
#undef ensure
#include <polyhook2/Detour/x64Detour.hpp>
#pragma pop_macro("ensure")
#include "include/Shared.hpp"
class Hooks : public Address {
public:
	uintptr_t m_HookFunction;
	uintptr_t m_OriginalFunction;
	std::unique_ptr<PLH::x64Detour> m_Detour;
	Hooks() {}
	~Hooks() {}

	Hooks(AddressRetrievalType retrieval_type, uintptr_t hook_function,uintptr_t base_address = NULL, uintptr_t relative_address = NULL,std::string signature = "") :
		m_HookFunction(hook_function),
		m_OriginalFunction(0),
		m_Detour(nullptr),
		Address(retrieval_type, base_address, relative_address, signature) {}

	bool Execute();
	bool Restore();

	__forceinline static void RegisterHook(Hooks* hook_instance) {
		g_Hooks.push_back(hook_instance);
	}
	static bool UnregisterHooks() {
		if (!RestoreHooks())
			return false;
		g_Hooks.clear();
		return true;
	};

	static bool ExecuteHooks();
	static bool RestoreHooks();

	inline static std::vector<Hooks*> g_Hooks;
};