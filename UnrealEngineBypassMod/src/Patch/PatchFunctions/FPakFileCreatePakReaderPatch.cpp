#include "include/Globals.hpp"

namespace Globals {
    /*
        bool bNeedsDecryptor = false;
        if (FPlatformProperties::RequiresCookedData())
        {
            bool bShouldCheckSignature = bSigned || FParse::Param(FCommandLine::Get(), TEXT("signedpak")) || FParse::Param(FCommandLine::Get(), TEXT("signed"));
    #if !UE_BUILD_SHIPPING
            bShouldCheckSignature &= !FParse::Param(FCommandLine::Get(), TEXT("FileOpenLog"));
    #endif
            if (bShouldCheckSignature)
            {
                bNeedsDecryptor = true;
            }
        }

        This patch patches jump that would occur if bShouldCheckSignature/bNeedsDecryptor is true
    */
	Patch g_FPakFile_CreatePakReader_Patch;
	void RegisterFPakFileCreatePakReaderPatch() {
        Globals::g_FPakFile_CreatePakReader_Patch = Patch(AddressRetrievalType::OFFSET,
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
            }, Globals::g_MainExeAddress, 0x2213F50);

        Patch::RegisterPatch(&Globals::g_FPakFile_CreatePakReader_Patch);
	}
}