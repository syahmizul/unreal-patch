#include "include/Globals.hpp"
#include "include/Helpers.hpp"

namespace Globals {
    // Patch to bypass signature check for our custom asset files.
    // Credit : https://gist.github.com/Buckminsterfullerene02/90077ce81c0fd908144498869f4ea288
    Patch g_BitfixPatch;

    void RegisterBitfixPatch() {
        Globals::g_BitfixPatch = Patch(AddressRetrievalType::SIGNATURE,
            [&](Patch* patch_instance) -> bool {

                uintptr_t first_offset = patch_instance->m_iTargetAddress + 0x37;
                if (Helpers::GetAddressValue<uint8_t>(first_offset) != 0xE9) {
                    return false;
                }
                uintptr_t second_offset = first_offset + 1;
                int32_t jump_offset = Helpers::GetAddressValue<int32_t>(second_offset);
                uintptr_t patch_address = first_offset + jump_offset + 5;
                if (patch_address == 0) {
                    return false;
                }

                const uint8_t ret_instruction = 0xC3;
                return patch_instance->PatchMemoryLocation(patch_address, &ret_instruction, 1);
            }, 0, 0,
            "48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 83 EC 28 E8 ?? ?? ?? ?? 48 89 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC");

        Patch::RegisterPatch(&g_BitfixPatch);
    }
}