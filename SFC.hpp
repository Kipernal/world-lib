#pragma once

namespace worldlib
{

//////////////////////////////////////////////////////////////////////////////
/// \file SFC.hpp
/// \brief Contains various functions for working with the SFC.
///
/// \details Please note that while there are routines for getting data from a ROM, there are no routines for setting it.  
///
/// \addtogroup SFC
///  @{
//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Tests if the ROM uses the SA-1 chip.  Functions use this automatically when determining addresses so you probably won't need to use it manually.
	///
	/// \details Checks 0x7FD5 and 0x7FD6 as PC addresses for their SA-1 status.  Returns true for those addresses.  This is the only place where PC addressing is used. For this reason, this library is not compatible with HiROM.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	///
	/// \return True if the ROM should use SA-1 addressing, false otherwise.
	///
	/// \throws std::runtime_error The ROM did not contain this data...but if your ROM doesn't contain this information you don't really have a ROM because something's gone horribly wrong.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	bool romUsesSA1(inputIteratorType romStart, inputIteratorType romEnd);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an address in SFC format and turns it into PC format (assuming no header, as usual).  Only lorom and SA-1 mappings are supported.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param addr			The SFC address to convert
	///
	/// \return A PC address corresponding to the given SFC address.
	///
	/// \throws std::runtime_error The address given could not be converted to SFC format
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	inline int SFCToPC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an address in PC format and turns it into SFC format (assuming no header, as usual).  Only lorom and SA-1 mappings are supported.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param addr			The PC address to convert
	///
	/// \return An address in SFC format corresponding to the given PC address.
	///
	/// \throws std::runtime_error The address given could not be converted a PC address
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	inline int PCToSFC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an color in SFC format and turns it into ARGB format.
	///
	/// \param color		The color to convert.
	///
	/// \return An ARGB color corresponding to the given SFC color
	///
	////////////////////////////////////////////////////////////
	inline std::uint32_t SFCToARGB(std::uint16_t color);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an color in ARGB format and turns it into SFC format.
	///
	/// \param color		The color to convert.
	///
	/// \return A color in SFC format corresponding to the given ARGB color
	///
	////////////////////////////////////////////////////////////
	inline std::uint16_t ARGBToSFC(std::uint32_t color);


	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get a single byte from a PC address
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A single byte
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint8_t readBytePC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get two bytes from a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 16-bit value
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint16_t readWordPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get three bytes from a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 24-bit value
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint32_t readTrivigintetPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get a single byte from an address in SFC format
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A single byte
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint8_t readByteSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);
	
	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get two bytes from an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 16-bit value
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint16_t readWordSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);
	
	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Get three bytes from an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 24-bit value
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint32_t readTrivigintetSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "SFC.inl"
