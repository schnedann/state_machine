/******************************************************************************/
/** Simple Hash Functions / Hash-Function building Blocks                    **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2020                                               **/
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

#ifndef SIMPLE_HASH_FCT_H
#define SIMPLE_HASH_FCT_H

#include "dtypes.h"
#include "compile_guards.h"
#include "bitmacros.h"

namespace Math{

namespace Hashing{

//----------------------------------------
//----- HASHING
//----------------------------------------

/*
 * Square a Number and take bits out of the middle,
 * these Bits are affected by all bit positions of
 * utilize 64Bit temporary (max type for T is 32Bit!!!)
 * the original Number
 * @param _x - number to square
 * @param _y - covered bits
 */
template<typename T> auto midsquare_64(T const& _x, T const& _bits) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return static_cast<T>(((u64(_x)*u64(_x))>>(sizeof(T)<<1))&Math::Boolean::GETFULLMASK<u64>(_bits));
}

template<typename T> auto doublehash(T const& _x, T const& _y, T const& iter, T const& _M) noexcept ->T{
  Compile::Guards::IsUnsigned<T>();
  return (_x+T((u64(iter)*u64(iter))*_y)%_M);
}

}

}

#endif // SIMPLE_HASH_FCT_H
