#include "Internal.hpp"
#include <exception>

namespace worldlib
{

	template <typename inputIteratorType>
	bool romUsesSA1(inputIteratorType romStart, inputIteratorType romEnd)
	{
		if (romStart + 0x7FD5 >= romEnd) throw std::runtime_error("ROM is too small!");
		return *(romStart + 0x7FD5) == 23;						// Behold the only place a PC address is ever used.
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
			throw std::runtime_error("PC address cannot be mapped to an SFC one.")

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
}