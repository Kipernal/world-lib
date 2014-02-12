#pragma once

#include <vector>

#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS
#include <Windows.h>
#include "../DLLcode/LunarDLL.h"
#undef max
#undef min
#endif

namespace worldlib
{


//////////////////////////////////////////////////////////////////////////////
/// \file Decompress.hpp
/// \brief Contains various functions for decompressing data
///
/// \addtogroup Decompression
///  @{
//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the LZ2 format.  
	/// \details In general you'll want the functions in level.hpp related to getting graphics files instead, but this may be useful if you have your own data compressed like this.
	///
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws std::runtime_error An error occurred while decompressing the data.  Either there was an unrecognized bit sequence or there was not enough data to decompress
	///
	/// \see decompressGraphicsFile, decompressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressLZ2(inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the LZ3 format.  
	/// \details In general you'll want the functions in level.hpp related to getting graphics files instead, but this may be useful if you have your own data compressed like this.
	///
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws std::runtime_error An error occurred while decompressing the data.  Either there was an unrecognized bit sequence or there was not enough data to decompress
	///
	/// \see decompressGraphicsFile, decompressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressLZ3(inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the format the ROM uses 
	///
	/// \param romStart		An iterator pointing to the start of your ROM data
	/// \param romEnd		An iterator pointing to the end of your ROM data
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws std::runtime_error An error occurred while decompressing the data.  Either there was an unrecognized bit sequence or there was not enough data to decompress
	///
	/// \see decompressGraphicsFile
	///
	////////////////////////////////////////////////////////////
	template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	// Compression functions all rely on the Lunar Compress DLL
	#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS

	namespace internal
	{
		////////////////////////////////////////////////////////////
		/// \brief Compresses data compressed in any format compatible with Lunar Compress
		///
		/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
		/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
		/// \param out			Where to output the compressed data
		/// \compressionType		The compression type. See LunarDLL.h
		/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
		///
		/// \return Iterator pointing to the end of your compressed data
		///
		/// \throws std::runtime_error Lunar Compress.dll could not be loaded or there was an error compressing the data.  The first is more likely.
		///
		////////////////////////////////////////////////////////////
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType compressGeneral(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int compressionType, int *compressedSize);
	}

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the LZ2 format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws std::runtime_error Lunar Compress.dll could not be loaded or there was an error compressing the data.  The first is more likely.
	///
	/// \see compressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressLZ2(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the LZ3 format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws std::runtime_error Lunar Compress.dll could not be loaded or there was an error compressing the data.  The first is more likely.
	///
	/// \see compressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressLZ3(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the compression format your ROM uses format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws std::runtime_error Lunar Compress.dll could not be loaded or there was an error compressing the data.  The first is more likely.
	///
	////////////////////////////////////////////////////////////
	template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);


	#endif


//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "Compression.inl"