#include <Mod/CppUserModBase.hpp>
#include <include/Globals.hpp>

class UnrealEngineBypassMod : public RC::CppUserModBase
{
public:
    UnrealEngineBypassMod() : RC::CppUserModBase() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "Initializing mod");
#endif

        ModName = MOD_NAME;
        ModVersion = MOD_VERSION;
        ModDescription = MOD_DESCRIPTION;
        ModAuthors = MOD_AUTHOR;

#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "Waiting for debugger");
        while (!IsDebuggerPresent()) {
            Sleep(1000);
        }
#endif

        bool bInitResult = Globals::Initialize();
#if UE_BUILD_DEBUG
        if (bInitResult) {
            LOG_FUNCTION(RC::LogLevel::Verbose, "Mod initialized");
        }
        else {
            LOG_FUNCTION(RC::LogLevel::Verbose, "Mod failed to initialize");
        }
#endif
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