#include "Internal.hpp"
#include <exception>

namespace worldlib
{

	template <typename inputIteratorType>
	bool romUsesSA1(inputIteratorType romStart, inputIteratorType romEnd)
	{
		auto mapBytePos = romStart;
		auto romBytePos = romStart;
		std::advance(mapBytePos, 0x7FD5);
		std::advance(romBytePos, 0x7FD6);
		if (romBytePos >= romEnd) throw std::runtime_error("ROM is too small!");

		// Behold the only place PC addresses are ever used (probably).
		unsigned char mapByte = *(mapBytePos);
		unsigned char romByte = *(romBytePos);
		return mapByte == 0x23 && (romByte == 0x32 || romByte == 0x34 || romByte == 0x35);
	}

	template <typename inputIteratorType>
	inline int SFCToPC(inputIteratorType romStart, inputIteratorType romEnd, int addr)
	{
		if (addr < 0 || addr > 0xFFFFFF ||		// not 24bit 
		    (addr & 0xFE0000) == 0x7E0000 ||		// wram 
		    (addr & 0x408000) == 0x000000)		// hardware regs 
		    throw std::runtime_error("SFC address cannot be mapped to a PC one.");

		if (romUsesSA1(romStart, romEnd) && addr >= 0x808000)
			addr -= 0x400000;
		addr = ((addr & 0x7F0000) >> 1 | (addr & 0x7FFF));
		return addr;
	}


	template <typename inputIteratorType>
	inline int PCToSFC(inputIteratorType romStart, inputIteratorType romEnd, int addr)
	{
		if (addr < 0 || addr >= 0x400000)
			throw std::runtime_error("PC address cannot be mapped to an SFC one.");

		addr = ((addr << 1) & 0x7F0000) | (addr & 0x7FFF) | 0x8000;

		if ((addr & 0xF00000) == 0x700000)
			addr |= 0x800000;

		if (romUsesSA1(romStart, romEnd) && addr >= 0x400000)
			addr += 0x400000;
		return addr;
	}

	inline std::uint32_t SFCToARGB(std::uint16_t color)
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


	inline std::uint16_t ARGBToSFC(std::uint32_t color)
	{
		int r = internal::getBits(color, 0x00FF0000) >> 3;
		int g = internal::getBits(color, 0x0000FF00) >> 3;
		int b = internal::getBits(color, 0x000000FF) >> 3;

		return (b << 10) | (g << 5) | (r << 0);
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

	template <typename inputIteratorType, typename outputIteratorType> outputIteratorType getROMTitle(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, bool includeEndingSpaces)
	{
		std::string temp;
		temp.reserve(0x16);
		int lastNonSpaceIndex = 0;
		for (int i = 0; i < 21; i++)
		{
			auto byte = readByteSFC(romStart, romEnd, 0xFFC0 + i);
			temp += byte;
			if (byte != ' ') lastNonSpaceIndex = i;
		}

		for (int i = 0; i <= lastNonSpaceIndex; i++)
			*(out++) = temp[i];
		
		return out;
	}

	template <typename inputIteratorType> bool isROMHeadered(inputIteratorType dataStart, inputIteratorType dataEnd)
	{
		auto length = std::distance(dataStart, dataEnd);

		if (length % 0x8000 == 0) return false;
		if ((length - 0x200) % 0x800 == 0) return true;

		throw std::runtime_error("Could not determine if the ROM was headered or not.");
	}

	template <typename inputIteratorType> inputIteratorType getROMStart(inputIteratorType dataStart, inputIteratorType dataEnd)
	{
		if (isROMHeadered(dataStart, dataEnd))
		{
			std::advance(dataStart, 0x200);
			return dataStart;
		}
		else 
			return dataStart;
	}
}