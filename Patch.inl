//#include "asardll.hpp"
#include <vector>
#include <cctype>

#include "asardll.hpp"


namespace worldlib
{

	namespace internal
	{
		// Returns true if asar returned an error.
		template <typename stringType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
		bool retrieveAsarResults(errorDataOutputIteratorType errorOut, warningDataOutputIteratorType warningOut, labelDataOutputIteratorType labelOut, defineDataOutputIteratorType defineOut)
		{
			int count = 0;
			bool asarError = false;

			const errordata *e = asar_geterrors(&count);
			while (count > 0)
			{
				asarError = true;
				count--;
				*(errorOut++) = PatchErrorData<stringType>(e[count].fullerrdata, e[count].rawerrdata, e[count].block, e[count].filename, e[count].line, e[count].callerfilename, e[count].callerline);
			}

			const errordata *w = asar_getwarnings(&count);
			while (count > 0)
			{
				count--;
				*(warningOut++) = PatchErrorData<stringType>(w[count].fullerrdata, w[count].rawerrdata, w[count].block, w[count].filename, w[count].line, w[count].callerfilename, w[count].callerline);
			}

			const labeldata *l = asar_getalllabels(&count);
			while (count > 0)
			{
				count--;
				*(labelOut++) = PatchLabelData<stringType>(l[count].name, l[count].location);
			}

			const definedata *d = asar_getalldefines(&count);
			while (count > 0)
			{
				count--;
				*(defineOut++) = PatchDefineData<stringType>(d[count].name, d[count].contents);
			}

			return asarError;
		}

	}



	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput)
	{
		if (asar_init() == false) { throw std::runtime_error("Could not load asar.dll"); }

		int maxLength = asar_maxromsize();
		int resultingLength = 0x8000;

		byte *buffer = (byte *)malloc(maxLength);		// Not using a vector since increasing its size is O(n).

		asar_patch(patchFilepath, (char *)buffer, maxLength, &resultingLength);

		for (int i = 0; i < resultingLength; i++)
		{
			*(out++) = buffer[i];
		}

		std::free(buffer);

		internal::retrieveAsarResults<stringType>(errorDataOutput, warningDataOutput, labelDataOutput, defineDataOutput);

		asar_close();
		return out;
	}

	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput)
	{
		std::vector<PatchWarningData<std::string>> warningData;
		std::vector<PatchLabelData<std::string>> labelData;
		std::vector<PatchDefineData<std::string>> defineData;

		return compileToBinary<stringType>(patchFilepath, out, errorDataOutput, std::back_inserter(warningData), std::back_inserter(labelData), std::back_inserter(defineData));
	}

	template <typename stringType, typename outputIteratorType>
	bool compileToBinary(const char *patchFilepath, outputIteratorType &out)
	{
		std::vector<PatchErrorData<std::string>> errorData;
		out = compileToBinary<stringType>(patchFilepath, out, std::back_inserter(errorData));
		return errorData.size() == 0;
	}

	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romDataType, int romSize, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput)
	{
		if (asar_init() == false) { throw std::runtime_error("Could not load asar.dll"); }

		int resultingLength = romSize;

		asar_patch(patchFilepath, (char *)(romDataType), romSize, &resultingLength);

		bool success = internal::retrieveAsarResults<stringType>(errorDataOutput, warningDataOutput, labelDataOutput, defineDataOutput);

		asar_close();

		return success;
	}

	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput)
	{
		std::vector<PatchWarningData<std::string>> warningData;
		std::vector<PatchLabelData<std::string>> labelData;
		std::vector<PatchDefineData<std::string>> defineData;

		return patchToROM<stringType>(patchFilepath, romData, romSize, errorDataOutput, std::back_inserter(warningData), std::back_inserter(labelData), std::back_inserter(defineData));
	}

	template <typename stringType, typename romDataType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize)
	{
		std::vector<PatchErrorData<std::string>> errorData;
		return patchToROM<stringType>(patchFilepath, romData, romSize, std::back_inserter(errorData));
	}

	template <typename patchStringType, typename definitionStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, int valueToChangeTo, int digits)
	{
		replacePatchDefinition(std::begin(patch), std::end(patch), std::begin(definitionToChange), std::end(definitionToChange), valueToChangeTo, digits);
	}

	template <typename patchStringType, typename definitionStringType, typename replacementStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, const replacementStringType &valueToChangeTo)
	{
		replacePatchDefinition(std::begin(patch), std::end(patch), std::begin(definitionToChange), std::end(definitionToChange), std::begin(valueToChangeTo), std::end(valueToChangeTo));
	}



	template <typename patchStringIteratorType, typename definitionStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, int valueToChangeTo, int digits)
	{
		char buffer1[10];
		char buffer2[10];
		sprintf(buffer1, "$%%0%dX", digits);
		sprintf(buffer2, buffer1, valueToChangeTo);
		replacePatchDefinition(patchStart, patchEnd, definitionStart, definitionEnd, std::begin(buffer2), std::begin(buffer2) + strlen(buffer2));
	}

	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename replacementStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, replacementStringIteratorType valueToChangeToStart, replacementStringIteratorType valueToChangeToEnd)
	{
		std::string searchStr = std::string("!") + std::string(definitionStart, definitionEnd);
		auto index = std::search(patchStart, patchEnd, searchStr.begin(), searchStr.end() - 1);

		if (index == patchEnd) 
			throw std::runtime_error("Patch definition could not be found.");

		index += searchStr.length() - 1;

		bool foundEquals = false;

		while (index < patchEnd)
		{
			if (*index == '=')
			{
				foundEquals = true;
				break;
			}
			else if (*index == '\n' || !std::isspace(*index))
			{
				throw std::runtime_error("Patch definition was found but was malformed.");
			}
			index++;
		}

		index++;

		if (foundEquals == false) throw std::runtime_error("Patch definition was found but was malformed.");

		bool foundDefinitionName = false;

		while (index < patchEnd)
		{
			if (*index == '\n')
			{
				throw std::runtime_error("Patch definition was found but was malformed.");
			}
			else if (!std::isspace(*index))
			{
				foundDefinitionName = true;
				break;
			}
			index++;
		}
		if (foundDefinitionName == false) throw std::runtime_error("Patch definition was found but was malformed.");

		
		auto originalDefinitionStart = index;
		int charsToReplace = 0;

		while (index < patchEnd)
		{
			if (!std::isspace(*index))
			{
				foundDefinitionName = true;
			}

			if (std::isspace(*index))
			{
				break;
			}
			index++;
			charsToReplace++;
		}

		int newDefinitionLength = std::distance(valueToChangeToStart, valueToChangeToEnd);

		if (charsToReplace != newDefinitionLength) 
			throw std::runtime_error("The value for the patch definition was not the same size as the given string to replce it with.");

		std::copy_n(valueToChangeToStart, charsToReplace, originalDefinitionStart);
	}





	template <typename dataType>
	std::vector<dataType> compileToBinary(const char *patchFilepath, std::vector<PatchErrorData<std::string>> &errorDataOutput)
	{
		std::vector<dataType> ret;
		compileToBinary(filePath, std::back_inserter(ret), errorDataOutput);
	}
}