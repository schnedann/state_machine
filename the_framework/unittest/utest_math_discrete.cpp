/****************************************
 * Includes
 ****************************************/
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

#include "catch.hpp"

#include "../core/dtypes.h"
#include "../math/math_discrete.h"

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

TEST_CASE("Math-Discrete","[math discrete]"){

  SECTION("log2 1/3"){
    {
      array<u8,50> res{};
      for(u64 ii=0; ii<res.size(); ++ii){
        res[ii] = u8(Math::Discrete::log2<u64>(ii));
      }
      u64 ii=1;
      for(u64 ij=0; ij<res.size(); ++ij){
        cout << "[" << ij << "|" << u16(res[ij]) << "]";
        if(ii==10){
          cout << "\n";
          ii=0;
        }else{
          cout << ",";
        }
        ++ii;
      }
      cout << "\n";
    }
  }

  SECTION("log2 2/3"){
    {
      //Compare log2 from stdlib with own implementation

      /* http://www.cplusplus.com/doc/tutorial/typecasting/
       * If the conversion is from a floating-point type to an integer type,
       * the value is truncated (the decimal part is removed). If the result
       * lies outside the range of representable values by the type,
       * the conversion causes undefined behavior.
       */

      for(u64 ii=1; ii>0; ii<<=1){
        CAPTURE(ii);
        auto res1 = Math::Discrete::log2<u64>(ii);
        auto res2 = u64(std::log2(ii));
        REQUIRE(res1==res2);
      }
    }
  }

  SECTION("log2 3/3"){
//TODO
    /*{
      for(u64 ii=1; ii>0; ii=ii*2+1){
        CAPTURE(ii);
        auto res1 = Math::Discrete::log2<u64>(ii);
        double res_i = std::round(std::log2(ii)); //std::round()
        CAPTURE(res_i);
        auto res2 = u64(res_i);
        REQUIRE(res1==res2);
      }
    }*/
  }

  SECTION("multiply_low_part"){
    u16 const a = 123;
    u16 const b = 456;
    u32 const c = u32(a)*u32(b);
    CAPTURE(a);
    CAPTURE(b);
    CAPTURE(c);
    REQUIRE( u16(c) == Math::Discrete::multiply_low_part<u16>(a,b) );
  }

  SECTION("multiply_high_part"){
    u16 const a = 123;
    u16 const b = 456;
    u32 const c = u32(a)*u32(b);
    CAPTURE(a);
    CAPTURE(b);
    CAPTURE(c);
    REQUIRE( u16(c>>16) == Math::Discrete::multiply_high_part<u16>(a,b) );
  }

  SECTION(""){

  }
}
