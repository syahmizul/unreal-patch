#pragma once

#include <functional>

// Stores the original bytes
class PatchMap {
public:
	uintptr_t	address;
	size_t		replace_size;
	std::vector<uint8_t> replace_bytes;
	std::vector<uint8_t> original_bytes;
};

class Patch : public Address {
public:
	using PatchFn = std::function<bool(Patch*)>;
	PatchFn m_PatchRoutine;
	std::vector<PatchMap> m_vecPatchMap;
	Patch() {}
	~Patch() {}

	Patch(AddressRetrievalType retrieval_type, PatchFn patch_routine, uintptr_t base_address = NULL, uintptr_t relative_address = NULL, std::string signature = "") :
		m_PatchRoutine(patch_routine),
		Address(retrieval_type, base_address, relative_address, signature) {}

	bool PatchMemoryLocation(uintptr_t address, const void* data, size_t size);

	bool Execute();
	bool Restore();
	static bool RegisterPatch(Patch* patch_instance);
	static bool UnregisterPatches();

	static bool ExecutePatches();
	static bool RestorePatches();

	inline static std::vector<Patch*> g_Patches;
};