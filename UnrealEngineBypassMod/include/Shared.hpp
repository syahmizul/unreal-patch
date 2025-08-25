#pragma once
#include <SigScanner/SinglePassSigScanner.hpp>

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

	Address() : 
		m_retrieval_type(AddressRetrievalType::SIGNATURE),
		m_iBaseAddress(0),
		m_iRelativeAddress(0),
		m_szSignature(),
		m_bIsInitialized(false),
		m_iTargetAddress(0)
	{}
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
		RC::SignatureContainer func_container(
			std::vector<RC::SignatureData>{ {.signature = m_szSignature} },
			// On match
			[&](RC::SignatureContainer& container) -> bool {
				m_iTargetAddress = reinterpret_cast<uintptr_t>(container.get_match_address());
				container.get_did_succeed() = true;
				m_bIsInitialized = true;
				return true;
			},
			// On scan finished
			[&](RC::SignatureContainer& container) -> void {
				0;
			}
		);

		RC::SinglePassScanner::SignatureContainerMap signature_map{
			{
				RC::ScanTarget::Engine,
				{ 
					func_container
				}
			}
		};
		RC::SinglePassScanner::start_scan(signature_map);
		return m_bIsInitialized;
	}

	bool FindAddressByOffset() {
		m_iTargetAddress = m_iBaseAddress + m_iRelativeAddress;
		m_bIsInitialized = true;
		return true;
	}
};