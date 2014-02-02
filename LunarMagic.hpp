#pragma once

namespace worldlib
{

	// Returns true if this library's version matches Lunar Magic's version.
	// If it returns false, you should warn the user about possible data corruption if you use any features that require compliance with hardcoded LM values.
	template <typename inputIteratorType>
	bool checkROMVersion(inputIteratorType romStart, inputIteratorType romEnd);

	// Returns true if the ROM has been modified with Lunar Magic and expanded.
	// Generally you'll want to bail out if this returns false, as most functions won't work unless Lunar Magic's applied its hacks to the ROM.
	template <typename inputIteratorType>
	bool checkROMModified(inputIteratorType romStart, inputIteratorType romEnd);

	// Returns the current tag Lunar Magic uses on the ROM.  General format seems to be:
	// "Lunar Magic Version 2.21 Public ©2013 FuSoYa, Defender of Relm http://fusoya.eludevisibility.org                                "
	// Probably won't need to use this, since checkROMModified and checkROMVersion should provide the same functionality.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLunarMagicString(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out);

}

#include "LunarMagic.inl"