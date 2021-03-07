/****************************************
 * Includes
 ****************************************/
#include <iostream>
#include <iomanip>
#include <random>

#include "catch.hpp"

#include "dtypes.h"
#include "bitmacros.h"
#include "math_discrete.h"
#include "simple_hash_fct.h"

/****************************************
 * Defines
 ****************************************/



/****************************************
 * Namespaces
 ****************************************/

using namespace std;

/****************************************
 * local Prototypes
 ****************************************/



/****************************************
 * Global Constants
 ****************************************/



/****************************************
 * Typedefs
 ****************************************/



//--- Code

TEST_CASE("bitmacros","[boolean]"){
  SECTION("1.1 - IS_EVEN not IS_ODD"){
    auto ref = u64(GENERATE(take(1000, filter([](u64 _x) { return _x % 2 == 0; }, random(0, Math::Boolean::__MAX<s32>())))));
    CAPTURE( ref );
    REQUIRE(  Math::Boolean::IS_EVEN<u64>(ref) );
    REQUIRE( !Math::Boolean::IS_ODD<u64>(ref) );
  }
  SECTION("1.2 - IS_ODD not IS_EVEN"){
    auto ref = u64(GENERATE(take(1000, filter([](u64 _x) { return _x % 2 == 1; }, random(0, Math::Boolean::__MAX<s32>())))));
    CAPTURE( ref );
    REQUIRE( !Math::Boolean::IS_EVEN<u64>(ref) );
    REQUIRE(  Math::Boolean::IS_ODD<u64>(ref) );
  }

  SECTION("2 - IS_ONEHOT"){
    for(u8 ii=0; ii<32; ++ii){
      auto oh = Math::Boolean::GETMASKBIT<u32>(ii);
      CAPTURE( ii );
      CAPTURE( oh );
      REQUIRE(  Math::Boolean::IS_ONEHOT<u32>(oh)   );
      if(oh>1) REQUIRE( !Math::Boolean::IS_ONEHOT<u32>(oh+1) );
      if(oh>3) REQUIRE( !Math::Boolean::IS_ONEHOT<u32>(oh-1) );
    }
  }

  SECTION("3 - GET_LOWESTBIT"){
    auto ii = u32(GENERATE(take(1000, filter([](u64 _x) { return _x % 2 == 0; }, random(0, Math::Boolean::__MAX<s32>())))));
    u32 reference;
    for(u8 ij=0; ij<32; ++ij){
      auto bit = Math::Boolean::GETMASKBIT<u32>(ij); //Bit at position ij
      reference = Math::Boolean::AND<u32>(ii,bit);   //>0 if bit exists in ii
      if(reference>0) break;
    }
    auto tmp = Math::Boolean::GET_LOWESTBIT<u32>(ii);
    CAPTURE( ii );
    REQUIRE( reference == tmp );
  }

  SECTION("4 - GETMASKBIT"){
    u32 reference = 1;
    for(u8 ii=0; ii<32; ++ii){
      CAPTURE( ii );
      REQUIRE( reference == Math::Boolean::GETMASKBIT<u32>(ii) );
      reference <<=1;
    }
  }

  SECTION("5a - GETFULLMASK / GETFULLMASK_v2"){
    REQUIRE( 0 == Math::Boolean::GETFULLMASK<u32>(0) );
    u32 reference = 1;
    for(u8 ii=1; ii<33; ++ii){
      CAPTURE( ii );
      REQUIRE( reference == Math::Boolean::GETFULLMASK<u32>(ii) );
      REQUIRE( reference == Math::Boolean::GETFULLMASK_v2<u32>(ii) );
      reference = (reference<<1)|1;
    }
  }

  SECTION("5b - GETFULLMASK 2/3"){
    REQUIRE( 0 == Math::Boolean::GETFULLMASK<u32>(0) );
    u32 reference = 1;
    for(u8 ii=1; ii<33; ++ii){
      CAPTURE( ii );
      REQUIRE( reference == Math::Boolean::GETFULLMASK<u32>(ii) );

      reference = (reference<<1)|1;
    }
  }

  SECTION("5c - GETFULLMASK 3/3"){
    u32 reference = 1;
    for(u8 ii=1; ii<33; ++ii){
      CAPTURE( ii );
      REQUIRE( reference == Math::Boolean::GETFULLMASK<u32>(ii) );
      //REQUIRE( reference == Math::Boolean::OFS_MAX<u32>(ii) );
      reference = (reference<<1)|1;
    }
  }

  SECTION("6a - REPLFIELD / GETVALUE"){
    u64 r1 = 0x0123456789ABCDEFull;
    u64 r2 = 0x01234AFFEDEADDEFull;   //in from start(@LSB!) length
    auto tmp = Math::Boolean::REPLFIELD<u64>(r2,r1,12,32);
    REQUIRE( r1 == tmp );
    REQUIRE( r1 != r2 );
    REQUIRE( 0xAFFEDEADull == Math::Boolean::GETVALUE<u64>(r2,12,32) );
    REQUIRE( Math::Boolean::GETVALUE<u64>(r1,44,20) == Math::Boolean::GETVALUE<u64>(r2,44,20) );
  }

  SECTION("6b - SETVALUE / GETVALUE"){
    u64 r1 = 0x0123456789ABCDEFull;
    u64 r2 = 0x01234AFFEDEADDEFull;
    auto tmp = Math::Boolean::SETVALUE<u64>(r2,Math::Boolean::GETVALUE<u64>(r1,12,32),12,32);
    REQUIRE( r1 == tmp );
    REQUIRE( r1 == r2 );
    REQUIRE( 0xAFFEDEADull != Math::Boolean::GETVALUE<u64>(r2,12,32) );
    REQUIRE( Math::Boolean::GETVALUE<u64>(r1,44,20) == Math::Boolean::GETVALUE<u64>(r2,44,20) );
    REQUIRE( Math::Boolean::GETVALUE<u64>(r1,12,32) == Math::Boolean::GETVALUE<u64>(r2,12,32) );
  }

  SECTION("7 - ZQ / INTABS - 1/2"){
    //u64 uref = 1;
    s64 sref = 1;
    for(s64 ii=-1; ii>-1000000; --ii){
      REQUIRE( sref++ == s64(Math::Boolean::ZQ<u64>(u64(ii))) );
      //REQUIRE( uref++ == Math::Boolean::INTABS<s64>(ii) );
    }
  }

  SECTION("7 - ZQ / INTABS - 2/2"){
    s8 dut = -128;
    u8 ref =  128;
    auto res = Math::Boolean::INTABS<s8>(dut);
    REQUIRE( ref == res );
  }

  SECTION("8 - multiply_low_part / multiply_high_part"){
    for(u16 ii=0; ii<256; ++ii){
      for(u16 ij=0; ij<256; ++ij){
        u16 ref = ii*ij;
        REQUIRE( u8(ref)    == Math::Discrete::multiply_low_part<u8>(u8(ii),u8(ij)) );
        REQUIRE( u8(ref>>8) == Math::Discrete::multiply_high_part<u8>(u8(ii),u8(ij)) );
      }
    }
  }

  SECTION("9 - midsquare_64"){
    u8 collect = 0;
    for(u16 ii=0; ii<256; ++ii){
      collect |= u8(Math::Hashing::midsquare_64<u16>(ii,8));
    }
    REQUIRE( Math::Boolean::GETFULLMASK<u8>(8) == collect );
  }

  SECTION("10 - GETMAXBITS"){
    REQUIRE( 7==Math::Boolean::GETMAXBITS<u8>(7) );
    REQUIRE( 8==Math::Boolean::GETMAXBITS<u8>(8) );
    REQUIRE( 8==Math::Boolean::GETMAXBITS<u8>(9) );

    REQUIRE( 15==Math::Boolean::GETMAXBITS<u16>(15) );
    REQUIRE( 16==Math::Boolean::GETMAXBITS<u16>(16) );
    REQUIRE( 16==Math::Boolean::GETMAXBITS<u16>(17) );
  }

  SECTION("11 - BITSPACE"){
    u64 ref = 2;
    for(u8 ii=1; ii<32; ++ii){
      REQUIRE( ref==Math::Boolean::BITSPACE<u64>(ii) );
      ref <<= 1;
    }
  }

  SECTION("12 - MASK_MSB"){
    u64 ref = 1;
    for(u8 ii=0; ii<32; ++ii){
      REQUIRE( ref==Math::Boolean::BITSPACE<u64>(ii) );
      ref <<= 1;
    }
  }
}
