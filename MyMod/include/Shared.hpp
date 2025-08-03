#pragma once
#include <SigScanner/SinglePassSigScanner.hpp>

using RC::SignatureContainer;
using RC::SignatureData;
using RC::SinglePassScanner;
using RC::ScanTarget;

enum AddressRetrievalType {
	SIGNATURE,
	OFFSET,
	//ABSOLUTE //Redundant,just use offset and offset relative to 0 and set the offset as the absolute address
};

class Address {
public:
	AddressRetrievalType m_retrieval_type;

	uintptr_t		m_iBaseAddress;
	uintptr_t		m_iRelativeAddress;

	std::string		m_szSignature;

	bool		m_bIsInitialized;
	uintptr_t	m_iTargetAddress;

	Address() {}
	Address(AddressRetrievalType retrieval_type, uintptr_t base_address = NULL, uintptr_t relative_address = NULL, std::string signature = "") :
		m_retrieval_type(retrieval_type),
		m_iBaseAddress(base_address),
		m_iRelativeAddress(relative_address),
		m_szSignature(signature),
		m_bIsInitialized(false),
		m_iTargetAddress(0)
	{}
	~Address(){}

public:
	bool FindAddress() {
		switch (m_retrieval_type)
		{
			case AddressRetrievalType::SIGNATURE: {
				return FindAddressBySignature();
			}
			case AddressRetrievalType::OFFSET: {
				return FindAddressByOffset();
			}
			default:
				return false;
		}
		return false;
	}

private:
	bool FindAddressBySignature() {

		SignatureContainer func_container(
			std::vector<SignatureData>{ {.signature = m_szSignature} },
			// On match
			[&](SignatureContainer& container) -> bool {
				m_iTargetAddress = reinterpret_cast<uintptr_t>(container.get_match_address());
				container.get_did_succeed() = true;
				m_bIsInitialized = true;
				return true;
			},
			// On scan finished
			[&](SignatureContainer& container) -> void {
				0;
			}
		);

		SinglePassScanner::SignatureContainerMap signature_map{
			{
				ScanTarget::Engine,
				{ 
					func_container
				}
			}
		};
		SinglePassScanner::start_scan(signature_map);
		return m_bIsInitialized;
	}

	bool FindAddressByOffset() {
		// its just adding the base address and relative address lol,maybe refactor this later on
		m_iTargetAddress = m_iBaseAddress + m_iRelativeAddress;
		m_bIsInitialized = true;
		return true;
	}
};