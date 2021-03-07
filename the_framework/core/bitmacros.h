/******************************************************************************/
/** ----- Bit M A C R O S -----     Integer Bitmanipulation Macros           **/
/** if not stated otherwise:                                                 **/
/** Assume Unsigned Integer Numbers and Two'sCompliment Arithmethic          **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2021                                               **/
/**                                                                          **/
/** Sources & Ideas for Algorithms used here:                                **/
/** http://aggregate.org/MAGIC/                                              **/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/******************************************************************************/

/*
 * Copyright 2017-2021,Danny Schneider
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BYTEMACROS_H
#define BYTEMACROS_H

//--------------------------------------------------

#include <type_traits>
#include "dtypes.h"
#include "compile_guards.h"

//--------------------------------------------------

//----- Macros
//      m... -> normal Macro
//      ma.. -> auto Assignment to first Operand

// --- Portable Bitmask for X Bits Generation ---
// => e.g. mBitMask(32) wont use shift 32,
// so its safe on a 32Bit Computer
#define mBitMask(_b) ((((1ULL<<((_b)-1))-1)<<1)|1ULL)

// --- Logic ---

#define mINVERT(_x)     (~(_x))
#define maINVERT(_x)    ((_x)~=(_x))

#define mXOR(_x,_y)     ((_x)^((_y)))
#define maXOR(_x,_y)    ((_x)^=((_y)))

#define mOR(_x,_y)      ((_x)|(_y))
#define maOR(_x,_y)     ((_x)|=(_y))

#define mAND(_x,_y)     ((_x)&(_y))
#define maAND(_x,_y)    ((_x)&=(_y))

#define mNAND(_x,_y)     (~mAND((_x),(_y)))
#define maNAND(_x,_y)    ((_x)=mNAND((_x),(_y)))

// --- Bit Manipulation ---

#define maSetBit(_v,_b) ((_v)|=(1U<<(_b)))
#define mGetBit(_v,_b)  ((_v)&(1U<<(_b)))
#define maClrBit(_v,_b) ((_v)&=~(1U<<(_b)))

// ---

#define maskBits(_x,_y)                   (mAND((_x),(_y)))
#define maskBitsExcept(_x,_y,_be)         (mAND(mAND((_x),(_y)),mINVERT(_be)))
#define maskTestBits(_x,_y,_b)            (mAND((_x),(_y))==(_b))
#define maskTestBitsExcept(_x,_y,_b,_be)  (maskBitsExcept(_x,_y,_be)==(_b))

// ---

#define maskBitsSet(_x,_y)                (maOR((_x),(_y)))
#define maskBitsSetExcept(_x,_y,_be)      (maOR((_x),mAND((_y),mINVERT(_be))))
#define maskBitsClear(_x,_y)              (maAND((_x),mINVERT(_y)))
#define maskBitsClearExcept(_x,_y,_be)    (maAND((_x),mINVERT(mAND((_y),mINVERT(_be)))))
#define maskBitsToggle(_x,_y)             (maXOR((_x),(_y)))
#define maskBitsToggleExcept(_x,_y,_be)   (maXOR((_x),mAND((_y),mINVERT(_be))))

// --- Shift Operations ---

#define mSHR(_v,_b)     ((_v)>>(_b))
#define maSHR(_v,_b)    ((_v)>>=(_b))

#define mSHL(_v,_b,_i)  (((_b)==0)?(_v):((((_v)<<(_b))&(0U-2))|(1U&(_i))))
#define maSHL(_v,_b,_i) ((_v)=(mSHL(_v,_b,_i)))

// --- Get / Change a Bit (to State i) ---

#define maSetBinary(_v,_b,_i) ((_v)=(((_v)&(~(1U<<(_b))))|((_i)<<(_b))))
#define mGetBinary(_v,_b)     (((_v)>>(_b))&1U)

// --- Word <--> Byte Access ---
#define mHByte(_v) (((_v)>>8)&0x00FFu)
#define mLByte(_v) ((_v)&0x00FFu)

#define mSwapByte(_w)    (mOR(mAND(mSHR((_w),8),mBitMask(8)),mSHL(mAND((_w),mBitMask(8)),8,0)))
#define mToWord(_b1,_b2) (mOR(mAND((_b1),mBitMask(8)),mSHL(mAND((_b2),mBitMask(8)),8,0)))

//--------------------------------------------------

namespace Math{

namespace Boolean{

//--------------------------------------------------

/**
 * Test Integer if it is Even (LSB==0)
 */
template<typename T> constexpr auto IS_EVEN(T const _x) noexcept ->bool{
  Compile::Guards::IsInteger<T>();
  return ((_x&1)==0);
}
/**
 * Test Integer if it is Odd (LSB==1)
 */
template<typename T> constexpr auto IS_ODD(T const _x) noexcept ->bool{
  Compile::Guards::IsInteger<T>();
  return ((_x&1)>0);
}

//--------------------------------------------------

/**
 * return lowest bit set in integer
 */
template<typename T> constexpr auto GET_LOWESTBIT(T const _x) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x^(_x-1))&_x);
}

/*
 * Get Lowest Bit Set in an Integer (result is One Hot)
 * Fastest posibility to encode / calculate Priorities!!!
 */
// #define dLOW_ONE(_x) ((_x)&((~(_x))+1))
template<typename T> constexpr auto GET_LOW_ONE(T _x) noexcept ->T{
  return (_x)&((~(_x))+1);
}

//-----

/**
 * Is Integer a Power of 2 (is Integer One Hot encoded?)
 * returns true if non negative integer is power of two
 */
template<typename T> constexpr auto IS_ONEHOT(T const _x) noexcept ->bool{
  Compile::Guards::IsUnsigned<T>();
  return (_x>0)?((_x&(_x-1))==0):(false);
}

//--------------------------------------------------

/**
 *
 */
template<typename T> constexpr T right_propagate_rightmost_1bit(T const _x){
  Compile::Guards::IsUnsigned<T>();
  return (_x | (_x-1));
}

/**
 *
 */
template<typename T> constexpr T isolate_rightmost_0bit(T const _x){
  Compile::Guards::IsUnsigned<T>();
  return (~_x & (_x+1));
}

/**
 *
 */
template<typename T> constexpr T set_rightmost_0bit(T const _x){
  Compile::Guards::IsUnsigned<T>();
  return (_x | (_x+1));
}

//--------------------------------------------------

/**
 * number of bits of given Type T
 */
template<typename T> constexpr auto GETBITSOFTYPE() noexcept ->size_t{
  return sizeof(T)<<3;
}

/**
 * satturate "bits" at value of number of bits of given Type T
 */
template<typename T> constexpr auto GETMAXBITS(size_t bits) noexcept ->T{
  return (GETBITSOFTYPE<T>()<bits)?(GETBITSOFTYPE<T>()):(bits);
}

//--------------------------------------------------

/**
 * Get Mask for single Bit 0..N-1
 *  0 - 1
 *  1 - 2
 *  2 - 4
 *  3 - 8
 */
template<typename T> constexpr auto GETMASKBIT(size_t bits) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return T(1)<<(GETMAXBITS<T>(bits));
}

/**
 * if we have 4 bits, then the mask for the bit 4 is: 10000,
 * which is the decimal value of 16. And 4 bits allow to represent
 * a "space" of 16 different values
 *
 * !!!WARNING!!! BITSPACE<uAB>(AB) will not fit in type uAB
 * 0 -    1
 * 1 -   10
 * 2 -  100
 * 3 - 1000
 */
template<typename T> constexpr auto BITSPACE(u8 bits) noexcept ->T{
  return GETMASKBIT<T>(bits);
}

/**
 * One Hot coded Mask --> highest Bit 0...N-1 Set only
 * Parameter: Bits of Mask
 * 0 -    0
 * 1 -    1
 * 2 -   10
 * 3 -  100
 */
template<typename T> T MASK_MSB(u8 bits){
  return (bits>0)?(BITSPACE<T>(bits-1)):(0);
}

/**
 * Overflow Save set all Bits for space of x Bits 0..N
 * 0 -   0
 * 1 -   1
 * 2 -  11
 * 3 - 111
 */
template<typename T> constexpr auto GETFULLMASK(T const bits) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (bits<2)?(bits):((((GETMASKBIT<T>(bits-1)-1)<<1)|1));
  //return (bits>1)?(((BITSPACE<T>(bits-1)-1)<<1)|1):(bits);
}

/**
 * Overflow Save set all Bits for space of x Bits 1..N
 * 0 -   0
 * 1 -   1
 * 2 -  11
 * 3 - 111
 */
template<typename T> constexpr auto GETFULLMASK_v2(T const _x) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return right_propagate_rightmost_1bit<T>(GETMASKBIT<T>(_x-1));
}

/**
 * Overflow Save Mask with all Bits Set
 * on a X Bit CPU X Bit Masks without Integer Overflow
 * Parameter: Bits of Mask
 */
/*template<typename T> [[deprecated]] T OFS_MAX(u8 bits){
  return (bits>0)?(((BITSPACE<T>(bits-1)-1)<<1)|1):(0);
}*/

//-----

/**
 * Apply Mask
 */
template<typename T> constexpr auto MASKBITS(T const _x,T const&_y)->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x)&(_y));
}

//--------------------------------------------------

//----------------------------------------
// http://www.fefe.de/intof.html (modified/extended)
//----------------------------------------

//Overflow Save Calculation of max. Unsigned Integer with x-Bits
template<typename T> constexpr auto MAXUINT(u8 bits)->T{
//#define MAXUINT(_X) ((((1U<<((_x)-1))-1)<<1)+1)
  return (((T(1)<<(bits-1))-1)<<1)|1;
}

/**
 * Finding the minimum and maximum values for a given integer type
 */

/**
 * @brief - (highest positive Value /2) + 1 --> the MSB is set
 */
template<typename T> constexpr auto __HALF_MAX_UNSIGNED()->T{
  Compile::Guards::IsUnsigned<T>();
//#define __HALF_MAX_UNSIGNED(type) (static_cast<type>(1)<<((sizeof(type)<<3)-1))
  return T(1)<<((sizeof(T)<<3)-1);
}

/**
 * @brief - (highest positive Value /2) + 1 --> the Bit next to the sign is set
 */
template<typename T> constexpr auto __HALF_MAX_SIGNED()->T{
  Compile::Guards::IsSigned<T>();
//#define __HALF_MAX_SIGNED(type)   (static_cast<type>(1)<<((sizeof(type)<<3)-2))
  return T(1)<<((sizeof(T)<<3)-2);
}

/**
 * @brief - highest positive Value
 */
template<typename T> constexpr auto __MAX_UNSIGNED()->T{
//#define __MAX_UNSIGNED(type)      ((__HALF_MAX_UNSIGNED(type)<<1)+__HALF_MAX_UNSIGNED(type))
  return ((__HALF_MAX_UNSIGNED<T>()-1)<<1)+1; //__HALF_MAX_UNSIGNED<T>();
}

/**
 * @brief
 */
template<typename T> constexpr auto __MAX_SIGNED()->T{
//#define __MAX_SIGNED(type)        (__HALF_MAX_SIGNED(type)-1+__HALF_MAX_SIGNED(type))
  return __HALF_MAX_SIGNED<T>()-1+__HALF_MAX_SIGNED<T>();
}

/**
 * @brief
 */
template<typename T> constexpr auto __MIN_SIGNED()->T{
//#define __MIN_SIGNED(type)        (-1-__MAX_SIGNED(type))
  return -1-__MAX_SIGNED<T>();
}

/**
 * @brief
 */
template<typename T> constexpr auto __MIN()->T{
//#define __MIN(type) (static_cast<type>(-1) < ((1)?(__MIN_SIGNED(type)):(static_cast<type>(0))))
  return T(-1) < ((1)?(__MIN_SIGNED<T>()):(T(0)));
}

/**
 * @brief
 */
template<typename T> constexpr auto __MAX()->T{
//#define __MAX(type) (static_cast<type>(~__MIN(type)))
  return T(~__MIN<T>());
}

//--------------------------------------------------

/**
 * Bitwise And
 */
template<typename T> constexpr auto AND(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x)&(_y));
}

/**
 *Bitwise Or
 */
template<typename T> constexpr auto OR(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x)|(_y));
}

/**
 *Bitwise Xor
 */
template<typename T> constexpr auto XOR(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x)^(_y));
}

//--------------------------------------------------

/**
 * in _x set set bits from _y
 */
template<typename T> constexpr auto SETBITS(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (OR<T>((_x),(_y)));
}

/**
 * in_x clear set bits from _y
 */
template<typename T> constexpr auto CLRBITS(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (AND<T>((_x),~(_y)));
}

/**
 * bits set in _x and _y will be cleared, bits cleared in _x and _y will be set
 */
template<typename T> constexpr auto TOGGLEBITS(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (XOR<T>((_x),(_y)));
}

/**
 * test if _x and _y are binary equal
 */
template<typename T> constexpr auto EQUAL(T const _x, T const _y) noexcept ->bool{
  Compile::Guards::IsUnsigned<T>();
  return (((_x)^(_y))==0);
}

/**
 * test if all bits in _y are set in _x
 */
template<typename T> constexpr auto TESTBITS(T const _x, T const _y) noexcept ->bool{
  Compile::Guards::IsUnsigned<T>();
  return EQUAL<T>(AND<T>((_x),(_y)),_y);
}

//--------------------------------------------------

/**
 * @brief - ARITHSHL
 */
template<typename T> constexpr auto ARITHSHL(T const _x, u8 const& _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  return ((_x)<<(_y));
}

/**
 * @brief - ARITHSHR
 */
template<typename T> constexpr auto ARITHSHR(T const _x, u8 const& _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  return ((_x)>>(_y));
}

/**
 * @brief - ROTL
 */
template<typename T> constexpr auto ROTL(T const _x, u8 const& _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  constexpr auto const TBits = u8(Math::Boolean::GETBITSOFTYPE<T>());
  return (((_x)<<(_y))|((_x)>>(TBits-_y)));
}

/**
 * @brief - ROTR
 */
template<typename T> constexpr auto ROTR(T const _x, u8 const& _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  constexpr auto const TBits = u8(Math::Boolean::GETBITSOFTYPE<T>());
  return (((_x)>>(_y))|((_x)<<(TBits-_y)));
}

//--------------------------------------------------

/**
 * from _x extract field from position _lsb with length _len
 */
template<typename T> constexpr auto GETVALUE(T const _x, T const _lsb, T const _len) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (((_x)>>(_lsb))&GETFULLMASK<T>(_len));
}

/**
 * Replace Field - Replace a field in x with same field from y
 */
template<typename T> constexpr auto REPLFIELD(T const _x, T const _y, T const _lsb, T const _len) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  T const mask = ~(GETFULLMASK<T>(_len)<<_lsb);
  return (_y)|(_x&mask);
}

/**
 * in _x replace field from position _lsb with length _len with data from _y
 */
template<typename T> constexpr auto SETVALUE(T& _x, T const _y, T const _lsb, T const _len) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  T shift_y = ARITHSHL<T>(_y,_lsb);
  return _x=REPLFIELD<T>(_x,shift_y,_lsb,_len);
}

//--------------------------------------------------

/**
 *
 */
template<typename T> constexpr auto RANGELIMIT(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return ((_x)%(_y));
}

//--------------------------------------------------

/**
 * Zweierkomplement - Two's Complement
 */
template<typename T> constexpr auto ZQ(T const _x) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  return ((~(_x))+1);
}

/**
 * if number is negative, get the same number with positiv sign
 */
template<typename T> auto INTABS(T const _x) -> typename std::make_unsigned<T>::type{
  Compile::Guards::IsInteger<T>();
  return static_cast<typename std::make_unsigned<T>::type>((_x<0)?(ZQ<T>(_x)):(_x));
}

//--------------------------------------------------

//Alternate Versions

/**
 * Get Lowest Bit Set in an Integer (One Hot)
 * Fast posibility to encode / calculate Priorities!!!
 */
/*template<typename T> auto lowest_one_set(T const _x) noexcept ->T{
  return ((_x)&((~(_x))+1));
}*/

//--------------------------------------------------

/**
 * Is Integer a Power of 2 (is Integer One Hot encoded?)
 */
/*template<typename T> auto is_power_of_2(T const _x) noexcept ->bool{
  return ((((_x)&((_x)-1))==0)?(true):(false));
}*/

//--------------------------------------------------

//--------------------------------------------------

} //namespace

} //namespace

#endif // BYTEMACROS_H
