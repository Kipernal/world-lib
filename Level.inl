#include "Internal.hpp"
#include "Decompress.hpp"

namespace worldlib
{
	namespace internal
	{

		std::uint32_t SFCToARGB(std::uint16_t color)
		{
			int a = 0xFF;
			int r = (color >> 0x00) & 0x1F;
			int g = (color >> 0x05) & 0x1F;
			int b = (color >> 0x0A) & 0x1F;

			r *= 8;
			g *= 8;
			b *= 8;

			int result = (a << 24) | (r << 16) | (g << 8) | (b << 0);
			return result;
		}

		// Returns the level's "standard" palette, regardless of its override settings.
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType getLevelStandardPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
		{
			int levelBackgroundIndex = (getLevelHeaderByte(romStart, romEnd, level, 0) & 0xE0) >> 5;
			int levelForegroundIndex = (getLevelHeaderByte(romStart, romEnd, level, 3) & 0x07) >> 0;
			int levelSpriteIndex =	   (getLevelHeaderByte(romStart, romEnd, level, 3) & 0x38) >> 3;

			int backgroundSwapPaletteLocation = (sharedBackgroundSwapPalettesLocation + levelBackgroundIndex * 24);
			int foregroundSwapPaletteLocation = (sharedForegroundSwapPalettesLocation + levelForegroundIndex * 24);
			int spriteSwapPaletteLocation = (sharedSpriteSwapPalettesLocation + levelSpriteIndex * 24);

			std::vector<std::uint32_t> palettes[0x10];

			// Set everything to black
			for (int y = 0; y < 16; y++) 
			{ 
				palettes[y].resize(16);
				for (int x = 0; x < 16; x++) palettes[y][x] = 0; 
			}

			// Set up the white colors
			for (int i = 0; i < 0x08; i++) palettes[i+0][1] = 0xFFE8F0F8;		// Why this is a different color I'll never know...
			for (int i = 0; i < 0x08; i++) palettes[i+8][1] = 0xFFF8F8F8;


			// Left side of the palette:

			// Set up the swappable background palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x0][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, backgroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x1][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, backgroundSwapPaletteLocation + i * 2 + 1 * 12));

			// Set up the swappable foreground palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x2][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x3][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 1 * 12));

			// Set up the constant foreground palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x4][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x5][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 1 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x6][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 2 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x7][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 3 * 12));

			// Set up the constant sprite palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x8][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x9][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 1 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xA][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 2 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xB][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 3 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xC][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 4 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xD][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 5 * 12));

			// Set up the swappable sprite palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xE][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpriteSwapPalettesLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xF][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedSpriteSwapPalettesLocation + i * 2 + 1 * 12));


			// Right side of the palette:

			// Set up the layer 3 palettes
			for (int x = 8, i = 0; i < 8; x++, i++) palettes[0x0][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedLayer3ConstPalettesLocation + i * 2 + 0 * 16));
			for (int x = 8, i = 0; i < 8; x++, i++) palettes[0x1][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedLayer3ConstPalettesLocation + i * 2 + 1 * 16));

			// Set up the berry palettes
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x2][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 0 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x3][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 1 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x4][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 2 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x9][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 0 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0xA][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 1 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0xB][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 2 * 14));

			// Set up Mario's palette
			for (int x = 6, i = 0; i < 10; x++, i++) palettes[8][x] = SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedMarioPaletteLocation + i * 2 + 0 * 20));

			// Finally, output everthing
			for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) *(out++) = palettes[y][x];

			return out;
		}


		// Returns the level's custom palette, regardless of its override settings.  If there is no palette, an exception is thrown.
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType getLevelCustomPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
		{
			auto address = internal::readTrivigintetSFC(romStart, romEnd, customPalettePointerTableLocation + level * 3);

			if (address == 0) throw std::runtime_error("Tried to get the custom palette of a level that has none!");

			address += 2;		// Skip the background color.

			for (int i = 0; i < 512; i+=2)
			{
				*(out++) = SFCToARGB(internal::readWordSFC(romStart, romEnd, address + i * 2));
			}

			return out;
		}
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		if (internal::readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3) == 0)
			return internal::getLevelStandardPalette(romStart, romEnd, out, level);
		else
			return internal::getLevelCustomPalette(romStart, romEnd, out, level);
	}


	template <typename inputIteratorType>
	std::uint32_t getLevelBackgroundColor(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		if (internal::readTrivigintetSFC(customPalettePointerTableLocation + level * 3) == 0)
		{
			auto byte = internal::getLevelHeaderByte(romStart, romEnd, level, 2);
			internal::getBits(byte, 0xE0);
			return SFCToARGB(internal::readWordSFC(romStart, romEnd, sharedBackgroundColorsLocation + byte));
		}
		else
		{
			auto address = internal::readTrivigintetSFC(customPalettePointerTableLocation + level * 3);
			if (address == 0) throw std::runtime_error("Tried to get the custom palette of a level that has none!");

			return SFCToARGB(internal::readWordSFC(romStart, romEnd, address));
		}
	}


	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		getLevelBackgroundGraphicsSlots(romStart, romEnd, out, level);
		getLevelSpriteGraphicsSlots(romStart, romEnd, out, level);
		*(out++) = getLevelAnimatedTileAreaSlot(romStart, romEnd, level);
		return out;
	}


	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelBackgroundGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		int address = internal::readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
		bool usesExGFX = (internal::readByteSFC(romStart, romEnd, address + 1) & 0x80) == 0x80;

		if (!usesExGFX)
		{
			int tileset = internal::getBits(internal::getLevelHeaderByte(romStart, romEnd, level, 4), 0x0F);
			address = internal::backgroundSlotListTableLocation + tileset * 4;
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x00);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x01);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x02);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x03);
		}
		else
		{
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x10);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x0E);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x0C);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x0A);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x08);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x06);
		}

		return out;
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelSpriteGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		int address = internal::readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
		bool usesExGFX = (internal::readByteSFC(romStart, romEnd, address + 1) & 0x80) == 0x80;
		if (!usesExGFX)
		{
			int tileset = internal::getBits(internal::getLevelHeaderByte(romStart, romEnd, level, 4), 0x0F);
			address = internal::spriteSlotListTableLocation + tileset * 4;
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x00);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x01);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x02);
			*(out++) = internal::readByteSFC(romStart, romEnd, address + 0x03);
		}
		else
		{

			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x18);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x16);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x14);
			*(out++) = internal::readWordSFC(romStart, romEnd, address + 0x12);
		}

		return out;
	}

	template <typename inputIteratorType>
	std::uint16_t getLevelAnimatedTileAreaSlot(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		int address = internal::readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
		bool usesExGFX = (internal::readByteSFC(romStart, romEnd, address + 1) & 0x80) == 0x80;
		if (!usesExGFX)
		{
			return 0x007F;
		}
		else
		{
			return internal::readWordSFC(romStart, romEnd, address + 0x1A);
		}
	}


	template <typename inputIteratorType>
	std::uint16_t getLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, int slotToGet)
	{
		std::vector<std::uint16_t> slots;
		getLevelGraphicsSlots(romStart, romEnd, std::back_inserter(slots), level);
		return slots[slotToGet];
	}


	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int file, int *compressedSize, int *decompressedSize)
	{
		int address = 0;
		if (file >= 0 && file <= 0x31)
			address = internal::readByteSFC(romStart, romEnd, internal::originalGraphicsFilesLowByteTableLocation + file) | (internal::readByteSFC(romStart, romEnd, internal::originalGraphicsFilesHighByteTableLocation + file) << 8) | (internal::readByteSFC(romStart, romEnd, internal::originalGraphicsFilesBankByteTableLocation + file) << 16);
		else if (file >= 0x80 && file <= 0xFF)
			address = internal::readTrivigintetSFC(romStart, romEnd, internal::readTrivigintetSFC(romStart, romEnd, internal::standardExGFXPointerToPointerTableLocation) + (file - 0x80) * 3);
		else if (file >= 0x100 && file <= 0xFFF)
			address = internal::readTrivigintetSFC(romStart, romEnd, internal::readTrivigintetSFC(romStart, romEnd, internal::superExGFXPointerToPointerTableLocation) + (file - 0x100) * 3);
		else if (file == 0x7F)
			return out;
		else
			throw std::runtime_error("ExGFX file is not valid.");

		if (address == 0 || address == 0xFFFFFF) 
			throw std::runtime_error("ExGFX file does not exist.");

		std::advance(romStart, SFCToPC(romStart, romEnd, address));

		auto compressionType = internal::readByteSFC(romStart, romEnd, internal::decompressionTypeLocation);

		if (compressionType == 0 || compressionType == 1)
			return decompressLZ2(romStart, romEnd, out, compressedSize, decompressedSize);
		else if (compressionType == 2)
			return decompressLZ2(romStart, romEnd, out, compressedSize, decompressedSize);
		else
			throw std::runtime_error("Unrecognized compression format.");
	}
}