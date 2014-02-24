#pragma once
#include <iterator>
#include "ColorBackInserter.hpp"


namespace worldlib
{

//////////////////////////////////////////////////////////////////////////////
/// \file Level.hpp
/// \brief Contains various functions for working with level data.
///
/// \addtogroup Level
///  @{
//////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	/// \brief Represents the various GFX slots a level can use
	////////////////////////////////////////////////////////////
	enum class GFXSlots {	
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

	////////////////////////////////////////////////////////////
	/// \brief Returns the specified level's palette
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to output the data
	/// \param level		The level to get the palette from
	///
	/// \return Iterator pointing to the end of your color data
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	////////////////////////////////////////////////////////////
	/// \brief Returns the specified level's background color
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param level		The level to get the background color from
	///
	/// \return The color, in ARGB format
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	std::uint32_t getLevelBackgroundColor(inputIteratorType romStart, inputIteratorType romEnd, int level);
	
	////////////////////////////////////////////////////////////
	/// \brief Returns all 11 of the specified level's graphics slots.
	/// \details The order is in the standard order of FG1, FG2, BG1, FG3, BG2, BG3, SP1, SP2, SP3, SP4, AN2
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to output the data
	/// \param level		The level to get the graphics slots from
	///
	/// \return Iterator pointing to the end of your slot list data.
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);


	////////////////////////////////////////////////////////////
	/// \brief Returns all 6 of the specified level's FG/BG slots in the standard order of FG1, FG2, BG1, FG3, BG2, BG3
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to output the data
	/// \param level		The level to get the graphics slots from
	///
	/// \return Iterator pointing to the end of your slot list data
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelBackgroundGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	////////////////////////////////////////////////////////////
	/// \brief Returns all 4 of the specified level's FG/BG slots in the standard order of SP1, SP2, SP3, SP4
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to output the data
	/// \param level		The level to get the graphics slots from
	///
	/// \return Iterator pointing to the end of your slot list data
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelSpriteGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	////////////////////////////////////////////////////////////
	/// \brief Returns the specified level's animated tile slot.  Keep in mind that animated graphics files can be more than 4kb!
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param level		The level to get the graphics slots from
	///
	/// \return The animated tile graphics slot of the specified level
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	std::uint16_t getLevelAnimatedTileAreaGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level);

	////////////////////////////////////////////////////////////
	/// \brief Returns the specified level's specified tile slot.  Keep in mind that animated graphics files can be more than 4kb!
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param level		The level to get the graphics slots from
	/// \param slotToGet		The level's slot to return
	///
	/// \return The animated tile graphics slot of the specified level
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	std::uint16_t getLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, GFXSlots slotToGet);
	
	////////////////////////////////////////////////////////////
	/// \brief Decompresses the specified level's specified graphics slot into an indexed bitmap.  In other words, it's like calling decompressLZX but automatically gives it the correct parameters based on the level and slots you choose (and based on whether the ROM uses LZ2 or LZ3 decompression).
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to output the data
	/// \param file			The file to decompress
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed image data
	///
	/// \throws std::runtime_error If there is an error decompressing the data, such as a corrupted graphics file (see decompressLZ2 and decompressLZ3), if the ROM uses an unrecognized compression format, or if the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through table data or something else weird like that)
	///
	/// \see decompressLZ2, decompressLZ3
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int file, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Gets the address of the specified graphics file.
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param file			The graphics file to get the address of.  Valid for all types of graphics: normal, Standard ExGFX, and Super ExGFX.
	///
	/// \return SNES address of the file in the ROM.  Returns -1 for file 0x7F, though you should always have special handling anyway for this "file".
	///
	/// \throws std::runtime_error If the graphics file does not exist in the ROM or is otherwise invalid, or if the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through table data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	int getAddressOfGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file);

	////////////////////////////////////////////////////////////
	/// \brief Returns true if the specified graphics file exists
	///
	/// \param romStart		An iterator pointing to the beginning of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param file			The graphics file to get the address of.  Valid for all types of graphics: normal, Standard ExGFX, and Super ExGFX.
	///
	/// \return True if the file exists, false if it doesn't.  Always returns true for file 0x7F, though you should always have special handling anyway for this "file".
	///
	/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through table data or something else weird like that)
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	bool romContainsGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file);



	////////////////////////////////////////////////////////////
	/// \brief Converts an indexed bitmap to a normal ARGB one.
	///
	/// \param graphicsFileStart	An iterator pointing to the start of the graphics file to convert
	/// \param graphicsFileEnd	An iterator pointing to the end of the graphics file to convert
	/// \param paletteStart		An iterator pointing to the start of the palette to use in conversion
	/// \param paletteEnd		An iterator pointing to the end of the palette to use in conversion
	/// \param tilesInOneRow	How many 8x8 tiles are in one row.  Chances are 0x10 is just fine here.
	/// \param bpp			The bpp to use for conversion.  Only 2, 4, and 8 are valid.
	/// \param paletteNumber	The palette number to use for conversion.  If the bpp is 8, this should be 0 unless your palette is for some reason larger than a standard SFC palette
	/// \param out			Where to send the decoded data.  It's just a stream of data--the following two parameters determine how the raw data should be interpreted in terms of width and height.  Highly recommended to use ColorBackInsertIterator to control how the color data is inserted.
	/// \param resultingWidth	Will contain the width of the image after the function ends if it is not nullptr
	/// \param resultingHeight	Will contain the height of the image after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your converted image data
	///	
	/// \throws std::runtime_error If the graphics file has a pixel that refers to a palette entry that does not exist
	///
	////////////////////////////////////////////////////////////
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);


	////////////////////////////////////////////////////////////
	/// \brief The Super Triple Deluxe version of indexedImageToBitmap that does everything.  Probably more than you might need it to, if for no other reason than it's huge.
	///
	/// \param graphicsFileStart	An iterator pointing to the start of the graphics file to convert
	/// \param graphicsFileEnd	An iterator pointing to the end of the graphics file to convert
	/// \param paletteStart		An iterator pointing to the start of the palette to use in conversion
	/// \param paletteEnd		An iterator pointing to the end of the palette to use in conversion
	/// \param tilesInOneRow	How many 8x8 tiles are in one row.  Chances are 0x10 is just fine here.
	/// \param bpp			The bpp to use for conversion.  Only 2, 4, and 8 are valid.
	/// \param x			The x position of the "window" of the indexed graphics to convert
	/// \param y			The y position of the "window" of the indexed graphics to convert
	/// \param width		The width of the "window" of the indexed graphics to convert.  If -1, will be treated as "to the rightmost edge of the image"
	/// \param height		The height of the "window" of the indexed graphics to convert.  If -1, will be treated as "to the bottommost edge of the image"
	/// \param flipX		Will flip all TILES (not the image itself, but the individual tiles) horizontally
	/// \param flipY		Will flip all TILES (not the image itself, but the individual tiles) vertically
	/// \param paletteNumber	The palette number to use for conversion.  If the bpp is 8, this should be 0 unless your palette is for some reason larger than a standard SFC palette
	/// \param out			Where to send the decoded data.  It's just a stream of data--the following two parameters determine how the raw data should be interpreted in terms of width and height.  Highly recommended to use ColorBackInsertIterator to control how the color data is inserted.
	/// \param resultingWidth	Will contain the width of the image after the function ends if it is not nullptr.  Mostly useful if width is -1, since it will tell you how wide the image you got was.
	/// \param resultingHeight	Will contain the height of the image after the function ends if it is not nullptr.  Mostly useful if the height is -1, since it will tell you how tall the image you got was.
	///
	/// \return Iterator pointing to the end of your converted image data
	///	
	/// \throws std::runtime_error If the graphics file has a pixel that refers to a palette entry that does not exist
	///
	////////////////////////////////////////////////////////////
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int x, int y, int width, int height, bool flipX, bool flipY, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Simplest version of indexedImageToBitmap that gets the entire image and assumes some defaults
	///
	/// \param graphicsFileStart	An iterator pointing to the start of the graphics file to convert
	/// \param graphicsFileEnd	An iterator pointing to the end of the graphics file to convert
	/// \param paletteStart		An iterator pointing to the start of the palette to use in conversion
	/// \param paletteEnd		An iterator pointing to the end of the palette to use in conversion
	/// \param bpp			The bpp to use for conversion.  Only 2, 4, and 8 are valid.
	/// \param paletteNumber	The palette number to use for conversion.  If the bpp is 8, this should be 0 unless your palette is for some reason larger than a standard SFC palette
	/// \param out			Where to send the decoded data.  It's just a stream of data--the following two parameters determine how the raw data should be interpreted in terms of width and height.  Highly recommended to use ColorBackInsertIterator to control how the color data is inserted.
	/// \param resultingWidth	Will contain the width of the image after the function ends if it is not nullptr.  Mostly useful if width is -1, since it will tell you how wide the image you got was.
	/// \param resultingHeight	Will contain the height of the image after the function ends if it is not nullptr.  Mostly useful if the height is -1, since it will tell you how tall the image you got was.
	///
	/// \return Iterator pointing to the end of your converted image data
	///	
	/// \throws std::runtime_error If the graphics file has a pixel that refers to a palette entry that does not exist
	///
	////////////////////////////////////////////////////////////
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth = nullptr, int *resultingHeight = nullptr);



	////////////////////////////////////////////////////////////
	/// \brief Converts an indexed tile, or multiple indexed tiles, into an ARGB bitmap with a height of 8 pixels and a width of 8 x number of tiles decoded pixels.
	/// It's recommended to just decode one 8x8 tile at a time, but even more recommended is to just use the other functions.  They'll give you an actual image instead of a very short very wide image.
	///
	/// \param graphicsStart	An iterator pointing to the start of the graphics to convert
	/// \param graphicsEnd		An iterator pointing to the end of the graphics to convert
	/// \param paletteStart		An iterator pointing to the start of the palette to use in conversion
	/// \param paletteEnd		An iterator pointing to the end of the palette to use in conversion
	/// \param bpp			The bpp to use for conversion.  Only 2, 4, and 8 are valid.
	/// \param flipX		Will flip all TILES (not the image itself, but the individual tiles) horizontally
	/// \param flipY		Will flip all TILES (not the image itself, but the individual tiles) vertically
	/// \param paletteNumber	The palette number to use for conversion.  If the bpp is 8, this should be 0 unless your palette is for some reason larger than a standard SFC palette
	/// \param out			Where to send the decoded data.  It's just a stream of data--the following two parameters determine how the raw data should be interpreted in terms of width and height.  Highly recommended to use ColorBackInsertIterator to control how the color data is inserted.
	///
	/// \return Iterator pointing to the end of your converted image data
	///	
	/// \throws std::runtime_error If the graphics file has a pixel that refers to a palette entry that does not exist
	///
	////////////////////////////////////////////////////////////
	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsStart, graphicsInputIteratorType graphicsEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, bool flipX, bool flipY, int paletteNumber, outputIteratorType out);


//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "Level.inl"
