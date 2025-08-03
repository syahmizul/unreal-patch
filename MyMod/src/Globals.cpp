#include <include/Globals.hpp>

namespace Globals {
    bool Register() {
        Globals::g_MainExe = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

        Globals::g_BitfixPatch = Patch(AddressRetrievalType::SIGNATURE,
            [&](Patch* patch_instance) -> bool {

                uintptr_t first_offset = patch_instance->m_iTargetAddress + 0x37;
                if (GetAddressValue<uint8_t>(first_offset) != 0xE9) {
                    return false;
                }
                uintptr_t second_offset = first_offset + 1;
                int32_t jump_offset = GetAddressValue<int32_t>(second_offset);
                uintptr_t patch_address = first_offset + jump_offset + 5;

                if (patch_address == 0) {
                    return false;
                }

                const uint8_t ret_instruction = 0xC3;
                return patch_instance->PatchMemoryLocation(patch_address, &ret_instruction, 1);
            }, 0, 0,
            "48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 83 EC 28 E8 ?? ?? ?? ?? 48 89 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC");

        if (!Patch::RegisterPatch(&g_BitfixPatch))
            return false;

        g_FPakFile_CreatePakReader_Patch = Patch(AddressRetrievalType::OFFSET,
            [&](Patch* patch_instance) -> bool {
                uintptr_t match_address = patch_instance->m_iTargetAddress;

                const uint8_t patch_count = 3;

                uintptr_t jump_patch_offset[patch_count] = {
                    match_address + 0x2E,
                    match_address + 0x4A,
                    match_address + 0x66
                };

                const uint8_t nop_bytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
                bool all_patches_successful = true;
                for (uintptr_t address : jump_patch_offset) {
                    if (!patch_instance->PatchMemoryLocation(address, nop_bytes, 6)) {
                        all_patches_successful = false;
                    }
                }
                return all_patches_successful;
            }, g_MainExe, 0x2213F50);

        if (!Patch::RegisterPatch(&g_FPakFile_CreatePakReader_Patch))
            return false;

        Globals::g_FPakFileConstructor_Hook.emplace(
            AddressRetrievalType::SIGNATURE,
            reinterpret_cast<uintptr_t>(&HookFunctions::hkFPakFileConstructor),
            0, 0,
            "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 54 41 55 41 56 41 57 48 83 EC 40 48 8D 05 ?? ?? ?? ?? 48"
        );

        if (!Hooks::RegisterHook(&Globals::g_FPakFileConstructor_Hook.value()))
            return false;

        Globals::g_FPakPlatformFile__Mount_Hook.emplace(
            AddressRetrievalType::SIGNATURE,
            reinterpret_cast<uintptr_t>(&HookFunctions::hkFPakPlatformFile__Mount),
            0, 0,
            "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 88 FD FF FF 48 81 EC 78 03 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 68"
        );

        if (!Hooks::RegisterHook(&Globals::g_FPakPlatformFile__Mount_Hook.value()))
            return false;

        return true;
    }
    bool Execute()
    {
        return Patch::ExecutePatches() && Hooks::ExecuteHooks();
    }
    bool Initialize() {
        return Globals::Register() && Globals::Execute();
    }
    bool Uninitialize() {
        return Patch::RestorePatches() && Hooks::RestoreHooks() && Patch::UnregisterPatches() && Hooks::UnregisterHooks();
    }
}

