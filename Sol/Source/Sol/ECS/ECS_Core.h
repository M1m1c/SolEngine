#pragma once
#include <cstdint>
#include <bitset>
namespace Sol 
{
	typedef std::uint32_t Entity;
	static const Entity MAX_ENTITIES = 5000;

	typedef std::uint8_t CompType;
	enum CompType : std::uint8_t
	{
		None = 0,
		Transform = 1,
		Mesh = 2,
	};

	static const CompType MAX_COMPONENTS = 32;
	typedef std::bitset<MAX_COMPONENTS> CompSignature;

	
}