#pragma once
#include "Prerequisites.h"
#include "MemoryAllocationBase.h"

#include <string>

RB_NAMESPACE_BEGIN

template <typename _Type>
using BasicString = std::basic_string < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;

template <typename _Type>
using BasicStringStream = std::basic_stringstream < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;

using WString = BasicString<wchar_t>;

using String = BasicString<char>;

using U16String = BasicString<char16_t>;

using U32String = BasicString<char32_t>;

using WStringStream = BasicStringStream<wchar_t>;

using StringStream = BasicStringStream<char>;

using U16StringStream = BasicStringStream<char16_t>;

using U32StringStream = BasicStringStream<char32_t>;

RB_CORE_API WString ToWString(const String& source);
RB_CORE_API WString ToWString(const char* source);
RB_CORE_API String ToString(const WString& source);
RB_CORE_API String ToString(const wchar_t* source);

RB_CORE_API String ToString(float val, uint16 precision = 6,
	uint16 width = 0, char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

RB_CORE_API String ToString(double val, uint16 precision = 6,
	uint16 width = 0, char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

RB_CORE_API String ToString(INT32 val, uint16 width = 0,
	char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

RB_CORE_API String ToString(UINT32 val,
	uint16 width = 0, char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

RB_CORE_API String ToString(INT64 val,
	uint16 width = 0, char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

RB_CORE_API String ToString(UINT64 val,
	uint16 width = 0, char fill = ' ',
	std::ios::fmtflags flags = std::ios::fmtflags(0));

template< typename _Type >
_Type ParseNumeric(const String& val, _Type defaultValue = 0)
{
	StringStream str(val);
	_Type ret = defaultValue;
	str >> ret;
	return ret;
}

class RB_CORE_API StringUtil
{
public:
	static void Trim(String& str, bool left = true, bool right = true);
	static void Trim(WString& str, bool left = true, bool right = true);
	static void Trim(String& str, const String& delims, bool left = true, bool right = true);
	static void Trim(WString& str, const WString& delims, bool left = true, bool right = true);

	static Array<String> Split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);
	static Array<WString> Split(const WString& str, const WString& delims = L"\t\n ", unsigned int maxSplits = 0);

	static void ToLowerCase(String& str);
	static void ToLowerCase(WString& str);
	static void ToUpperCase(String& str);
	static void ToUpperCase(WString& str);

	template<typename _Type, class ... _Args>
	static BasicString<_Type> Format(const BasicString<_Type>& format, _Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size <= 0)
		{
			throw std::runtime_error("Error during formatting.");
		}
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return BasicString<_Type>(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside }
	}

public:
	static const String BLANK;
	static const WString WBLANK;

private:
	template <class _Type>
	static Array<BasicString<_Type>> SplitInternal(const BasicString<_Type>& str, const BasicString<_Type>& delims, unsigned int maxSplits)
	{
		Array<BasicString<_Type>> ret;
		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

		unsigned int numSplits = 0;

		// Use STL methods
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == BasicString<_Type>::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != BasicString<_Type>::npos);

		return ret;
	}
};

RB_NAMESPACE_END