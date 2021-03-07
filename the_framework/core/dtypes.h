/******************************************************************************/
/** Compact Datatype Definitions                                             **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2020                                               **/
/******************************************************************************/

/*
 * Copyright (c) 2020, Danny Schneider
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer. (of course you are
 * allowed to extend the above copyright notice by the means of adding your
 * name and date if and only if you have contributed to the source)
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Redistributions of any forked and or modified version of this code in binary
 * form need to make the source code availible upon request. If maintainance of any
 * forked and or modified version of this code is stopped, future access to the code
 * needs to be ensured e.g. by at least uploading it to an public open platform and
 * or the official original repository if availible.
 *
 * 4. Usage of this code (stating Discrimination Against Fields of Endeavor [*1] )
 * neither in source nor in binary form - in any military, intelligence,
 * public surveillance, surveillance state , political supression context is forbidden
 * Simply:
 * do not use this software to harm people or to violate non-derogable human rights
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
 *
 * [*1]: Comment of Conndition 4, yes this conndition violates the Open Source Definition
 * But as an Engineer with a german background I do believe you are responsible how the
 * things are used you develop. So yes, even if Conndition 4 might be circumvented and/or
 * be ignored by bad people, I want to state with this liecense, that my intend is to not
 * have the software used to harm people, and I want to ask you to respect this.
 */

#ifndef DTYPES_H
#define DTYPES_H

#ifdef __cplusplus
  // System Includes
  #include <cstdint>
  #include <cstddef>
  #include <utility>

//--------------------------------------------------
// normal Types
//--------------------------------------------------

using  u8 = uint8_t ;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using  s8 = int8_t ;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

//--------------------------------------------------
// fast Types
//--------------------------------------------------

using  uf8 = uint_fast8_t ;
using uf16 = uint_fast16_t;
using uf32 = uint_fast32_t;
using uf64 = uint_fast64_t;

using  sf8 =  int_fast8_t ;
using sf16 =  int_fast16_t;
using sf32 =  int_fast32_t;
using sf64 =  int_fast64_t;

//--------------------------------------------------
// Multi-return-Types
//--------------------------------------------------

template<typename T> using with_error_t = std::pair<bool,T>;
template<typename T> bool has_error(with_error_t<T> const& rt){
  return rt.first;
}
template<typename T> T& get_data(with_error_t<T>& rt){
  return rt.second;
}
template<typename T> with_error_t<T> make_with_error(bool const err, T const& data){
  return with_error_t<T>(err,data);
}

#else
  #include <stdint.h>
  #include <stddef.h>

//--------------------------------------------------
// normal Types
//--------------------------------------------------

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

//--------------------------------------------------
// fast Types
//--------------------------------------------------

typedef uint_fast8_t   uf8;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;

typedef  int_fast8_t    sf8;
typedef  int_fast16_t  sf16;
typedef  int_fast32_t  sf32;
typedef  int_fast64_t  sf64;

#endif

//--------------------------------------------------
// unions for several unsigned types
//--------------------------------------------------

typedef union cutype16_u{
  u8  a[2];
  u16 b;
}cutype16_t;

typedef union cutype32_u{
  u8  a[4];
  u16 b[2];
  u32 c;
}cutype32_t;

typedef union cutype64_u{
  u8  a[8];
  u16 b[4];
  u32 c[2];
  u64 d;
}cutype64_t;

//--------------------------------------------------

#endif
