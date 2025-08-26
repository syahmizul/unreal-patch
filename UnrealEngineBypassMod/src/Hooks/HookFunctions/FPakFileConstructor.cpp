#include "include/Globals.hpp"
#include <HAL/Platform.hpp>
#include <DynamicOutput/Output.hpp>

class IPlatformFile;
static uintptr_t hkFPakFileConstructor(uintptr_t instance, IPlatformFile* LowerLevel, const RC::Unreal::TCHAR* Filename, bool bIsSigned, bool bLoadIndex);

namespace Globals {
    // Patches the bSigned parameter of FPakFile's constructor to be false when it's our custom pak file.
    std::optional<Hooks> g_FPakFileConstructor_Hook;

    void RegisterFPakFileConstructorHook() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "RegisterFPakFileConstructorHook called");
#endif

        Globals::g_FPakFileConstructor_Hook.emplace(
            AddressRetrievalType::SIGNATURE,
            reinterpret_cast<uintptr_t>(&hkFPakFileConstructor),
            0, 0,
            "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 54 41 55 41 56 41 57 48 83 EC 40 48 8D 05 ?? ?? ?? ?? 48"
        );

        Hooks::RegisterHook(&Globals::g_FPakFileConstructor_Hook.value());

#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "FPakFileConstructor hook registered");
#endif
    }

}

static uintptr_t hkFPakFileConstructor(uintptr_t instance, IPlatformFile* LowerLevel, const RC::Unreal::TCHAR* Filename, bool bIsSigned, bool bLoadIndex) {
#if UE_BUILD_DEBUG
    LOG_FUNCTION(RC::LogLevel::Verbose, "hkFPakFileConstructor called for Pak: {}", Filename);
#endif

    using FPakFileConstructorFn = uintptr_t(*)(uintptr_t, IPlatformFile*, const RC::Unreal::TCHAR*, bool, bool);

    if (!Globals::g_FPakFileConstructor_Hook.has_value()) {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Error, "g_FPakFileConstructor_Hook is not set!");
#endif
        return NULL;
    }

    auto& hook_instance = Globals::g_FPakFileConstructor_Hook.value();

    if (hook_instance.m_OriginalFunction == 0) {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Error, "Original FPakFile constructor is null!");
#endif
        return NULL;
    }

    FPakFileConstructorFn original_function = reinterpret_cast<FPakFileConstructorFn>(hook_instance.m_OriginalFunction);

    bool is_custom_pak = wcsstr(Filename, L"LogicMods") != nullptr;
#if UE_BUILD_DEBUG
    LOG_FUNCTION(RC::LogLevel::Verbose, "Pak detected: {} | CustomPak = {}", Filename, is_custom_pak);
#endif

    if (is_custom_pak) {
        bIsSigned = false; // Disable signature check for custom assets
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "bIsSigned overridden to false for LogicMods");
#endif
    }

    uintptr_t result = original_function(instance, LowerLevel, Filename, bIsSigned, bLoadIndex);

#if UE_BUILD_DEBUG
    if (result != 0) {
        LOG_FUNCTION(RC::LogLevel::Verbose, "Constructed successfully: {}", Filename);
    }
    else {
        LOG_FUNCTION(RC::LogLevel::Error, "hkFPakFileConstructor failed to construct: {}", Filename);
    }

    LOG_FUNCTION(RC::LogLevel::Verbose, "hkFPakFileConstructor finished for Pak: {}", Filename);
#endif

    return result;
}
