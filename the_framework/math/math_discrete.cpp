#include "global_config.h"
#include "math_discrete.h"

using namespace Math::Discrete;
using namespace Math::Boolean;

//-----

/**
 * @brief log2u32
 * @param _x
 * @return
 */
u8 Math::Discrete::log2u32(u32 const _x){
  u32 lx = _x;
  const u32 b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
  const u32 S[] = {1, 2, 4, 8, 16};

  u32 res = 0; // result of log2(_x) will go here
  for (int ii = 4; ii >= 0; --ii){ // unroll for speed...
    if (lx & b[ii]){
      lx >>= S[ii];
      res |= S[ii];
    }
  }
  return u8(res);
}

//-----

/*
 * Next largest (Integer which is a) Power of 2
 */
u64 nlpo2(u64 const data, u8 const bits){
  u64 res = data;
  //Not Power of Two
  if(is_power_of_2<u64>(res)==0){
    //clog2(bits)
    for(u8 ii=1; ii<bits; ii<<=1){
      //Set all Lower Bits
      res |= (res >> ii);
    }
    res += 1;
  }
  return res;
}

/*
 * General purpouse Bitreverse
 */
u64 bitreverse(u64 const data, u8 const bits){
  u64 res = 0;
  u64 lmask = MASK_MSB<u64>(bits);
  u64 rmask = 1;

  while(lmask > rmask){
    res |= (data&lmask)?(rmask):(0);
    res |= (data&rmask)?(lmask):(0);
#if DEBUG_CODE == 1
    cout << im_prnbin(res,bits) << endl;
#endif
    lmask >>= 1;
    rmask <<= 1;
  }
  return res;
}

//-----

/**
 * Absolute Value of an Integer
 * Code is Architecture independent
 */
u16 intAbs16(s16 v){
  u16 r;                   // the result goes here
  s16 const mask = v >> 15; //Sign as Mask
  r = (v + mask) ^ mask;      //Calc Absolute
  return r;
}

u32 intAbs32(s32 v){
  u32 r;                    // the result goes here
  s32 const mask = v >> 31; //Sign as Mask
  r = (v + mask) ^ mask;      //Calc Absolute
  return r;
}

u64 intAbs64(s64 v){
  u64 r;                    // the result goes here
  s64 const mask = v >> 63; //Sign as Mask
  r = (v + mask) ^ mask;    //Calc Absolute
  return r;
}

//-----

/* Prototype Code...

s64 div_ceil(s64 numerator, s64 denominator){
  std::lldiv_t res = std::lldiv(numerator, denominator);
  return (res.rem)?(res.quot+1):(res.quot);
}

s64 div_to_zero(s64 n, s64 d){
 return n / d;
} //as per C++11 standard note 80

s64 div_up(s64 n, s64 d) {
  return (n / d) + (((n < 0) ^ (d > 0)) && (n % d));
} //i.e. +1 iff (not exact int && positive result)

s64 div_down(s64 n, s64 d) {
  return n / d - (((n > 0) ^ (d > 0)) && (n % d));
} //i.e. +1 iff (not exact int && negative result)

s64 div_to_nearest(s64 n, s64 d) {
  return (2*n - d + 2*(true&&((n<0)^(d>0)))*d) / (2*d);
} //i.e. +-0.5 as per pre-rounding result sign, then div_to-zero
//it however rounds numbers like +/- 3.5 towards 0 and not even.

*/

//----- Count Bits Set to 1

/**
 * Most Efficient Code for the
 * Hamming Weight:
 * Count the Number of Set Bits
 */
u8 hamming_weight16(u16 val){
  u16 res=val;
  res -= (res >> 2) & 0x5555u;
  res  = (res & 0x3333u) + ((res >> 2) & 0x3333u);
  res  = (res + (res >> 4)) & 0x0f0fu;
  res += res >> 8;
  return static_cast<u8>(res & 0x1fu);
}

u8 hamming_weight32(u32 val){
  u32 res=val;
  res -= (res >> 2) & 0x55555555u;
  res  = (res & 0x33333333u) + ((res >> 2) & 0x33333333u);
  res  = (res + (res >> 4)) & 0x0f0f0f0fu;
  res += res >> 24;
  return static_cast<u8>(res & 0x1fu);
}

/**
 * Signed Division with rounding Mode Ceil()
 * eg. Gnu Compiler rounds to Zero normaly
 */
s16 sdivceil(s16 a, s16 b){
  s16 div = a / b;
  if (((a ^ b) >= 0) && (a % b != 0)){
      div++;
  }
  return div;
}

/**
 * Unsigned Division with rounding Mode Ceil()
 * eg. Gnu Compiler rounds to Zero normaly
 */
u16 udivceil(u16 a, u16 b){
  u16 div = a / b;
  if (a % b != 0){
    div++;
  }
  return div;
}

//--------------------------------------------------------------------------------

/**
 * Returns the floor form of binary logarithm for a 32 bit integer.
 * -1 is returned if n is 0.
 */
s32 floorLog2_32(u32 n){
  u32 pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return (n==0)?(-1):(static_cast<s32>(pos));
}

/**
 * Returns the floor form of binary logarithm for a 32 bit integer.
 * -1 is returned if n is 0.
 */
s16 floorLog2_16(u16 n){
  u32 pos = 0;
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return (n==0)?(-1):(static_cast<s16>(pos));
}

//--------------------------------------------------------------------------------

//-----

/**
 * Inverse square Root
 * see: http://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
float Q_rsqrt(float const number ){
  auto const x2 = number * 0.5F;
  auto const tmp = 0x5F375A86f - (*(reinterpret_cast<u32 const *>(&number))>>1);
  auto y = *(reinterpret_cast<float const *>(&tmp));
  y  = y * ( 1.5F - ( x2 * y * y ) );   // 1st iteration
  y  = y * ( 1.5F - ( x2 * y * y ) );   // 2nd iteration, this can be removed
  return y;
}
double Q_rsqrt(double const number ){
  auto const x2 = number * 0.5;
  auto const tmp = 0x5FE6EB50C7B537A9 - (*(reinterpret_cast<u64 const *>(&number))>>1);
  auto y = *(reinterpret_cast<float const *>(&tmp));
  y  = y * ( 1.5 - ( x2 * y * y ) );   // 1st iteration
  y  = y * ( 1.5 - ( x2 * y * y ) );   // 2nd iteration, this can be removed
  return y;
}

//-----

//-----
