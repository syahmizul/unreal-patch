#include <include/Globals.hpp>

bool Hooks::RegisterHook(Hooks* hook_instance) {
    g_Hooks.push_back(hook_instance);
    return true;
}

bool Hooks::UnregisterHooks() {
    RestoreHooks();
    g_Hooks.clear();
    return true;
}

bool Hooks::Execute() {
    if (!FindAddress() || !m_bIsInitialized || m_iTargetAddress == 0 || m_HookFunction == 0)
        return false;
    m_Detour = std::make_unique<PLH::x64Detour>(
        m_iTargetAddress,
        m_HookFunction,
        &m_OriginalFunction
    );

    if (!m_Detour)
        return false;

    auto result = m_Detour->hook();

    if (m_OriginalFunction == 0)
        return false;

    return result;
}

bool Hooks::Restore() {
    if (!(m_bIsInitialized || m_Detour))
        return false;
    return m_Detour->unHook();
}



bool Hooks::ExecuteHooks() {
    bool bIsSuccessful = true;
    for (int i = 0; i < g_Hooks.size(); ++i) {
        Hooks* hooks_instance = g_Hooks[i];
        if (!hooks_instance->Execute())
            bIsSuccessful = false;
    }
    return bIsSuccessful;
}

bool Hooks::RestoreHooks() {
    bool bIsSuccessful = true;
    for (int i = 0; i < g_Hooks.size(); ++i) {
        Hooks* hooks_instance = g_Hooks[i];
        if (!hooks_instance->Restore())
            bIsSuccessful = false;
    }
    return bIsSuccessful;
}
