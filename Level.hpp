#pragma once
#include <iterator>
#include "ColorBackInserter.hpp"

namespace worldlib
{

	enum class ExgfxSlots {	
		FG1 = 0, 
		FG2 = 1, 
		BG1 = 2,
		FG3 = 3, 
		BG2 = 4, 
		BG3 = 5, 
		SP1 = 6, 
		SP2 = 7, 
		SP3 = 8, 
		SP4 = 9, 
		AN2 = 10 
	};

	// Returns the specified level's palette in ARGB format (thus, your output iterator's type should be some sort of a pointer to 32-bit integers).
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	// Returns the specified level's background color in ARGB format.
	template <typename inputIteratorType>
	std::uint32_t getLevelBackgroundColor(inputIteratorType romStart, inputIteratorType romEnd, int level);

	// Returns all 11 of the specified level's graphics slots in the standard order of FG1, FG2, BG1, FG3, BG2, BG3, SP1, SP2, SP3, SP4, AN2.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	// Returns all 6 of the specified level's FG/BG graphics slots in the standard order of FG1, FG2, BG1, FG2, BG2, BG3.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelBackgroundGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	// Returns all 4 of the specified level's sprite graphics slots in the standard order of SP1, SP2, SP3, SP4.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelSpriteGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	// Returns the specified level's animated tile area graphics slot.  Keep in mind that animated graphics files can be more than 4kb!
	template <typename inputIteratorType>
	std::uint16_t getLevelAnimatedTileAreaGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level);

	// Returns the specified level's specified graphics slot (in the order of getLevelGraphicsSlots).
	template <typename inputIteratorType>
	std::uint16_t getLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, ExgfxSlots slotToGet);

	// Reads the specified graphics file, including ExGFX.  Remember that ExGFX files are not limited to being 4KB large.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int file, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	// Returns the address of the specified graphics file.  As a special case, returns -1 if the file requested is 0x7F (the "placeholder" file).
	template <typename inputIteratorType>
	int getAddressOfGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file);

	// Returns true if the ROM contains the specified graphics file.  File 0x7F counts as existing no matter what.
	template <typename inputIteratorType>
	bool romContainsGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file);

	// Converts an indexed bitmap to a standard ARGB bitmap.
	// tilesInOneRow should be how many tiles wide the file is.  Generally 0x10 is fine here.
	// bpp should be 2, 4, or 8.  No other values are supported.
	// Palette should be 0 for 8bpp files.
	// Using ColorBackInserter for out is highly recommended, as it will allow you to control the manner in which data is given to you.
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);

	// Same as above, but allows you to specify the coordinates to grab.
	// x, y, width, and height do not have to be multiples of 8.
	// If width or height are negative, then they are treated as "to the left/bottom edge of the file" respectively.
	// Any attempt to retrieve pixels outside of the bitmap will simply result in a completely transparent pixel (0).
	// resultingWidth and resultingHeight will return the width and height of the bitmap.  Only really useful if you used -1 for the width and/or height, since they'll tell you how large the bitmap you got was.
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int x, int y, int width, int height, bool flipX, bool flipY, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);

	// Simpler version of the above that assumes some defaults.
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);




	// Converts an indexed tile into a standard ARGB bitmap.
	// Give it an iterator to the start of the data to decode and one to the end and an ARGB bitmap will be produced that is 8 pixels tall and tilecount * 8 pixels wide.
	// Note that for bpp only 2, 4, and 8 are allowed.
	// Recommended use is to just decode one 8x8 tile at a time.  One row at a time may also work for your purposes.
	// Generally you'll want to use the two functions above.  This just returns a string of tiles, basically.
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsStart, graphicsInputIteratorType graphicsEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, bool flipX, bool flipY, int paletteNumber, outputIteratorType out);



}

#include "Level.inl"