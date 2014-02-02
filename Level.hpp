#pragma once

namespace worldlib
{

	// Returns the specified level's palette in ARGB format (thus, your output iterator's type should be some sort of a pointer to 32-bit integers).
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level);

	// Returns the specified level's background color in ARGB format.
	template <typename inputIteratorType>
	std::uint32_t getLevelBackgroundColor(inputIteratorType romStart, inputIteratorType romEnd, int level);

	// Returns all 11 of the specified level's graphics slots in the standard order of FG1, FG2, BG1, FG2, BG2, BG3, SP1, SP2, SP3, SP4, AN2.
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
	std::uint16_t getLevelAnimatedTileAreaSlot(inputIteratorType romStart, inputIteratorType romEnd, int level);

	// Returns the specified level's specified graphics slot (in the order of getLevelGraphicsSlots).
	template <typename inputIteratorType>
	std::uint16_t getLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, int slotToGet);

	// Reads the specified graphics file, including ExGFX.  Remember that ExGFX files are not limited to being 4KB large.
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int file, int *compressedSize = nullptr, int *decompressedSize = nullptr);
}

#include "Level.inl"