world-lib
=========

Header-only library for interfacing with Super Mario World ROMs and Lunar Magic's hacks.

The purpose of this library is to provide a generic method for grabbing data from SMW ROMs in a way that makes it easy to create an application that works alongside Lunar Magic.

Includes functions mostly related to level data, for example, getting a level's complete palette, regardless of whether or not its palette is bypassed.

Also included are graphics decompression routines for the two standard methods SMW uses to compress graphics.

The library is designed to be simple enough to use but also sufficiently powerful.  Here's a quick example that decompresses level 105's sprites graphics slots and saves them to a PNG:

````C++
int main(int argc, char* argv[])
{
	std::vector<unsigned char> rom;

	// Read in the ROM.
	std::ifstream in("smw.smc", std::ios::in | std::ios::binary);
	if (in) rom = std::vector<unsigned char>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	else return 0;

	auto romStart = rom.begin() + 0x200;
	auto romEnd = rom.end();

	// Get level 105's sprite graphics slots
	int sp1 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, 0x105, worldlib::ExgfxSlots::SP1);
	int sp2 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, 0x105, worldlib::ExgfxSlots::SP2);
	int sp3 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, 0x105, worldlib::ExgfxSlots::SP3);
	int sp4 = worldlib::getLevelSingleGraphicsSlot(romStart, romEnd, 0x105, worldlib::ExgfxSlots::SP4);

	// Get level 105's palette
	std::vector<std::uint32_t> palette;
	worldlib::getLevelPalette(romStart, romEnd, std::back_inserter(palette), 0x105);

	// Decompress those slots' graphics files
	std::vector<unsigned char> sp1Chr, sp2Chr, sp3Chr, sp4Chr;
	worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(sp1Chr), sp1);
	worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(sp2Chr), sp2);
	worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(sp3Chr), sp3);
	worldlib::decompressGraphicsFile(romStart, romEnd, std::back_inserter(sp4Chr), sp4);


	// Turn them from indexed, tiled data to normal ARGB bitmaps, and save them all in one bitmap.
	std::vector<unsigned char> spriteBitmap;
	int bitmapWidth, bitmapHeight;
	int totalHeight = 0;

	worldlib::indexedImageToBitmap(sp1Chr.begin(), sp1Chr.end(), palette.begin(), palette.end(), 0x10, 4, 0xA, worldlib::ColorBackInserter(spriteBitmap, worldlib::ColorOrder::RGBA), &bitmapWidth, &bitmapHeight);

	// It looks like a monster function, but that's just because it needs a lot of information about how to render the file.
	// In order:	the start of the graphics file to decode,
	//		the end of the graphics file to decode,
	//		the start of the palette to use,
	//		the end of the palette to use,
	//		the number of tiles per row (generally just keep this at 16),
	//		the bpp (usually 4),
	//		the numerical palette to use (the one here is the "yellow" sprite palette),
	//		the iterator to output the data to (this one lets you choose between 8-bit and 32-bit output),
	//		the place to store the resulting bitmap's width to,
	//		the place to store the resulting bitmap's height to.

	// If you follow this format, it's pretty straightforward.
	// There's also another version that just grabs a specific area of the bitmap, and a version that just grabs any number of specific tiles.

  // Now just get the other 3 slots.  Each is appended to spriteBitmap by ColorBackInserter.
	totalHeight += bitmapHeight;
	worldlib::indexedImageToBitmap(sp2Chr.begin(), sp2Chr.end(), palette.begin(), palette.end(), 0x10, 4, 0xA, worldlib::ColorBackInserter(spriteBitmap, worldlib::ColorOrder::RGBA), &bitmapWidth, &bitmapHeight);
	totalHeight += bitmapHeight;
	worldlib::indexedImageToBitmap(sp3Chr.begin(), sp3Chr.end(), palette.begin(), palette.end(), 0x10, 4, 0xA, worldlib::ColorBackInserter(spriteBitmap, worldlib::ColorOrder::RGBA), &bitmapWidth, &bitmapHeight);
	totalHeight += bitmapHeight;
	worldlib::indexedImageToBitmap(sp4Chr.begin(), sp4Chr.end(), palette.begin(), palette.end(), 0x10, 4, 0xA, worldlib::ColorBackInserter(spriteBitmap, worldlib::ColorOrder::RGBA), &bitmapWidth, &bitmapHeight);
	totalHeight += bitmapHeight;

	// Now you have the image--you can do whatever you want with it.
	
  // (This is a separate library, obviously. Just for demonstration.)
	lodepng::encode("105 sprite graphics.png", spriteBitmap, bitmapWidth, totalHeight);   
}
````
