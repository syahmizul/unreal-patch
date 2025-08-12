#include <include/Globals.hpp>

bool HookFunctions::hkFPakPlatformFile__Mount(uintptr_t instance, const TCHAR* InPakFilename, uint32_t PakOrder, const TCHAR* InPath /*= nullptr*/, bool bLoadIndex /*= true*/, uintptr_t OutPakListEntry /*= nullptr*/) {
	
	using FPakPlatformFile__MountFn = bool(*)(uintptr_t instance, const TCHAR* InPakFilename, uint32_t PakOrder, const TCHAR* InPath /*= nullptr*/, bool bLoadIndex /*= true*/, uintptr_t OutPakListEntry /*= nullptr*/);

	if (!Globals::g_FPakPlatformFile__Mount_Hook.has_value())
		return false;

	auto& hook_instance = Globals::g_FPakPlatformFile__Mount_Hook.value();

	if (hook_instance.m_OriginalFunction == 0)
		return false;

	FPakPlatformFile__MountFn original_function = reinterpret_cast<FPakPlatformFile__MountFn>(hook_instance.m_OriginalFunction);

	bool is_custom_pak = wcsstr(InPakFilename, L"LogicMods") != nullptr;

	// Patch bytes with our unconditional jump
	if (is_custom_pak) {
		uintptr_t current_fn_address = hook_instance.m_iTargetAddress;
		uintptr_t jump_offset = current_fn_address + 0x451;

		const uint8_t data[] = {
			0x90,0xE9,0x05,0x03,0,0
		};

		Helpers::SafeMemoryPatch(jump_offset, data, 6);
	}

	bool result = original_function(instance, InPakFilename, PakOrder, InPath, bLoadIndex, OutPakListEntry);
	
	// Restore original bytes
	if (is_custom_pak) {
		uintptr_t current_fn_address = hook_instance.m_iTargetAddress;
		uintptr_t jump_offset = current_fn_address + 0x451;

		const uint8_t data[] = {
			0x0F,0x84,0x05,0x03,0,0
		};

		Helpers::SafeMemoryPatch(jump_offset, data, 6);
	}
	
	return result;
}