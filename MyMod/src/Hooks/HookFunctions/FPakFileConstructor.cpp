#include <include/Globals.hpp>
using namespace RC::Unreal;
uintptr_t HookFunctions::hkFPakFileConstructor(uintptr_t instance, IPlatformFile* LowerLevel,const TCHAR* Filename, bool bIsSigned, bool bLoadIndex) {
    using FPakFileConstructorFn = uintptr_t(*)(uintptr_t instance, IPlatformFile* LowerLevel, const TCHAR* Filename, bool bIsSigned, bool bLoadIndex);
    
    if (!Globals::g_FPakFileConstructor_Hook.has_value())
        return false;

    auto& hook_instance = Globals::g_FPakFileConstructor_Hook.value();

    if (hook_instance.m_OriginalFunction == 0)
        return false;

    FPakFileConstructorFn original_function = reinterpret_cast<FPakFileConstructorFn>(hook_instance.m_OriginalFunction);
    
    
    // Log PAK file loading attempts
    std::wstring filename_str(Filename);
    bool is_custom_pak = wcsstr(Filename, L"LogicMods") != nullptr;

    if (is_custom_pak) {
        bIsSigned = false; // Disable signature check for custom assets
    }

    return original_function(instance, LowerLevel, Filename, bIsSigned, bLoadIndex);

    //auto oFunc = PLH::FnCast(originalFPakFileConstructor, FPakFileConstructorFn());
    //uintptr_t result = oFunc(instance, LowerLevel, Filename, bIsSigned, bLoadIndex);

    //if (result != 0) {
    //    Output::send<LogLevel::Verbose>(STR("MyMod [HOOK]: Successfully loaded PAK: {}\n"), Filename);
    //}
    //else {
    //    Output::send<LogLevel::Warning>(STR("MyMod [HOOK]: Failed to load PAK: {}\n"), Filename);
    //}

    //return result;
}