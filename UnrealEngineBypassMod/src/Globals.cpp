#include "include/Globals.hpp"

namespace Globals {
    bool Initialize() {
        Globals::Register();
        return Globals::Execute();
    }
    bool Uninitialize() {
        return Patch::RestorePatches()     &&
               Hooks::RestoreHooks()       &&
               Patch::UnregisterPatches()  &&
               Hooks::UnregisterHooks();
    }

    static bool Execute() {
        return Patch::ExecutePatches() && Hooks::ExecuteHooks();
    }

    static void Register() {
        Globals::RegisterBitfixPatch();
        // Unused,bitfix patch is enough for now.Uncomment these if the bitfix patch doesn't work
        //Globals::RegisterFPakFileCreatePakReaderPatch();
        //Globals::RegisterFPakFileConstructorHook();
        //Globals::RegisterFPakPlatformFile__MountHook();
    }
}