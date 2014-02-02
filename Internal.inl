
namespace worldlib
{
	namespace internal
	{

		// LM-related addresses:
		const int customPalettePointerTableLocation = 0x0EF600;			// Location of each level's custom palette, if it has one.
		const int standardExGFXPointerToPointerTableLocation = 0x0FF94F;	// Location of the pointer to the pointers to ExGFX files 80-FF
		const int superExGFXPointerToPointerTableLocation = 0x0FF873;		// Location of the pointer to the pointers to ExGFX files 100-FFF (stored in RATS-protected space)
		const int decompressionTypeLocation = 0x0FFFEB;				// Location of the byte that determines which compression method the ROM uses. 00 and 01 are LZ2, 02 is LZ3.

		// Original SMW-related addresses
		const int sharedBackgroundColorsLocation = 0x00B0A0;			// Location of the 8 background colors (2 bytes long each)

		const int sharedBackgroundSwapPalettesLocation = 0x00B0B0;		// For rows 0 and 1: Location of the 8 background palettes (192 bytes long of 6 colors per row per entry)
		const int sharedForegroundSwapPalettesLocation = 0x00B190;		// For rows 2 and 3: Location of the 8 foreground palettes (24 bytes long each, 12 for each row)
		const int sharedForegroundConstPalettesLocation = 0x00B250;		// For rows 4,5,6,7: Location of the remaining static palettes (12 bytes per row)

		const int sharedLayer3ConstPalettesLocation = 0x00B170;			// Location of the 4 shared layer 3 palettes (8 bytes each, two per row at the end of rows 0 and 1)

		const int sharedSpritePaletteLocation = 0x00B280;			// Location of the 6 shared sprite colors (12 bytes each)
		const int sharedMarioPaletteLocation = 0x00B2DC;			// Location of Mario's palette (20 bytes, colors 6-F of row 8)
		const int sharedSpriteSwapPalettesLocation = 0x00B318;			// For rows E and F: Location of the 8 sprite palettes (24 bytes long each, 12 for each row)

		const int sharedBerryPaletteLocation = 0x00B674;			// For rows 2, 3, 4 (and 9, A, B), colors 9 - F (14 bytes for each row, 3 rows)

		// LM-related addresses:
		const int exgfxBypassListPointerToPointerTable = 0x0FF873;		// Where to find the pointer to the pointers to the ExGFX files list (stored in RATS-protected space). Seems to be tons of other level-related stuff there too?
		const int exgfxBypassOffset = 0x2D00;					// How far into the table the actual ExGFX list is.  Not an address.

		// Original SMW-related addresses
		const int spriteSlotListTableLocation = 0x00A8C3;			// List of the sprite slots levels can use (4 bytes each, 26 slots)
		const int backgroundSlotListTableLocation = 0x00A92B;			// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
		const int layer1PointerTableLocation = 0x05E000;			// Pointer to layer 1 data.  Used because the headers contain important information.
		
		const int originalGraphicsFilesLowByteTableLocation = 0x00B992;		// Table of the low bytes of the locations of GFX00 - GFX31
		const int originalGraphicsFilesHighByteTableLocation = 0x00B9C4;	// Table of the high bytes of the locations of GFX00 - GFX31
		const int originalGraphicsFilesBankByteTableLocation = 0x00B9F6;	// Table of the bank bytes of the locations of GFX00 - GFX31
		const int lowExgfxFilesTableLocation = 0x0FF94F;			// Location of the table of the locations of GFX80 - GFXFFF


		template <typename inputIteratorType> std::uint8_t getLevelHeaderByte(inputIteratorType romStart, inputIteratorType romEnd, int level, int byteNumber)
		{
			int levelAddress = readTrivigintetSFC(romStart, romEnd, level * 3 + layer1PointerTableLocation);

			return readByteSFC(romStart, romEnd, levelAddress + byteNumber);
		}

		template <typename inputIteratorType> std::uint8_t readBytePC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			std::advance(romStart, offset);
			if (romStart >= romEnd)
				throw std::runtime_error("Address is out of bounds for the current ROM.");
			return *romStart;
		}

		template <typename inputIteratorType> std::uint16_t readWordPC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			auto byte1 = readBytePC(romStart, romEnd, offset + 0);
			auto byte2 = readBytePC(romStart, romEnd, offset + 1);

			return (byte2 << 8) | byte1;
		}

		template <typename inputIteratorType> std::uint32_t readTrivigintetPC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			auto byte1 = readBytePC(romStart, romEnd, offset + 0);
			auto byte2 = readBytePC(romStart, romEnd, offset + 1);
			auto byte3 = readBytePC(romStart, romEnd, offset + 2);

			return (byte3 << 16) | (byte2 << 8) | byte1;
		}


		template <typename inputIteratorType> std::uint8_t readByteSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			return readBytePC(romStart, romEnd, SFCToPC(romStart, romEnd, offset));
		}

		template <typename inputIteratorType> std::uint16_t readWordSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			return readWordPC(romStart, romEnd, SFCToPC(romStart, romEnd, offset));
		}

		template <typename inputIteratorType> std::uint32_t readTrivigintetSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset)
		{
			return readTrivigintetPC(romStart, romEnd, SFCToPC(romStart, romEnd, offset));
		}

		template <typename integerType, typename maskType> integerType getBits(integerType integer, maskType mask)
		{
			integer &= mask;
			while ((mask & 1) == 0)
			{
				mask >>= 1;
				integer >>= 1;
			}

			return integer;
		}
	}

}