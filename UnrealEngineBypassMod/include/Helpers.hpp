#pragma once

#include <Windows.h>

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

    inline bool ChangeMemoryProtection(uintptr_t address, size_t size, DWORD new_protect, DWORD& old_protect) {
        if (!VirtualProtect(reinterpret_cast<LPVOID>(address), size, new_protect, &old_protect)) {
            DWORD error = GetLastError();
            return false;
        }
        return true;
    }

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
            return false;
        }
        return true;
    }
}


