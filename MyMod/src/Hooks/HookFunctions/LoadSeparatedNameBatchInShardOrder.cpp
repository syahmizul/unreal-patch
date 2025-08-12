#include <include/Globals.hpp>
#include <include/UnrealTypes/FDisplayNameEntryId.hpp>

#include <Unreal/Core/Containers/Array.hpp>
using namespace RC::Unreal;
uintptr_t HookFunctions::hkLoadSeparatedNameBatchInShardOrder(TArray<FDisplayNameEntryId>& Out,const uint64* Hashes,uintptr_t Headers,uintptr_t Strings) {
	using LoadSeparatedNameBatchInShardOrderFn = uintptr_t(*)(TArray<FDisplayNameEntryId>& Out, const uint64* Hashes, uintptr_t Headers, uintptr_t Strings);

	if (!Globals::g_LoadSeparatedNameBatchInShardOrder_Hook.has_value())
		return false;

	auto& hook_instance = Globals::g_LoadSeparatedNameBatchInShardOrder_Hook.value();

	if (hook_instance.m_OriginalFunction == 0)
		return false;

	LoadSeparatedNameBatchInShardOrderFn original_function = reinterpret_cast<LoadSeparatedNameBatchInShardOrderFn>(hook_instance.m_OriginalFunction);

	auto result = original_function(Out, Hashes, Headers, Strings);

	const wchar_t* names_to_add[] = {
		L"/Game/Mods/MyMod",
		L"/Game/Mods/MyMod/ModActor",
		L"ModActor",
		L"ModActor_C"
	};

	for (const auto& name : names_to_add) {
		FName asset_name = FName(name);
		FDisplayNameEntryId display_entry_id = FDisplayNameEntryId(asset_name);
		Out.Add(display_entry_id);
	}

	return result;
}