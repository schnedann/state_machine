/******************************************************************************/
/** Compile Time Assertions                                                  **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2021                                               **/
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

#ifndef COMPILE_GUARDS_H
#define COMPILE_GUARDS_H

#include <type_traits>
#include "dtypes.h"

namespace Compile{

namespace Guards{

template<typename T> constexpr void IsSigned(){
  static_assert (std::is_signed<T>(), "Error, signed Type expected");
}

template<typename T> constexpr void IsUnsigned(){
  static_assert (std::is_unsigned<T>(), "Error, Signed Integer expected");
  static_assert (!std::is_enum<T>(), "Error, must not be of enum type expected");
}

template<typename T> constexpr void IsInteger(){
  static_assert (std::is_integral<T>(), "Error, Integer expected");
  static_assert (!std::is_enum<T>(), "Error, must not be of enum type expected");
}

template<typename T> constexpr void IsFloatType(){
  static_assert (std::is_floating_point<T>(), "Error, Floating Point expected");
}

template<typename T> constexpr void isFundamental(){
  static_assert (std::is_fundamental<T>(), "Error, arithmetic type, void, or nullptr_t expected");
}

template<typename T> constexpr void isArithmetic(){
  static_assert (std::is_arithmetic<T>(), "Error, integral type or a floating-point type or a cv-qualified version thereof expected");
}

} //namespace

} //namespace

#endif
