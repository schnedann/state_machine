#include <string>
#include <array>
#include <vector>
#include "catch.hpp"

#include "dtypes.h"
#include "meta.h"

TEST_CASE("Meta"){
  SECTION("Meta::Varadic::sum"){
    auto dut = Meta::Varadic::sum<std::string,std::string,std::string>("Hello"," ","World");
    auto ref = "Hello World";
    REQUIRE( 0==dut.compare(ref) );
  }

  SECTION("Meta::Varadic::bswap"){
    {
      auto dut = Meta::Varadic::bswap<u16>(0x1234U);
      auto ref = 0x3412U;
      REQUIRE( dut == ref );
    }
    {
      auto dut = Meta::Varadic::bswap<u32>(0xABCD1234UL);
      auto ref = 0x3412CDABUL;
      REQUIRE( dut == ref );
    }
    {
      auto dut = Meta::Varadic::bswap<u64>(0x0123456789abcdefULL);
      auto ref = 0xefcdab8967452301ULL;
      REQUIRE( dut == ref );
    }
  }

  SECTION("Meta::Math::DISCRETE<>::allbitsset"){
    /*REQUIRE(Meta::Math::DISCRETE< u8>::allbitsset ==  u8(-1));
    REQUIRE(Meta::Math::DISCRETE<u16>::allbitsset == u16(-1));
    REQUIRE(Meta::Math::DISCRETE<u32>::allbitsset == u32(-1));
    REQUIRE(Meta::Math::DISCRETE<u64>::allbitsset == u64(-1));*/
  }

  SECTION("Meta::Math::DISCRETE<u16>::isPrime"){
    {
      std::array<u16,100> set = {
        2,  3,  5,  7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
        89, 97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,
        211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,
        349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,
        487,491,499,503,509,521,523,541
      };

      for(auto prime:set){
        CAPTURE( prime );
        REQUIRE( Meta::Math::Discrete<u16>::isPrime(prime) );
        if(prime>3) REQUIRE( !Meta::Math::Discrete<u16>::isPrime(prime-1) );
        if(prime>3) REQUIRE( !Meta::Math::Discrete<u16>::isPrime(prime+1) );
      }
    }
    {
      bool ip = Meta::Math::Discrete<u32>::isPrime(9999991);
      REQUIRE(ip);
    }
  }

  SECTION("Meta::Varadic::print_container"){
    std::vector<u16> set = {
      2,  3,  5,  7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
      89, 97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,
      211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,
      349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,
      487,491,499,503,509,521,523,541
    };
    Meta::Varadic::print_container<std::vector,u16>(set);
  }
}
