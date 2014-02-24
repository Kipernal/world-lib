world-lib
=========

Header-only library for interfacing with Super Mario World ROMs and Lunar Magic's hacks.

The purpose of this library is to provide a generic method for grabbing data from SMW ROMs in a way that makes it easy to create an application that works alongside Lunar Magic.

Includes functions mostly related to level data, for example, getting a level's complete palette, regardless of whether or not its palette is bypassed.

Also included are graphics decompression routines for the two standard methods SMW uses to compress graphics.

The library is designed to be simple enough to use but also sufficiently powerful.  Here's a quick example that loads a ROM and saves the graphics file used for level 105's SP1 as a PNG.  See world-lib-net as well, which wraps most world-lib functions.

````C++
using namespace worldlib;	// I am not advocating using using.  Just for the record.  
				// But it makes things easier to read here.

int main(int argc, char* argv[])
{
	// The following is an example that opens a ROM and outputs level 105's SP1 graphics as a bitmap.

	// First we open the ROM as a vector.
	// This library can work with almost datatype you want, from vectors to arrays to something 
	// insane like linked lists.
	// This example will stick to vectors, though.

	std::vector<unsigned char> rom;
	std::ifstream in("smw.smc", std::ios::in | std::ios::binary);
	if (in)	rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	else	return 0;


	// Just define some convenience variables.
	// getROMStart gets the start of the ROM data regardless of whether or not it's headered.
	auto romStart = getROMStart(rom.begin(), rom.end());
	auto romEnd = rom.end();


	// Make sure the ROM is valid for editing purposes.
	// See the declaration for exactly what it checks.
	if (checkROMValid(romStart, romEnd) == false) return 0;


	// Now let's get level 105's palette as a vector.

	std::vector<std::uint32_t> palette;
	getLevelPalette(romStart, romEnd, std::back_inserter(palette), 0x0105);

	// std::back_inserter(palette) just tells the function how to insert data into the vector.
	// Generally you'll want to use back_inserter with a vector on most outputs.
	// This will save you the trouble of having to guess the right output buffer size.
	// Either way, palette now has level 105's palette, minus the BG color.
	// It doesn't matter if the palette is custom or not--it should always work as expected.



	// Now let's get the missing BG color.  Not necessary--just for fun.
	std::uint32_t bgColor = getLevelBackgroundColor(romStart, romEnd, 0x0105);
	// Pretty simple.




	// Now let's get the graphics slot used for SP1 in level 105
	int sp1SlotNumber = getLevelSingleGraphicsSlot(romStart, romEnd, 0x0105, GFXSlots::SP1);
	// Done.  sp1SlotNumber is probably 0 right now.  If the level uses ExGFX it could be 
	// something different.



	// Now let's get and decompress that graphics file.
	std::vector<unsigned char> sp1chr;
	decompressGraphicsFile(romStart, romEnd, std::back_inserter(sp1chr), sp1SlotNumber);
	// sp1chr now contains the decompressed graphics data, automatically decompressed using 
	// the ROM's compression format.




	// Now let's turn the indexed graphics into a normal RGBA bitmap.
	// This function's kinda complicated, though. It needs a lot of information.
	std::vector<unsigned char> sp1bmp;
	int width = 0, height = 0;

	indexedImageToBitmap(sp1chr.begin(), sp1chr.end(),   // We need the graphics data
			     palette.begin(), palette.end(), // We need palette data
			     0x10,                            // Tiles per row (0x10 is usually always fine)
			     4,                               // The BPP (2 or 4.  Maybe 8 rarely)
			     0xA,                             // The palette row to use.  This is the "yellow sprite" palette.
			     ColorBackInserter(sp1bmp, ColorOrder::RGBA), // Like back_inserter, but lets you choose the order the colors are stored in.
			     &width, &height);               // We'd like to know the dimensions of the resulting bitmap.

	// sp1bmp now contains a raw string of RGBA data for you to use however you want.  
	// For example:
	lodepng::encode("105 SP1 graphics.png", sp1bmp, width, height);
	// That's an external library, obviously, but it shows what you can do with the data output.

	
	// And that's it!  There are more functions than just these, obviously.
	// Also check out, for example, the patching functions, which you may find useful.
}

````
