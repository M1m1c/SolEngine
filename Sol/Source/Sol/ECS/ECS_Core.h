#pragma once
#include <cstdint>
#include <bitset>
namespace Sol 
{
	typedef std::uint32_t EntityID;
	static const EntityID MAX_ENTITIES = 10000;

	//255 is the absolute maximum number of component types we can have using a uint8_t
	typedef std::uint8_t CompType;

	//TODO how do we make this value dynamic so that we can add more than a set number of components types
	//This is the set max number of component types that we allow in the engine.
	static const std::uint8_t MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> CompSignature;
}