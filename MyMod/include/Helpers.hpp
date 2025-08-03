#pragma once

namespace Helpers {
    template<typename T>
    inline constexpr T* GetAddressAs(uintptr_t address) {
        return reinterpret_cast<T*>(address);
    }

    template<typename T>
    inline constexpr T GetAddressValue(uintptr_t address) {
        return *reinterpret_cast<T*>(address);
    }

    template<typename T>
    inline constexpr void SetAddressValue(uintptr_t address, T value) {
        *reinterpret_cast<T*>(address) = value;
    }

    // Memory protection helper
    inline bool ChangeMemoryProtection(uintptr_t address, size_t size, DWORD new_protect, DWORD& old_protect) {
        if (!VirtualProtect(reinterpret_cast<LPVOID>(address), size, new_protect, &old_protect)) {
            DWORD error = GetLastError();
            /*LogError("Memory Protection Change",
                std::format("Failed at address 0x{:X}, size {}, error code {}", address, size, error));*/
            return false;
        }
        return true;
    }

    // Safe memory patching
    inline bool SafeMemoryPatch(uintptr_t address, const void* data, size_t size) {
        DWORD old_protect;
        if (!ChangeMemoryProtection(address, size, PAGE_EXECUTE_READWRITE, old_protect)) {
            return false;
        }

        // Apply patch
        memcpy(reinterpret_cast<void*>(address), data, size);

        // Restore protection
        DWORD dummy;
        if (!VirtualProtect(reinterpret_cast<LPVOID>(address), size, old_protect, &dummy)) {
            //LogWarning(operation_name, "Failed to restore memory protection");
            //return false; <-- not necesary a failure,just "unsafe",maybe..
        }

        //LogSuccess(std::format("{} - Patched {} bytes at 0x{:X}", operation_name, size, address));
        return true;
    }

    // Signature scanning helper (original method)
    //bool ScanAndPatch(const PatchConfig& config, std::function<bool(uintptr_t)> patch_callback) {
    //    if (!config.IsValid()) {
    //        LogError(config.name, "Invalid patch configuration");
    //        return false;
    //    }

    //    if (!config.use_signature) {
    //        // Use offset-based approach
    //        uintptr_t target_address = config.GetTargetAddress();
    //        LogInfo(std::format("{} - Using offset method: Base 0x{:X} + Offset 0x{:X} = Target 0x{:X}",
    //            config.name, config.base_address.value(),
    //            static_cast<uintptr_t>(config.relative_offset.value()), target_address));

    //        try {
    //            return patch_callback(target_address);
    //        }
    //        catch (const std::exception& e) {
    //            LogError(config.name, std::format("Exception during offset patch: {}", e.what()));
    //            return false;
    //        }
    //    }

    //    // Use signature scanning approach
    //    LogInfo(std::format("{} - Using signature method: {}", config.name, config.signature.value()));

    //    SignatureData sig_data;
    //    sig_data.signature = config.signature.value();
    //    std::vector<SignatureData> signatures = { sig_data };

    //    bool scan_successful = false;
    //    bool patch_successful = false;

    //    SignatureContainer container(signatures,
    //        // On Match Found
    //        [&](SignatureContainer& container) -> bool {
    //            uintptr_t match_address = reinterpret_cast<uintptr_t>(container.get_match_address());
    //            LogInfo(std::format("{} - Found signature at 0x{:X}", config.name, match_address));

    //            try {
    //                patch_successful = patch_callback(match_address);
    //                scan_successful = true;
    //                container.get_did_succeed() = true;
    //            }
    //            catch (const std::exception& e) {
    //                LogError(config.name, std::format("Exception during signature patch: {}", e.what()));
    //                patch_successful = false;
    //            }

    //            return true;
    //        },
    //        // On Scan Finished
    //        [&](SignatureContainer& container) -> void {
    //            if (!container.get_did_succeed()) {
    //                LogError(config.name, "Signature not found");
    //                scan_successful = false;
    //            }
    //        });

    //    RC::SinglePassScanner::SignatureContainerMap signature_map;
    //    signature_map[ScanTarget::Engine].push_back(container);
    //    SinglePassScanner::start_scan(signature_map);

    //    return scan_successful && patch_successful;
    //}
}


