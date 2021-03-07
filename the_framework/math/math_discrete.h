/******************************************************************************/
/** Discrete Integer Mathematics         **/
/** collected and / or implemented by       **/
/** Danny Schneider, 2017-2020        **/
/******************************************************************************/

/*
 * Copyright 2017-2020,Danny Schneider
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

#ifndef MATH_DISCRETE_H
#define MATH_DISCRETE_H

#include "dtypes.h"
#include "compile_guards.h"
#include "bitmacros.h"

//----------------------------------------
//----- Averaging
//----------------------------------------

/*
 * Overflow Save Calculation of Mean Value of two Numbers
 * C = B + ((A-B)/2), Where A > B
 * Code automaticliy choose which is A, which B...
 *
 * For Consecutive Calculations of Mean for 'Value' do:
 * init:        C=0; or C=dMEAN2(Min,Max);
 * consecutive: C=dMEAN2(Value,C);
 */
#define dMEAN2(_x,_y) (((_x)>(_y))?(_y+(((_x)-(_y))>>1LLU)):(_x+(((_y)-(_x))>>1LLU)))

//----------------------------------------
// Median of Three
//----------------------------------------

/*
  x>y>z --> y
  x>z>y --> z
  z>x>y --> x
  y>x>z --> x
  y>z>x --> z
  z>y>x --> y
*/
#define dMEDIAN3(_x,_y,_z) (((_x)>(_y))?(((_y)>(_z))?(_y):(((_x)>(_z))?(_z):(_x))):(((_x)>(_z))?(_x):(((_y)>(_z))?(_z):(_y))))


//----------------------------------------

namespace Math{

namespace Discrete{

template<typename T> T ofs_mean(T const _x, T const _y){
  return dMEAN2(_x,_y);
}

template<typename T> T median_of_three(T const _x, T const _y, T const _z){
  return dMEDIAN3(_x,_y,_z);
}

//-----

template<typename T> constexpr bool odd(T const _x){
  Compile::Guards::IsInteger<T>();
  return ((_x&1)>0);
}

template<typename T> constexpr bool even(T const _x){
  Compile::Guards::IsInteger<T>();
  return ((_x&1)==0);
}

//-----

/**
 * @brief multiply_low_part - Multiplication only uses half of Type T
 *  e.g. if your CPU only supports N/2 bits, this calculates the lower bits
 *  normaly thrown away when executing a N/2 Type multiplication
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr auto multiply_low_part(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  u8 const hbits = sizeof(T)<<2;
  T const a = _x>>hbits;
  T const b = _x&((u64(1)<<hbits)-1);
  T const c = _y>>hbits;
  T const d = _y&((u64(1)<<hbits)-1);
  return (((b*c)<<hbits)+((a*d)<<hbits)+(b*d));
}

/**
 * @brief multiply_high_part - Multiplication only uses half of Type T
 *  e.g. if your CPU only supports N/2 bits, the result equals the
 *  normal result when executing a N/2 Type multiplication
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr auto multiply_high_part(T const _x, T const _y) noexcept ->T{
  Compile::Guards::IsInteger<T>();
  //u8 const  bits = sizeof(T)<<3;
  u8 const hbits = sizeof(T)<<2;
  T const a = _x>>hbits;
  T const b = _x&((u64(1)<<hbits)-1);
  T const c = _y>>hbits;
  T const d = _y&((u64(1)<<hbits)-1);
  return ((((b*d)>>hbits)+(a*d)+(b*c))>>hbits)+(a*c);
}

//-----

/*
 x =              0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
floor(log2(x)) -Inf  0  1  1  2  2  2  2  3  3  3  3  3  3  3  3  4  4  4  4  4  4  4  4  4  4
 ceil(log2(x)) -Inf  0  1  2  2  3  3  3  3  4  4  4  4  4  4  4  4  5  5  5  5  5  5  5  5  5
*/

/**
 * @brief log2
 *
 * @param  x
 * @return
 */
template<typename T> constexpr T log2(T const _x){
  Compile::Guards::IsUnsigned<T>();

  //get number of bits divided by 2 of type T
  constexpr u8 const halfbits = sizeof(T)<<2;
  T res = 0;

  if(_x>0){
    //Mask with all bits set in the upper half and zeros in the lower
    T mask = Math::Boolean::ARITHSHL<T>(Math::Boolean::GETFULLMASK<T>(halfbits),halfbits);
    //init shift by halfbits
    u8 shift = halfbits;

    while(shift>1){
      if(_x&mask){
        res += shift;
      }
      shift >>= 1;
      mask ^= (mask >> shift);
    };

    if(_x&mask){
      res += shift;
    }
    mask ^= (mask >> 1);
    if(_x&mask){
      res += shift;
    }
   //mask ^= (mask >> 1);
    --res;
  }
  return res;
}



/**
 * @brief countbitsset - Highly Optimized bit counting
 *     using a temporary variable c
 * @param  v
 * @return
 */
template<typename T> constexpr u8 countbitsset(T const v){
  Compile::Guards::IsUnsigned<T>();
  /* 64-bit recursive reduction using SWAR...
  but first step is mapping 2-bit values
  into sum of 2 1-bit values in sneaky way
  */
  T x = v;
  x -=  ((x >> 1) & T(0x5555555555555555ull));
  x  = (((x >> 2) & T(0x3333333333333333ull)) + (x & T(0x3333333333333333ull)));
  x  = (((x >> 4) + x) & T(0x0f0f0f0f0f0f0f0full));
  x +=   (x >> 8);
  x +=   (x >> 16);
  return u8(x & T(0x000000000000001Ful));
}

/**
 * Integer leading Zeros Count
 */
template<typename T> constexpr u8 leadingzeros(T const _x){
  constexpr u8 const tbits = Math::Boolean::GETBITSOFTYPE<T>();
  T v = _x;
  v |= (v >> 1);
  v |= (v >> 2);
  v |= (v >> 4);
  if(tbits> 8) v |= (v >> 8);
  if(tbits>16) v |= (v >> 16);
  if(tbits>32) v |= (v >> 32);
  return(tbits - countbitsset<T>(v));
}

/**
 * @brief is_power_of_2
 *
 * @param  v
 * @return
 */
template<typename T> constexpr bool is_power_of_2(T const v){
  Compile::Guards::IsUnsigned<T>();
  return ((v&(v-1))==0);
}

/**
 * @brief DiscreteMath::nextpow2
 * @param _x
 * @return
 */
template<typename T> T nextpow2(T const _x){
  Compile::Guards::IsUnsigned<T>();
  constexpr u8 const tbits = Math::Boolean::GETBITSOFTYPE<T>();
  T lx = _x;
  --lx;
  lx |= lx >> 1;
  lx |= lx >> 2;
  lx |= lx >> 4;
  if(tbits> 8) lx |= lx >> 8;
  if(tbits>16) lx |= lx >> 16;
  if(tbits>32) lx |= lx >> 32;
  ++lx;
  return lx;
}

/**
 * @brief intpower
 *  Integer Power x^p
 * @param value
 * @param step
 * @return
 */
template<class T> constexpr auto intpower(T const x, T const p) -> T{
  Compile::Guards::IsUnsigned<T>();
  T res = x;
  if(0 == p){
    res = 1;
  }else if(p>1){
    T tmp = intpower(x, static_cast<T>(p>>1));
    if(0==(p%2)){
   res = tmp * tmp;
    }else{
   res *= tmp * tmp;
    }
  }
  return res;
}

/**
 * @brief minimum
 *
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr bool minimum(T const x, T const y){
  Compile::Guards::IsUnsigned<T>();
  return x+(((y-x)>>(Math::Boolean::GETBITSOFTYPE<T>()-1))&(y-x));
}

/**
 * @brief maximum
 *
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr bool maximum(T const x, T const y){
  Compile::Guards::IsUnsigned<T>();
  return x-(((x-y)>>(Math::Boolean::GETBITSOFTYPE<T>()-1))&(x-y));
}

/**
 * @brief dec2gray
 *
 * @param  v
 * @return
 */
template<typename T> constexpr T dec2gray(T const v){
  Compile::Guards::IsUnsigned<T>();
  return v^(v>>1);
}

//----------------------------------------

/**
 * @brief ceildiv - Integer Division with Rouning up
 *
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr T ceildiv(T const x, T const y){
  Compile::Guards::IsInteger<T>();
  return ((y>0)?((x+y-1)/y):(0));
  //return ((y>0)?(1+((x-1)/y)):(0)); //Alternate Code
}

// so far Code does not working with signed numners...
template<typename T> constexpr T ceildiv_v2(T const _x, T const _y){
  Compile::Guards::IsInteger<T>();
  return ((_y)!=0)?(((_x)>0)?(((((_x)-1)/(_y))+1)):((_x)/(_y))):(0);
}

//----------------------------------------

/**
 * @brief nearrounddiv
 *
 * @param  x
 * @param  y
 * @return
 */
template<typename T> constexpr bool nearrounddiv(T const x, T const y){
  Compile::Guards::IsInteger<T>();
  return ((x+(y>>1))/y);
}

/**
 * @brief uiround
 *  Round to nearest unsigned int with steping step - expecting T as unsigned Integer Type
 * @param value
 * @param step
 * @return
 */
template<class T> constexpr auto uiround(T const value, T const step) -> T{
  Compile::Guards::IsUnsigned<T>();
  T res = value;
  T tmp = value%step;
  if(tmp>=(step>>1)){
    res += (step-tmp);
  }else{
    res -= tmp;
  }
  return res;
}

//-----

/**
 *
 */
template<class T> auto linear_interpol(T const x1, T const x2, T const y1, T const y2, T const xz) -> T{
  T res = y1;
  T intermeadiate_result1 = y2-y1;
  T intermeadiate_result2 = xz-x1;
  decltype (intermeadiate_result1 * intermeadiate_result1) intermeadiate_result3 = intermeadiate_result1 * intermeadiate_result1;
  decltype (intermeadiate_result1 * intermeadiate_result1) intermeadiate_result4 = x2-x1;
  T intermeadiate_result5 = static_cast<T>(intermeadiate_result3/intermeadiate_result4);
  res += intermeadiate_result5;
  return res;
}

//-----

u8 log2u32(u32 const _x);

/*
 * Next largest Integer witch is a Power of 2
 * Example: 3,4 --> 4
 */
u64 nlpo2(u64 const data, u8 const bits);

/*
 * Bitreverse
 * Example: 4Bits 3 --> 12
 */
u64 bitreverse(u64 const data, u8 const bits);

//-----
u16 intAbs16(s16 const v);
u32 intAbs32(s32 const v);
u64 intAbs64(s64 const v);
u8 hamming_weight(u16 const val);
//-----
s16 sdivceil(s16 const a, s16 const b);
u16 udivceil(u16 const a, u16 const b);
//-----
s32 floorLog2_32(u32 const n);
s16 floorLog2_16(u16 const n);
//-----
float  Q_rsqrt( float const  number );
double Q_rsqrt( double const number );
//-----

/**
 * @brief integerSqrt
 *        https://en.wikipedia.org/wiki/Integer_square_root
 * @param x
 * @return floor(sqrt(x)) for x = unsigned Integer
 */
template<typename T> T integerSqrt(T const x){
  Compile::Guards::IsUnsigned<T>();
  T res = x;

  if(x>1){
    u8  shift = 0;
    T nShifted = 0;
    //We check for nShifted being x,
    //since some implementations of
    //logical right shifting shift modulo the word size.
    do{
      shift += 2;
      nShifted = x >> shift;
    }while((nShifted>0) && (nShifted!=x));

    shift -= 2;

    //Find digits of result.
    res = 0;
    bool run = true;
    while(run){
      res <<= 1;
      T candidateResult = res + 1;
//TODO: Overflow safe math injection here
      if ((candidateResult*candidateResult) <= (x >> shift)){
        res = candidateResult;
      }
      if(shift>0) shift -= 2;
      else break;
    }
  }
  return res;
}

//-----

/**
 * Greatest Common Divider
 */
template<typename T> T gcd(T a, T b){
  Compile::Guards::IsUnsigned<T>();
  T res = 0;
  if(b == 0){
    res = a;
  }else{
    res = gcd<T>(b, (a%b));
  }
  return res;
}

/**
 * Least Common Multiple
 */
template<typename T> T lcm(T a, T b){
  Compile::Guards::IsUnsigned<T>();
  T res = 0;
  res  = a * b;
  res /= gcd<T>(a,b);
  return res;
}

//-----

} //namespace

} //namespace

#endif // MATH_DISCRETE_H
