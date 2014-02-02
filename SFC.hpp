#pragma once

// Contains various functions for working with the SFC.

namespace worldlib
{

	// Returns true if the ROM has uses the SA-1.  Note that most functions use this automatically in their routines
	// to determine the addressing to use.
	template <typename inputIteratorType>
	bool romUsesSA1(inputIteratorType romStart, inputIteratorType romEnd);

	// Converts a SFC address to a PC one (assuming no header).
	// The ROM is required to determine if it is using SA-1.
	// No other mappings are supported.
	template <typename inputIteratorType>
	inline int SFCToPC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	// Converts a PC address to a SFC one (assuming no header).
	// The ROM is required to determine if it is using SA-1.
	// No other mappings are supported.
	template <typename inputIteratorType>
	inline int PCToSFC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	// Converts a SFC color to an ARBG one.
	inline std::uint32_t SFCToARGB(std::uint16_t color);

	// Converts an ARGB color to a SFC one.  Alpha is discarded.
	inline std::uint32_t ARGBToSFC(std::uint16_t color);
}

#include "SFC.inl"