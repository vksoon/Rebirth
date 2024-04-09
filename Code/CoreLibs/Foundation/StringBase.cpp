#include "StringBase.h"

RB_NAMESPACE_BEGIN

template<typename _Type>
_Type UTF8To32(_Type begin, _Type end, char32_t& output, char32_t invalidChar = 0)
{
	// Nothing to parse
	if (begin >= end)
		return begin;

	// Determine the number of bytes used by the character
	UINT32 numBytes;

	UINT8 firstByte = (UINT8)*begin;
	if (firstByte < 192)
		numBytes = 1;
	else if (firstByte < 224)
		numBytes = 2;
	else if (firstByte < 240)
		numBytes = 3;
	else if (firstByte < 248)
		numBytes = 4;
	else if (firstByte < 252)
		numBytes = 5;
	else // < 256
		numBytes = 6;

	// Not enough bytes were provided, invalid character
	if ((begin + numBytes) > end)
	{
		output = invalidChar;
		return end;
	}

	// Decode the character
	output = 0;
	switch (numBytes)
	{
	case 6: output += (UINT8)(*begin); ++begin; output <<= 6;
	case 5: output += (UINT8)(*begin); ++begin; output <<= 6;
	case 4: output += (UINT8)(*begin); ++begin; output <<= 6;
	case 3: output += (UINT8)(*begin); ++begin; output <<= 6;
	case 2: output += (UINT8)(*begin); ++begin; output <<= 6;
	case 1: output += (UINT8)(*begin); ++begin;
	default: break;
	}

	constexpr UINT32 offsets[6] = { 0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080 };
	output -= offsets[numBytes - 1];

	return begin;
}

/** Converts an UTF-32 encoded character into an (possibly multibyte) UTF-8 character. */
template<typename _Type>
_Type UTF32To8(char32_t input, _Type output, UINT32 maxElems, char invalidChar = 0)
{
	// No place to write the character
	if (maxElems == 0)
		return output;

	// Check if character is valid
	if ((input > 0x0010FFFF) || ((input >= 0xD800) && (input <= 0xDBFF)))
	{
		*output = invalidChar;
		++output;

		return output;
	}

	// Determine the number of bytes used by the character
	UINT32 numBytes;
	if (input < 0x80)
		numBytes = 1;
	else if (input < 0x800)
		numBytes = 2;
	else if (input < 0x10000)
		numBytes = 3;
	else // <= 0x0010FFFF 
		numBytes = 4;

	// Check if we have enough space
	if (numBytes > maxElems)
	{
		*output = invalidChar;
		++output;

		return output;
	}

	// Encode the character
	constexpr UINT8 headers[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	char bytes[4];
	switch (numBytes)
	{
	case 4: bytes[3] = (char)((input | 0x80) & 0xBF); input >>= 6;
	case 3: bytes[2] = (char)((input | 0x80) & 0xBF); input >>= 6;
	case 2: bytes[1] = (char)((input | 0x80) & 0xBF); input >>= 6;
	case 1: bytes[0] = (char)(input | headers[numBytes]);
	default: break;
	}

	output = std::copy(bytes, bytes + numBytes, output);
	return output;
}

/** Converts an UTF-32 encoded character into an UTF-16 character. */
template<typename _Type>
_Type UTF32To16(char32_t input, _Type output, UINT32 maxElems, char16_t invalidChar = 0)
{
	// No place to write the character
	if (maxElems == 0)
		return output;

	// Invalid character
	if (input > 0x0010FFFF)
	{
		*output = invalidChar;
		++output;

		return output;
	}

	// Can be encoded as a single element
	if (input <= 0xFFFF)
	{
		// Check if in valid range
		if ((input >= 0xD800) && (input <= 0xDFFF))
		{
			*output = invalidChar;
			++output;

			return output;
		}

		*output = (char16_t)input;
		++output;
	}
	else // Must be encoded as two elements
	{
		// Two elements won't fit
		if (maxElems < 2)
		{
			*output = invalidChar;
			++output;

			return output;
		}

		input -= 0x0010000;

		*output = (char16_t)((input >> 10) + 0xD800);
		++output;

		*output = (char16_t)((input & 0x3FFUL) + 0xDC00);
		++output;
	}

	return output;
}


/** Converts an UTF-16 encoded character into an UTF-32 character. */
template<typename _Type>
_Type UTF16To32(_Type begin, _Type end, char32_t& output, char32_t invalidChar = 0)
{
	// Nothing to parse
	if (begin >= end)
		return begin;

	char16_t firstElem = (char16_t)*begin;
	++begin;

	// Check if it's a surrogate pair
	if ((firstElem >= 0xD800) && (firstElem <= 0xDBFF))
	{
		// Invalid character
		if (begin >= end)
		{
			output = invalidChar;
			return end;
		}

		char32_t secondElem = (char32_t)*begin;
		++begin;

		if ((secondElem >= 0xDC00) && (secondElem <= 0xDFFF))
			output = (char32_t)(((firstElem - 0xD800) << 10) + (secondElem - 0xDC00) + 0x0010000);
		else // Invalid character
			output = invalidChar;
	}
	else
	{
		output = (char32_t)firstElem;
		return begin;
	}

	return begin;
}

template<typename _Type>
_Type UTF32ToWide(char32_t input, _Type output, UINT32 maxElems, wchar_t invalidChar = 0)
{
	if (sizeof(wchar_t) == 4) // Assuming UTF-32 (i.e. Unix)
	{
		*output = (wchar_t)input;
		++output;

		return output;
	}
	else // Assuming UTF-16 (i.e. Windows)
		return UTF32To16(input, output, maxElems, invalidChar);
}

template<typename _Type>
_Type WideToUTF32(_Type begin, _Type end, char32_t& output, char32_t invalidChar = 0)
{
	if (sizeof(wchar_t) == 4) // Assuming UTF-32 (i.e. Unix)
	{
		output = (char32_t)*begin;
		++begin;

		return begin;
	}
	else // Assuming UTF-16 (i.e. Windows)
		return UTF16To32(begin, end, output, invalidChar);
}

// ToString implementations

WString ToWString(const String& Input)
{
	WString output;
	auto backInserter = std::back_inserter(output);

	auto iter = Input.begin();
	while (iter != Input.end())
	{
		char32_t u32char;
		iter = UTF8To32(iter, Input.end(), u32char);

		UTF32ToWide(u32char, backInserter, 2);
	}

	return output;
}

WString ToWString(const char* source)
{
	return ToWString(String(source));
}

String ToString(const WString& Input)
{
	String output;
	output.reserve(Input.size());

	auto backInserter = std::back_inserter(output);

	auto iter = Input.begin();
	while (iter != Input.end())
	{
		char32_t u32char;
		iter = WideToUTF32(iter, Input.end(), u32char);
		UTF32To8(u32char, backInserter, 4);
	}

	return output;
}

String ToString(const wchar_t* source)
{
	return ToString(WString(source));
}

String ToString(float val, uint16 precision, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

String ToString(double val, uint16 precision, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

String ToString(INT32 val, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

String ToString(UINT32 val, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

String ToString(INT64 val, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

String ToString(UINT64 val, uint16 width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}

// Parse implementations

const String StringUtil::BLANK;
const WString StringUtil::WBLANK;

void StringUtil::Trim(String& str, bool left, bool right)
{
	static const String delims = " \t\r";
	Trim(str, delims, left, right);
}

void StringUtil::Trim(WString& str, bool left, bool right)
{
	static const WString delims = L" \t\r";
	Trim(str, delims, left, right);
}

void StringUtil::Trim(String& str, const String& delims, bool left, bool right)
{
	if (right)
		str.erase(str.find_last_not_of(delims) + 1); // trim right
	if (left)
		str.erase(0, str.find_first_not_of(delims)); // trim left
}

void StringUtil::Trim(WString& str, const WString& delims, bool left, bool right)
{
	if (right)
		str.erase(str.find_last_not_of(delims) + 1); // trim right
	if (left)
		str.erase(0, str.find_first_not_of(delims)); // trim left
}

Array<String> StringUtil::Split(const String& str, const String& delims, unsigned int maxSplits)
{
	return SplitInternal<char>(str, delims, maxSplits);
}

Array<WString> StringUtil::Split(const WString& str, const WString& delims, unsigned int maxSplits)
{
	return SplitInternal<wchar_t>(str, delims, maxSplits);
}

void StringUtil::ToLowerCase(String& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void StringUtil::ToLowerCase(WString& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void StringUtil::ToUpperCase(String& str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

void StringUtil::ToUpperCase(WString& str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

RB_NAMESPACE_END