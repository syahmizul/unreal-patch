#pragma once
#include <CoreFwd.hpp>

using namespace RC::Unreal;

namespace HookFunctions {
	uintptr_t	hkFPakFileConstructor(uintptr_t instance, IPlatformFile* LowerLevel,const TCHAR* Filename, bool bIsSigned, bool bLoadIndex);
	bool		hkFPakPlatformFile__Mount(uintptr_t instance, const TCHAR* InPakFilename, uint32_t PakOrder, const TCHAR* InPath /*= nullptr*/, bool bLoadIndex /*= true*/, uintptr_t OutPakListEntry /*= nullptr*/);
}