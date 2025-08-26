#include "include/Globals.hpp"

namespace Globals {
    bool Initialize() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "");
#endif
        Globals::Register();
        return Globals::Execute();
    }
    bool Uninitialize() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "");
#endif
        return Patch::RestorePatches()     &&
               Hooks::RestoreHooks()       &&
               Patch::UnregisterPatches()  &&
               Hooks::UnregisterHooks();
    }

    static bool Execute() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "");
#endif
        return Patch::ExecutePatches() && Hooks::ExecuteHooks();
    }

    static void Register() {
#if UE_BUILD_DEBUG
        LOG_FUNCTION(RC::LogLevel::Verbose, "");
#endif
        Globals::RegisterBitfixPatch();
        // Unused,bitfix patch is enough for now.Uncomment these if the bitfix patch doesn't work
        //Globals::RegisterFPakFileCreatePakReaderPatch();
        //Globals::RegisterFPakFileConstructorHook();
        //Globals::RegisterFPakPlatformFile__MountHook();
    }
}