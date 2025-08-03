#include <Mod/CppUserModBase.hpp>
#include <include/Globals.hpp>

class MyMod : public CppUserModBase
{
public:
    MyMod() : CppUserModBase() {
        ModName = STR("MyMod");
        ModVersion = STR("1.0");
        ModDescription = STR("Unreal Engine signature bypass.");
        ModAuthors = STR("Syahmi Zulkalnain");

        Globals::Initialize();
    }
    ~MyMod() override {
        Globals::Uninitialize();
    }

private:

    //bool BypassAesKeyAndSignaturesCheck() {
    //    // ===== PATCH CONFIGURATION - MODIFY AS NEEDED =====

    //    // Method 1: Signature scanning (original)
    //    auto config = PatchConfig::WithSignature("AES Key & Signature Bypass",
    //        "48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 83 EC 28 E8 ?? ?? ?? ?? 48 89 05 ?? ?? ?? ?? 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 48 8D 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC");

    //    // Method 2: Offset-based (uncomment and set your offset)
    //    // auto config = PatchConfig::WithOffset("AES Key & Signature Bypass", engine_base, 0x1234567);

    //    return ScanAndPatch(config, [this](uintptr_t match_address) -> bool {
    //        uintptr_t first_offset = match_address + 0x37;

    //        if (GetAddressValue<uint8_t>(first_offset) != 0xE9) {
    //            LogError("AES Bypass", std::format("Expected 0xE9 at offset 0x{:X}, found 0x{:02X}",
    //                first_offset, GetAddressValue<uint8_t>(first_offset)));
    //            return false;
    //        }

    //        uintptr_t second_offset = first_offset + 1;
    //        int32_t jump_offset = GetAddressValue<int32_t>(second_offset);
    //        uintptr_t patch_address = first_offset + jump_offset + 5;

    //        if (patch_address == 0) {
    //            LogError("AES Bypass", "Calculated patch address is null");
    //            return false;
    //        }

    //        LogInfo(std::format("AES Bypass - Patching address 0x{:X} with RET instruction", patch_address));

    //        const uint8_t ret_instruction = 0xC3;
    //        return SafeMemoryPatch(patch_address, &ret_instruction, 1, "AES Bypass RET Patch");
    //        });
    //}

    //bool PatchFPakFileCreatePakReader() {
    //    // ===== PATCH CONFIGURATION - MODIFY AS NEEDED =====

    //    // Method 1: Signature scanning (original)
    //    /*auto config = PatchConfig::WithSignature("FPakFile::CreatePakReader Patch",
    //        "4C 8B DC 4D 89 43 18 49 89 53 10 53 55 57 48");*/

    //    // Method 2: Offset-based (uncomment and set your offset)
    //     auto config = PatchConfig::WithOffset("FPakFile::CreatePakReader Patch", engine_base, 0x2213F50);

    //    return ScanAndPatch(config, [this](uintptr_t match_address) -> bool {
    //        // Define jump locations relative to match address
    //        const std::vector<std::pair<uintptr_t, std::string>> jump_locations = {
    //            {match_address + 0x2E, "Jump 1"},
    //            {match_address + 0x4A, "Jump 2"},
    //            {match_address + 0x66, "Jump 3"}
    //        };

    //        const uint8_t nop_bytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    //        bool all_patches_successful = true;

    //        for (const auto& [address, name] : jump_locations) {
    //            LogInfo(std::format("Patching {} at 0x{:X}", name, address));

    //            if (!SafeMemoryPatch(address, nop_bytes, 6, std::format("FPakFile::CreatePakReader {}", name))) {
    //                all_patches_successful = false;
    //                LogError("FPakFile::CreatePakReader Patch", std::format("Failed to patch {}", name));
    //            }
    //        }

    //        return all_patches_successful;
    //        });
    //}

    //bool HookFPakFileConstructor() {
    //    // ===== PATCH CONFIGURATION - MODIFY AS NEEDED =====

    //    // Method 1: Signature scanning (original)
    //    auto config = PatchConfig::WithSignature("FPakFile Constructor Hook",
    //        "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 54 41 55 41 56 41 57 48 83 EC 40 48 8D 05 ?? ?? ?? ?? 48");

    //    // Method 2: Offset-based (uncomment and set your offset)
    //    // auto config = PatchConfig::WithOffset("FPakFile Constructor Hook", engine_base, 0x3456789);

    //    return ScanAndPatch(config, [this](uintptr_t match_address) -> bool {
    //        try {
    //            LogInfo(std::format("Setting up FPakFile Constructor detour hook at 0x{:X}", match_address));

    //            FPakFileConstructor_Detour = std::make_unique<PLH::x64Detour>(
    //                std::bit_cast<uint64_t>(match_address),
    //                std::bit_cast<uint64_t>(&hkFPakFileConstructor),
    //                std::bit_cast<uint64_t*>(&originalFPakFileConstructor));

    //            if (!FPakFileConstructor_Detour->hook()) {
    //                LogError("FPakFile Constructor Hook", "Detour hook failed");
    //                return false;
    //            }

    //            LogInfo(std::format("FPakFile Constructor Hook installed - Original function at 0x{:X}", originalFPakFileConstructor));
    //            return true;

    //        }
    //        catch (const std::exception& e) {
    //            LogError("FPakFile Constructor Hook", std::format("Exception: {}", e.what()));
    //            return false;
    //        }
    //        });
    //}

    //bool HookFPakPlatformFile__Mount_IoDispatcherFileBackend_IsValid() {
    //    /*if (bPakSuccess && IoDispatcherFileBackend.IsValid()) <-- Patches so this gets skipped since we only have a .pak file for our BP mod.
    //    {
    //        FGuid EncryptionKeyGuid = Pak->GetInfo().EncryptionKeyGuid;
    //        FAES::FAESKey EncryptionKey;
    //    */

    //    /*
    //        sub_142220DF0+443  000 E8 78 C8 F5 FE                          call    sub_14117DAB0   ; Call Procedure
    //        sub_142220DF0+443
    //        sub_142220DF0+448
    //        sub_142220DF0+448                              loc_142221238:                          ; CODE XREF: sub_142220DF0+441↑j
    //        sub_142220DF0+448  000 49 83 BC 24 D0 00 00 00                 cmp     qword ptr [r12+0D0h], 0 ; Compare Two Operands
    //        sub_142220DF0+448  000 00
    //        sub_142220DF0+451  000 0F 84 05 03 00 00                       jz      loc_14222154C   ; Jump if Zero (ZF=1) <-- This jump here is the culprit
    //        sub_142220DF0+451
    //        sub_142220DF0+457  000 41 0F 10 86 A8 00 00 00                 movups  xmm0, xmmword ptr [r14+0A8h] ; Move Unaligned Four Packed Single-FP
    //        sub_142220DF0+45F  000 0F 57 C9                                xorps   xmm1, xmm1      ; Bitwise Logical XOR for Single-FP Data
    //    */

    //    // Method 1: Signature scanning (original)
    //    auto config = PatchConfig::WithSignature("FPakPlatformFile::Mount Hook",
    //        "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 88 FD FF FF 48 81 EC 78 03 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 68");
    //    

    //    // Method 2: Offset-based (uncomment and set your offset)
    //    //auto config = PatchConfig::WithOffset("FPakPlatformFile::Mount Patch", engine_base, 0x2213F50);
    //    
    //    return ScanAndPatch(config, [this](uintptr_t match_address) -> bool {
    //        // Define jump locations relative to match address

    //        //bool is_patch_successful = true;
    //        //const uint8_t patch_bytes[] = { 0x90,0xE9,0x05,0x03,0x0,0x0 }; // jump unconditionally

    //        //std::string name = "IoDispatcherFileBackend_IsValid";

    //        //LogInfo(std::format("Patching {} at 0x{:X}", name, match_address));

    //        //if (!SafeMemoryPatch(match_address, patch_bytes, 6, std::format("{} patch", name))) {
    //        //    is_patch_successful = false;
    //        //    LogError(std::format("{} patch", name), std::format("Failed to patch {}", name));
    //        //}

    //        //return is_patch_successful;
    //        try {
    //            LogInfo(std::format("Setting up FPakPlatformFile::Mount hook at 0x{:X}", match_address));

    //            FPakPlatformFile__Mount = std::make_unique<PLH::x64Detour>(
    //                std::bit_cast<uint64_t>(match_address),
    //                std::bit_cast<uint64_t>(&hkFPakPlatformFile__Mount),
    //                std::bit_cast<uint64_t*>(&originalFPakPlatformFile__Mount));

    //            if (!FPakPlatformFile__Mount->hook()) {
    //                LogError("FPakPlatformFile::Mount Hook", "Detour hook failed");
    //                return false;
    //            }

    //            LogInfo(std::format("FPakPlatformFile::Mount Hook installed - Original function at 0x{:X}", originalFPakPlatformFile__Mount));
    //            return true;

    //        }
    //        catch (const std::exception& e) {
    //            LogError("FPakPlatformFile::Mount Hook", std::format("Exception: {}", e.what()));
    //            return false;
    //        }

    //    });
    //}

    

};

// Export functions
extern "C"
{
    __declspec(dllexport) CppUserModBase* start_mod()
    {
        while (!IsDebuggerPresent())
            Sleep(1000);
        return new MyMod();
    }

    __declspec(dllexport) void uninstall_mod(CppUserModBase* mod)
    {
        delete mod;
    }
}