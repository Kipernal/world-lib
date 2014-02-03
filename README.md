world-lib
=========

Library for interfacing with Super Mario World ROMs and Lunar Magic's hacks.

The purpose of this library is to provide a generic method for grabbing data from SMW ROMs in a way that makes it easy to create an application that works alongside Lunar Magic.

Includes functions mostly related to level data, for example, getting a level's complete palette, regardless of whether or not its palette is overridden.

Also included are graphics decompression routines for the two standard methods SMW uses to compress graphics.

The library is designed to be simple enough to use but also sufficiently powerful.  Here's a quick example that decompresses level 105's FG1 graphics slot:


std::vector<unsigned char> rom = <code to open the SMW ROM into a vector here>
std::vector<unsigned char> result;

int graphicsFile = getLevelSingleGraphicsSlot(rom.begin() + 0x200, rom.end(), 0x105, 4);
// This function returns the graphics file for whatever level you want.
// It doesn't matter if the level is using Super Graphics Bypass or not.

decompressGraphicsFile(rom.begin() + 0x200, rom.end(), std::back_inserter(result), graphicsFile);
// This function decompresses whatever graphics file you ask it to.
// There's another function that will decompress any data you want,
// but this is significantly more convenient as it automatically finds
// the correct tables and such for you based on the file number.

// The decompressed graphics file is stored in "result" thanks to the
// std::back_inserter.								