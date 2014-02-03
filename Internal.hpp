#pragma once

namespace worldlib
{
	namespace internal
	{
		template <typename inputIteratorType> std::uint8_t getLevelHeaderByte(inputIteratorType romStart, inputIteratorType romEnd, int level, int byteNumber);			// Returns the header byte from the specified level

		template <typename inputIteratorType> std::uint8_t readBytePC(inputIteratorType romStart, inputIteratorType romEnd, int offset);					// Reads a byte from the specified location in the ROM with a PC offset.
		template <typename inputIteratorType> std::uint16_t readWordPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);					// Reads a word from the specified location in the ROM with a PC offset.
		template <typename inputIteratorType> std::uint32_t readTrivigintetPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);				// Reads a word and a half from the specified location in the ROM with a PC offset.

		template <typename inputIteratorType> std::uint8_t readByteSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);					// Reads a byte from the specified location in the ROM with a SFC offset.
		template <typename inputIteratorType> std::uint16_t readWordSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);					// Reads a word from the specified location in the ROM with a SFC offset.
		template <typename inputIteratorType> std::uint32_t readTrivigintetSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);				// Reads a word and a half from the specified location in the ROM with a SFC offset.
	
		template <typename integerType, typename maskType> integerType getBits(integerType integer, maskType mask);		// Returns integer but with a mask applied and shifted right such that there are no 0s to the right of the result.

		inline std::uint8_t reverseBits(std::uint8_t byte);
		inline std::uint16_t reverseBits(std::uint16_t value);
		inline std::uint32_t reverseBits(std::uint32_t value);
		inline std::uint64_t reverseBits(std::uint64_t value);


		inline std::uint64_t explodeInt(std::uint32_t i);
		inline std::uint32_t explodeInt(std::uint16_t i);
		inline std::uint16_t explodeInt(std::uint8_t i);

		inline std::uint64_t getBit(std::uint64_t number, int bit);
		inline std::uint32_t getBit(std::uint32_t number, int bit);
		inline std::uint16_t getBit(std::uint16_t number, int bit);
		inline std::uint8_t getBit(std::uint8_t number, int bit);
	}


}

#include "Internal.inl"