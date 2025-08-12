#pragma once

#define NOMINMAX

#include <cstdint>
#include <string>
#include <vector>

#include <Windows.h>

#include <include/Shared.hpp>

#include <include/Hooks/Hooks.hpp>
#include <include/Hooks/HookFunctions.hpp>
#include <include/Patch/Patch.hpp>

#include <include/Helpers.hpp>
using namespace Helpers;

namespace Globals {
	bool Register();
	bool Execute();
	bool Initialize();
	bool Uninitialize();

    inline uintptr_t g_MainExe;

    // https://gist.github.com/Buckminsterfullerene02/90077ce81c0fd908144498869f4ea288
    inline Patch g_BitfixPatch;

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
    inline Patch g_FPakFile_CreatePakReader_Patch;

    // Patches the bSigned parameter of FPakFile's constructor to be false when it's our custom pak file.
    inline std::optional<Hooks> g_FPakFileConstructor_Hook;

    /*
        Patches so it skips these if it's our asset

        if (bPakSuccess && IoDispatcherFileBackend.IsValid())
        {
            FGuid EncryptionKeyGuid = Pak->GetInfo().EncryptionKeyGuid;
            FAES::FAESKey EncryptionKey;

            if (!UE::FEncryptionKeyManager::Get().TryGetKey(EncryptionKeyGuid, EncryptionKey))
            {
    
    */
    inline std::optional<Hooks> g_FPakPlatformFile__Mount_Hook;

    // Hooks the function that reads AssetRegistry.bin(contained in asset files like .utoc/.ucas/.pak,I think) to add in our asset name
    inline std::optional<Hooks> g_LoadSeparatedNameBatchInShardOrder_Hook;
}