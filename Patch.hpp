#include <iterator>
#include <vector>

#pragma once

// Patching functions all rely on the Asar DLL
#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS

namespace worldlib
{

	//////////////////////////////////////////////////////////////////////////////
	/// \file Patch.hpp
	/// \brief Contains various functions for patching and assembling files
	///
	/// \addtogroup Patch
	///  @{
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \brief Contains error data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchErrorData
	{

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchErrorData();


		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.  In retrospect I could've just taken in one of Asar's structs, couldn't've I?
		////////////////////////////////////////////////////////////
		PatchErrorData(const char *fed, const char *red, const char *b, const char *fn, int l, const char *cfn, int cl) : fullErrorData((std::string)fed), rawErrorData((std::string)red), block((std::string)b), fileName((std::string)fn), line(l), callerLine(cl)
		{
			if (cfn != nullptr) callerFilename = std::string(cfn);
		};

		////////////////////////////////////////////////////////////
		/// \brief Contains the full error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType fullErrorData;

		////////////////////////////////////////////////////////////
		/// \brief Contains just the "simple" error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType rawErrorData;

		////////////////////////////////////////////////////////////
		/// \brief Contains the "descriptive" error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType block;

		////////////////////////////////////////////////////////////
		/// \brief The file name of the offending file
		////////////////////////////////////////////////////////////
		stringType fileName;

		////////////////////////////////////////////////////////////
		/// \brief The line the error occurred at
		////////////////////////////////////////////////////////////
		int line;

		////////////////////////////////////////////////////////////
		/// \brief The calling file
		////////////////////////////////////////////////////////////
		stringType callerFilename;

		////////////////////////////////////////////////////////////
		/// \brief The line the file was called at in the calling file
		////////////////////////////////////////////////////////////
		int callerLine;
	};

	////////////////////////////////////////////////////////////
	/// \brief Contains warning data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType> using PatchWarningData = PatchErrorData<stringType>;

	////////////////////////////////////////////////////////////
	/// \brief Contains label data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchLabelData
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchLabelData();

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.  And what kind of a word is "couldn't've" anyway.
		////////////////////////////////////////////////////////////
		PatchLabelData(const char *n, int l) : name((std::string)n), location(l) {};

		////////////////////////////////////////////////////////////
		/// \brief The name of the label
		////////////////////////////////////////////////////////////
		stringType name;

		////////////////////////////////////////////////////////////
		/// \brief The location of the label
		////////////////////////////////////////////////////////////
		int location;
	};

	////////////////////////////////////////////////////////////
	/// \brief Contains define data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchDefineData
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchDefineData();

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.
		////////////////////////////////////////////////////////////
		PatchDefineData(const char *n, const char *c) : name((std::string)n), contents(c) {};

		////////////////////////////////////////////////////////////
		/// \brief The name of the define
		////////////////////////////////////////////////////////////
		stringType name;

		////////////////////////////////////////////////////////////
		/// \brief The literal string value of the define
		////////////////////////////////////////////////////////////
		stringType contents;
	};






	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.
	/// \details The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	/// \param errorDataOutput	Where to send the error data
	/// \param warningDataOutput	Where to send the warning data
	/// \param labelDataOutput	Where to send the label data
	/// \param defineDataOutput	Where to send the define data
	///
	/// \return Returns an iterator to the end of your compiled data.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.
	///
	/// \details The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	/// \param errorDataOutput	Where to send the error data
	///
	/// \return Returns an iterator to the end of your compiled data.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.  The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	///
	/// \return Returns true on success and false on failure
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType>
	bool compileToBinary(const char *patchFilepath, outputIteratorType &out);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief "Quick compile".  Just gives you a vector from a string, basically.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param errorDataOutput	Where to send the error data.  Be nice if this weren't necessary, but we need this for error reporting, obviously.
	///
	/// \return Returns a vector containing the compiled data
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename dataType>
	std::vector<dataType> compileToBinary(const char *patchFilepath, std::vector<PatchErrorData<std::string>> &errorDataOutput);








	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	/// \param errorDataOutput	Where to send the error data
	/// \param warningDataOutput	Where to send the warning data
	/// \param labelDataOutput	Where to send the label data
	/// \param defineDataOutput	Where to send the define data
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	/// \param errorDataOutput	Where to send the error data
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput);
	
	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize);
	
	
	
	
	
	
	
	
	
	
	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a new string. 
	/// \details It's highly recommended to look at the version that just takes normal strings instead of iterators so that this function doesn't take up like 5 lines of back_insert_iterator boilerplate.
	///
	/// \param patchStart		An iterator pointing to the patch's start
	/// \param patchEnd		An iterator pointing to the patch's end
	/// \param definitionStart	An iterator pointing to a string containing the name of the definition you want to replace, not including the "!"
	/// \param definitionEnd	An iterator pointing to the definition's end
	/// \param valueToChangeToStart	An iterator pointing to a string containing the name of the value you want to replace the original definition's value with
	/// \param valueToChangeToEnd	An iterator pointing to the value's end
	///
	/// \throws std::runtime_error	Thrown if:
	/// 1. The definition cannot be found
	/// 2. The definition is malformed
	/// 3. The definition's value's length (in characters) is not equal of the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename replacementStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, replacementStringIteratorType valueToChangeToStart, replacementStringIteratorType valueToChangeToEnd);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a number, printed as a hex value (with a $ preceding it). 
	/// \details It's highly recommended to look at the version that just takes normal strings instead of iterators so that this function doesn't take up like 5 lines of back_insert_iterator boilerplate.
	///
	/// \param patchStart		An iterator pointing to the patch's start
	/// \param patchEnd		An iterator pointing to the patch's end
	/// \param definitionStart	An iterator pointing to a string containing the name of the definition you want to replace, not including the "!"
	/// \param definitionEnd	An iterator pointing to the definition's end
	/// \param valueToChangeTo	The numeric value to use (will be converted to a hex value with a '$' preceding it)
	/// \param digits		How mny digits long the value should be (for example, use 4 to convert "$50" to "$0050")
	///
	/// \throws std::runtime_error	Thrown if:
	/// 1. The definition cannot be found
	/// 2. The definition is malformed
	/// 3. The definition's value's length (in characters) is not equal of the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringIteratorType, typename definitionStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, int valueToChangeTo, int digits);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a new string.
	/// \details The length of the new definition's value must be equal to the length of the old definition's value
	///
	/// \param patch		The patch that should have its definition changed
	/// \param definitionToChange	A string containing the name of the definition you want to replace, not including the "!"
	/// \param valueToChangeTo	A string containing the name of the value you want to replace the original definition's value with
	///
	/// \throws std::runtime_error	Thrown if:
	/// 1. The definition cannot be found
	/// 2. The definition is malformed
	/// 3. The definition's value's length (in characters) is not equal of the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringType, typename definitionStringType, typename replacementStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, const replacementStringType &valueToChangeTo);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a number
	/// \details The number is printed as a hex value (with a $ preceding it). It's highly recommended to look at the version that just takes normal strings instead of iterators so that this function doesn't take up like 5 lines of back_insert_iterator boilerplate.
	///
	/// \param patch		The patch that should have its definition changed
	/// \param definitionToChange	A string containing the name of the definition you want to replace, not including the "!"
	/// \param valueToChangeTo	The numeric value to use (will be converted to a hex value with a '$' preceding it)
	/// \param digits		How mny digits long the value should be (for example, use 4 to convert "$50" to "$0050")
	///
	/// \throws std::runtime_error	Thrown if:
	/// 1. The definition cannot be found
	/// 2. The definition is malformed
	/// 3. The definition's value's length (in characters) is not equal of the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringType, typename definitionStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, int valueToChangeTo, int digits);

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "Patch.inl"


#endif