#pragma once
#include "Prerequisites.h"

RB_NAMESPACE_BEGIN

template <bool> struct TEnumAsByte_EnumClass;
template <> struct TEnumAsByte_EnumClass<false> {};

/**
 * Template to store enumeration values as bytes in a type-safe way.
 */
template<class TEnum>
class TEnumAsByte
{
public:
	typedef TEnum EnumType;

	TEnumAsByte() = default;
	TEnumAsByte(const TEnumAsByte&) = default;
	TEnumAsByte& operator=(const TEnumAsByte&) = default;

	/**
		* Constructor, initialize to the enum value.
		*
		* @param InValue value to construct with.
		*/
	TEnumAsByte(TEnum InValue)
		: Value(static_cast<uint8>(InValue))
	{ }

	/**
		* Constructor, initialize to the int32 value.
		*
		* @param InValue value to construct with.
		*/
	explicit TEnumAsByte(int32 InValue)
		: Value(static_cast<uint8>(InValue))
	{ }

	/**
		* Constructor, initialize to the int32 value.
		*
		* @param InValue value to construct with.
		*/
	explicit TEnumAsByte(uint8 InValue)
		: Value(InValue)
	{ }

public:
	/**
		* Compares two enumeration values for equality.
		*
		* @param InValue The value to compare with.
		* @return true if the two values are equal, false otherwise.
		*/
	bool operator==(TEnum InValue) const
	{
		return static_cast<TEnum>(Value) == InValue;
	}

	/**
		* Compares two enumeration values for equality.
		*
		* @param InValue The value to compare with.
		* @return true if the two values are equal, false otherwise.
		*/
	bool operator==(TEnumAsByte InValue) const
	{
		return Value == InValue.Value;
	}

	/** Implicit conversion to TEnum. */
	operator TEnum() const
	{
		return (TEnum)Value;
	}

public:

	/**
		* Gets the enumeration value.
		*
		* @return The enumeration value.
		*/
	TEnum GetValue() const
	{
		return (TEnum)Value;
	}

private:

	/** Holds the value as a byte. **/
	uint8 Value;
};

RB_NAMESPACE_END