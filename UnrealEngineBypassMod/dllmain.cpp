#include <Windows.h>

#include <Mod/CppUserModBase.hpp>
#include <include/Globals.hpp>

class UnrealEngineBypassMod : public RC::CppUserModBase
{
public:
    UnrealEngineBypassMod() : RC::CppUserModBase() {
        ModName = MOD_NAME;
        ModVersion = MOD_VERSION;
        ModDescription = MOD_DESCRIPTION;
        ModAuthors = MOD_AUTHOR;

#if UE_BUILD_DEBUG
        while (!IsDebuggerPresent()) {
            Sleep(1000);
        }
#endif
        Globals::Initialize();
    }

    ~UnrealEngineBypassMod() override {
        Globals::Uninitialize();
    }
};

// Export functions
extern "C"
{
    __declspec(dllexport) RC::CppUserModBase* start_mod()
    {
        return new UnrealEngineBypassMod();
    }

    __declspec(dllexport) void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}