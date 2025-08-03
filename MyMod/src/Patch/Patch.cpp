#include <include/Globals.hpp>

bool Patch::RegisterPatch(Patch* patch_instance) {
    g_Patches.push_back(patch_instance);
    return true;
}

bool Patch::UnregisterPatches() {
    RestorePatches();
    g_Patches.clear();
    return true;
}

bool Patch::Execute() {
    return this->FindAddress() && this->m_PatchRoutine(this);
}

bool Patch::Restore() {
    bool allSuccess = true;

    // Iterate through all patches in reverse order (LIFO - Last In, First Out)
    // This helps maintain consistency if patches overlap
    for (auto it = m_vecPatchMap.rbegin(); it != m_vecPatchMap.rend(); ++it) {
        const PatchMap& patch = *it;

        // Restore the original bytes at the patched address
        bool success = Helpers::SafeMemoryPatch(
            patch.address,
            patch.original_bytes.data(),
            patch.original_bytes.size()
        );

        if (!success) {
            allSuccess = false;
            // Continue trying to restore other patches even if one fails
        }
    }

    // Clear the patch map since all patches have been restored
    m_vecPatchMap.clear();

    return allSuccess;
}

bool Patch::ExecutePatches() {
    bool bIsSuccessful = true;
    for (int i = 0; i < g_Patches.size(); ++i) {
        Patch* patch_instance = g_Patches[i];
        if (!patch_instance->Execute())
            bIsSuccessful = false;
    }
    return bIsSuccessful;
}

bool Patch::RestorePatches() {
    bool bIsSuccessful = true;
    for (int i = 0; i < g_Patches.size(); ++i) {
        Patch* patch_instance = g_Patches[i];
        if (!patch_instance->Restore())
            bIsSuccessful = false;
    }
    return bIsSuccessful;
}

bool Patch::PatchMemoryLocation(uintptr_t address, const void* data, size_t size) {
    PatchMap newPatchMap;
    newPatchMap.address = address;
    newPatchMap.replace_size = size;

    std::vector<uint8_t>& original_bytes = newPatchMap.original_bytes;
    original_bytes.resize(size);
    std::memcpy(original_bytes.data(), reinterpret_cast<void*>(address), size);

    std::vector<uint8_t>& replace_bytes = newPatchMap.replace_bytes;
    replace_bytes.resize(size);
    std::memcpy(replace_bytes.data(), data, size);

    m_vecPatchMap.push_back(newPatchMap);

    return Helpers::SafeMemoryPatch(address, data, size);
}