#pragma once

#include <vector>

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
	/// \see decompressGraphicsFile
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
	/// \see decompressGraphicsFile
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressLZ3(inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);



//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "decompress.inl"