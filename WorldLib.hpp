#include <vector>
#include <cstdint>

/* General functions for extracting data from a SMW ROM.  Example usage to get level 105's 4th graphics file:

std::vector<unsigned char> rom = <code to open the SMW ROM into a vector here>
std::vector<unsigned char> result;

int graphicsFile = getLevelSingleGraphicsSlot(rom.begin() + 0x200, rom.end(), 0x105, 4);			// Get the 4th ExGFX file for level 105
decompressGraphicsFile(rom.begin() + 0x200, rom.end(), std::back_inserter(result), graphicsFile);		// Decompress that ExGFX file into a vector.  
														// std::back_inserter ensures that memory is allocated automatically




Note that all of these functions expect UNHEADERED ROMs, despite what Lunar Magic wants.
Thus, you'd be wise to stick to the above rom.begin() + 0x200 format (or do "auto romStart = rom.begin() + 0x200" and then just use romStart from there on or something).


*/

#include "Level.hpp"
#include "LunarMagic.hpp"
#include "SFC.hpp"