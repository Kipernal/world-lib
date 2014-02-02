#pragma once

#include <vector>

namespace worldlib
{

// compressedDataStart is the start of the data to decompress.
// dataEnd is an iterator pointing to the end of what could possibly be decompressable data.  Just use, for example, vector.end() here.
// out is where the output will be sent.  std::back_inserter(vector) is good to use here.

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ2(inputIteratorType compressedDataStart, inputIteratorType dataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ3(inputIteratorType compressedDataStart, inputIteratorType dataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

// Convenience functions to the above.  Pass in a ROM and the offset of the compressed data in the ROM, and it will be decompressed.
inline std::vector<unsigned char> decompressLZ2(std::vector<unsigned char> rom, int offset, int *compressedSize = nullptr, int *decompressedSize = nullptr);
inline std::vector<unsigned char> decompressLZ3(std::vector<unsigned char> rom, int offset, int *compressedSize = nullptr, int *decompressedSize = nullptr);
}

#include "decompress.inl"
