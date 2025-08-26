#include "include/Globals.hpp"
#include "include/Helpers.hpp"
#include <HAL/Platform.hpp>
#include <DynamicOutput/Output.hpp>

static bool hkFPakPlatformFile__Mount(uintptr_t instance, const RC::Unreal::TCHAR* InPakFilename, uint32_t PakOrder, const RC::Unreal::TCHAR* InPath /*= nullptr*/, bool bLoadIndex /*= true*/, uintptr_t OutPakListEntry /*= nullptr*/);

namespace Globals {
	/*
		Patches so it skips these if it's our asset

		if (bPakSuccess && IoDispatcherFileBackend.IsValid())
		{
			FGuid EncryptionKeyGuid = Pak->GetInfo().EncryptionKeyGuid;
			FAES::FAESKey EncryptionKey;

			if (!UE::FEncryptionKeyManager::Get().TryGetKey(EncryptionKeyGuid, EncryptionKey))
			{
	*/
	std::optional<Hooks> g_FPakPlatformFile__Mount_Hook;

	void RegisterFPakPlatformFile__MountHook() {
#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "RegisterFPakPlatformFile__MountHook called");
#endif

		Globals::g_FPakPlatformFile__Mount_Hook.emplace(
			AddressRetrievalType::SIGNATURE,
			reinterpret_cast<uintptr_t>(&hkFPakPlatformFile__Mount),
			0, 0,
			"40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 88 FD FF FF 48 81 EC 78 03 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 68"
		);

		Hooks::RegisterHook(&Globals::g_FPakPlatformFile__Mount_Hook.value());

#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "FPakPlatformFile__Mount hook registered");
#endif
	}
}

static bool hkFPakPlatformFile__Mount(uintptr_t instance, const RC::Unreal::TCHAR* InPakFilename, uint32_t PakOrder, const RC::Unreal::TCHAR* InPath /*= nullptr*/, bool bLoadIndex /*= true*/, uintptr_t OutPakListEntry /*= nullptr*/) {
#if UE_BUILD_DEBUG
	LOG_FUNCTION(RC::LogLevel::Verbose, "hkFPakPlatformFile__Mount called for Pak: {}", InPakFilename);
#endif

	using FPakPlatformFile__MountFn = bool(*)(uintptr_t, const RC::Unreal::TCHAR*, uint32_t, const RC::Unreal::TCHAR*, bool, uintptr_t);

	if (!Globals::g_FPakPlatformFile__Mount_Hook.has_value()) {
#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Error, "g_FPakPlatformFile__Mount_Hook is not set!");
#endif
		return false;
	}

	auto& hook_instance = Globals::g_FPakPlatformFile__Mount_Hook.value();

	if (hook_instance.m_OriginalFunction == 0) {
#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Error, "Original function is null!");
#endif
		return false;
	}

	FPakPlatformFile__MountFn original_function = reinterpret_cast<FPakPlatformFile__MountFn>(hook_instance.m_OriginalFunction);

	bool is_custom_pak = wcsstr(InPakFilename, L"LogicMods") != nullptr;
#if UE_BUILD_DEBUG
	LOG_FUNCTION(RC::LogLevel::Verbose, "Pak detected: {} | CustomPak = {}", InPakFilename, is_custom_pak);
#endif

	// Patch bytes with our unconditional jump
	if (is_custom_pak) {
#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "Patching bytes for custom pak: {}", InPakFilename);
#endif

		uintptr_t current_fn_address = hook_instance.m_iTargetAddress;
		uintptr_t jump_offset = current_fn_address + 0x451;

		const uint8_t data[] = {
			0x90,0xE9,0x05,0x03,0,0
		};

		if (!Helpers::SafeMemoryPatch(jump_offset, data, 6)) {
#if UE_BUILD_DEBUG
			LOG_FUNCTION(RC::LogLevel::Error, "Failed to patch bytes : {}", InPakFilename);
#endif
			return false;
		}

#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "Patch applied successfully: {}", InPakFilename);
#endif
	}

	bool result = original_function(instance, InPakFilename, PakOrder, InPath, bLoadIndex, OutPakListEntry);
#if UE_BUILD_DEBUG
	LOG_FUNCTION(RC::LogLevel::Verbose, "Original Mount result for {} = {}", InPakFilename, result);
#endif

	// Restore original bytes
	if (is_custom_pak) {
#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "Restoring bytes for custom pak: {}", InPakFilename);
#endif

		uintptr_t current_fn_address = hook_instance.m_iTargetAddress;
		uintptr_t jump_offset = current_fn_address + 0x451;

		const uint8_t data[] = {
			0x0F,0x84,0x05,0x03,0,0
		};

		if (!Helpers::SafeMemoryPatch(jump_offset, data, 6)) {
#if UE_BUILD_DEBUG
			LOG_FUNCTION(RC::LogLevel::Error, "Failed to restore bytes : {}", InPakFilename);
#endif
			return false;
		}

#if UE_BUILD_DEBUG
		LOG_FUNCTION(RC::LogLevel::Verbose, "Restore applied successfully: {}", InPakFilename);
#endif
	}

#if UE_BUILD_DEBUG
	LOG_FUNCTION(RC::LogLevel::Verbose, "hkFPakPlatformFile__Mount finished for Pak: {}", InPakFilename);
#endif

	return result;
}
