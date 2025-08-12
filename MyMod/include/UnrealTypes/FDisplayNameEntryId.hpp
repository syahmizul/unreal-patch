#pragma once

#include <include/Globals.hpp>
#include <NameTypes.hpp>

/**
 * Serialization util that optimizes WITH_CASE_PRESERVING_NAME-loading by reducing comparison id lookups
 *
 * Stores 32-bit display entry id with an unused bit to indicate if FName::GetComparisonIdFromDisplayId lookup is needed.
 *
 * Note that only display entries should be saved to make output deterministic.
 */
class FDisplayNameEntryId
{
public:
	FDisplayNameEntryId() : FDisplayNameEntryId(FName()) {}
	explicit FDisplayNameEntryId(FName Name) : FDisplayNameEntryId(FNameEntryId::FromUnstableInt(Name.GetDisplayIndex()), FNameEntryId::FromUnstableInt(Name.GetComparisonIndex())) {}
private:
	FNameEntryId Id;
	FDisplayNameEntryId(FNameEntryId InId, FNameEntryId) : Id(InId) {}
};