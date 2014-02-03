
#include <exception>
#include <iterator>
#include <limits>
#include <cstdint>
#include <algorithm>

#define _SFCLIB_INTEGER_ITERATOR_ASSERT(type) static_assert(std::numeric_limits<std::iterator_traits<type>::value_type>::is_integer == true, "The iterator type must have a value_type that is an integer.  8-bit integers recommended.")

namespace worldlib
{

	namespace internal
	{
		template <typename inputIteratorType>
		inline auto getCompressionByte(inputIteratorType &start, inputIteratorType end, int *compressedSize) -> decltype(*start)
		{
			if (start >= end) throw std::runtime_error("Unexpected end reached.");

			if (compressedSize != nullptr) *compressedSize += 1;

			return *(start++);
		}
	}

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ2(inputIteratorType start, inputIteratorType end, outputIteratorType out, int *compressedSize, int *decompressedSize)
{
	_SFCLIB_INTEGER_ITERATOR_ASSERT(inputIteratorType);

	enum CommandType
	{
		DirectCopy = 0,
		ByteFill = 1,
		WordFill = 2,
		IncreasingFill = 3,
		Repeat = 4,
		Unused1 = 5,
		Unused2 = 6,
		LongCommand = 7
	};
	
	// Used internally.  On success out is modified.
	std::vector<typename std::iterator_traits<inputIteratorType>::value_type> resultBuffer;


	if (compressedSize != nullptr)
		*compressedSize = 0;


	while (start != end)
	{
		if (decompressedSize != nullptr)
			*decompressedSize = resultBuffer.size();

		int runLength;
		auto headerByte = internal::getCompressionByte(start, end, compressedSize);
		if (headerByte == 0xFF) break;
		CommandType commandType = static_cast<CommandType>((headerByte & 0xE0) >> 5);

		if (commandType == LongCommand)
		{
			auto secondHeaderByte = internal::getCompressionByte(start, end, compressedSize);
			commandType = static_cast<CommandType>((headerByte & 0x1C) >> 2);
			runLength = ((headerByte & 0x3) << 8) | secondHeaderByte;
		}
		else
		{
			runLength = headerByte & 0x1F;
		}

		if (commandType == DirectCopy)					// The data for this chunk is uncompressed
		{
			while (runLength >= 0)
			{
				auto byte = internal::getCompressionByte(start, end, compressedSize);
				resultBuffer.push_back(byte);
				runLength--;
			}
		}
		else if (commandType == ByteFill)				// The data for this chunk is one stream of one byte
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == WordFill)				// The data for this chunk is one stream of two alternating bytes.
		{
			auto fillByte1 = internal::getCompressionByte(start, end, compressedSize);
			auto fillByte2 = internal::getCompressionByte(start, end, compressedSize);
			bool useFirst = true;
			while (runLength >= 0)
			{
				if (useFirst)
					resultBuffer.push_back(fillByte1);
				else
					resultBuffer.push_back(fillByte2);
				useFirst = !useFirst;
				runLength--;
			}

		}
		else if (commandType == IncreasingFill)				// The data for this chunk is one byte increasing in value
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				fillByte++;
				fillByte &= 0xFF;
				runLength--;
			}
		}
		else if (commandType == Repeat)					// The data for this chunk copies previously written data
		{
			int offset = (internal::getCompressionByte(start, end, compressedSize) << 0x8) | (internal::getCompressionByte(start, end, compressedSize));	// Big endian, for some reason...
			int i = 0;

			while (runLength >= 0)
			{
				resultBuffer.push_back(resultBuffer[offset + i]);
				i++;
				runLength--;
			}
		}
		else
		{
			throw std::runtime_error("Unknown command bit sequence.");
		}
	}

	for (unsigned int i = 0; i < resultBuffer.size(); i++)
		*(out++) = resultBuffer[i];

	return out;
}

inline std::vector<unsigned char> decompressLZ2(std::vector<unsigned char> compressedData, int offset, int *compressedSize, int *decompressedSize)
{
	std::vector<unsigned char> ret;
	decompressLZ2(compressedData.begin() + offset, compressedData.end(), std::back_inserter(ret), compressedSize, decompressedSize);
	return ret;
}


template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ3(inputIteratorType start, inputIteratorType end, outputIteratorType out, int *compressedSize, int *decompressedSize)
{
	_SFCLIB_INTEGER_ITERATOR_ASSERT(inputIteratorType);

	enum CommandType
	{
		DirectCopy = 0,
		ByteFill = 1,
		WordFill = 2,
		ZeroFill = 3,
		Repeat = 4,
		BitReverseRepeat = 5,
		BackwardsRepeat = 6,
		LongCommand = 7
	};

	// Used internally.  On success out is modified.
	std::vector<typename std::iterator_traits<inputIteratorType>::value_type> resultBuffer;

	if (compressedSize != nullptr)
		*compressedSize = 0;


	while (start != end)
	{
		int runLength;
		auto headerByte = internal::getCompressionByte(start, end, compressedSize);
		if (headerByte == 0xFF) break;
		CommandType commandType = static_cast<CommandType>((headerByte & 0xE0) >> 5);

		if (commandType == LongCommand)
		{
			auto secondHeaderByte = internal::getCompressionByte(start, end, compressedSize);
			commandType = static_cast<CommandType>((headerByte & 0x1C) >> 2);
			runLength = ((headerByte & 0x3) << 8) | secondHeaderByte;
		}
		else
		{
			runLength = headerByte & 0x1F;
		}

		if (commandType == DirectCopy)					// The data for this chunk is uncompressed
		{
			while (runLength >= 0)
			{
				resultBuffer.push_back(internal::getCompressionByte(start, end, compressedSize));
				runLength--;
			}
		}
		else if (commandType == ByteFill)				// The data for this chunk is one stream of one byte
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == WordFill)				// The data for this chunk is one stream of two alternating bytes.
		{
			auto fillByte1 = internal::getCompressionByte(start, end, compressedSize);
			auto fillByte2 = internal::getCompressionByte(start, end, compressedSize);
			bool useFirst = true;
			while (runLength >= 0)
			{
				if (useFirst)
					resultBuffer.push_back(fillByte1);
				else
					resultBuffer.push_back(fillByte2);
				useFirst = !useFirst;
				runLength--;
			}

		}
		else if (commandType == ZeroFill)				// The data for this chunk a string of zeros
		{
			auto fillByte = 0;
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == Repeat || commandType == BitReverseRepeat || commandType == BackwardsRepeat)			// The data for this chunk copies previously written data
		{
			int offset = 0;
			int incrementDirection = 1;

			auto firstByte = internal::getCompressionByte(start, end, compressedSize);

			// The offset is either relative to the current buffer position or a fixed point from the beginning.
			if ((firstByte & 0x80) == 0x80) 
				offset = resultBuffer.size() - (firstByte & 0x7F) - 1;
			else
				offset = (firstByte & 0x7F) * 0x100 + internal::getCompressionByte(start, end, compressedSize);

			if (commandType == Repeat || commandType == BitReverseRepeat)
				incrementDirection = 1;
			else if (commandType == BackwardsRepeat) 
				incrementDirection = -1;

			bool reverseBits = commandType == BitReverseRepeat;

			int i = 0;
			while (runLength >= 0)
			{
				auto byte = resultBuffer[offset + i];
				if (reverseBits) 
					byte = internal::reverseBits(byte);
				resultBuffer.push_back(byte);
				i += incrementDirection;
				runLength--;
			}
		}
		else
		{
			throw std::runtime_error("Unknown command bit sequence.");
		}
	}

	for (unsigned int i = 0; i < resultBuffer.size(); i++)
		*(out++) = resultBuffer[i];

	if (decompressedSize != nullptr)
		*decompressedSize = resultBuffer.size();

	return out;
}

inline std::vector<unsigned char> decompressLZ3(std::vector<unsigned char> compressedData, int offset, int *compressedSize, int *decompressedSize)
{
	std::vector<unsigned char> ret;
	decompressLZ3(compressedData.begin() + offset, compressedData.end(), std::back_inserter(ret), compressedSize, decompressedSize);
	return ret;
}


}