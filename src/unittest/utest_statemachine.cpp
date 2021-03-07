//#include <iostream>
#include <array>

#include "catch.hpp"
#include "dtypes.h"

#include "statemachine.h"

using namespace std;

TEST_CASE("statemachine"){
  SECTION("1"){
    array<u8,11> const refsumlist = {
      0, 1, 2,
      6,15,16,
      16,17,21,
      30,30
    };

    constexpr static u8 const states      = 4;
    constexpr static u8 const transitions = 7;

    u16 svisited=0;
    u16 tvisited=0;

    auto pow = [](u8 a)->u64{
      u64 res = a;
      return res*a;
    };

    STATE_MACHINE::adjacency_matrix<states,transitions> sm(
      {//Transitions From State to State --> the State-Graph
        {STATE_MACHINE::noTr,
         1                  ,
         STATE_MACHINE::noTr,
         0                  },
        {2                  ,
         3                  ,
         4                  ,
         STATE_MACHINE::noTr},
        {STATE_MACHINE::noTr,
         STATE_MACHINE::noTr,
         STATE_MACHINE::noTr,
         5                  },
        {7                  ,
         6                  ,
         STATE_MACHINE::noTr,
         0                  }
      },{//State Callbacks#
        //State 0
        [&svisited,&sm,&pow](){
          svisited += pow(sm.get_current_state());
          //cout << "[" << u16(sm.get_current_state()) << "]: " << svisited << "\n";
        },
        //State 1
        [&svisited,&sm,&pow](){
           svisited += pow(sm.get_current_state());
           //cout << "[" << u16(sm.get_current_state()) << "]: " << svisited << "\n";
        },
        //State 2
        [&svisited,&sm,&pow](){
          svisited += pow(sm.get_current_state());
          //cout << "[" << u16(sm.get_current_state()) << "]: " << svisited << "\n";
        },
        //State 3
        [&svisited,&sm,&pow](){
          svisited += pow(sm.get_current_state());
          //cout << "[" << u16(sm.get_current_state()) << "]: " << svisited << "\n";
        }
      },{//Transition Callbacks
        //0: 0-->1
        [&tvisited]()->bool{
           bool OK = false;
           if(0==tvisited || 6==tvisited) OK = true;
           ++tvisited;
           return OK;
        },
        //1: 1-->0
        [&tvisited]()->bool{
          bool OK = false;
          if(5==tvisited) OK = true;
          if(OK) ++tvisited;
          return OK;
        },
        //2: 1-->1
        [&tvisited]()->bool{
           bool OK = false;
           if(1==tvisited) OK = true;
           if(OK) ++tvisited;
           return OK;
        },
        //3: 1-->2
        [&tvisited]()->bool{
           bool OK = false;
           if(2==tvisited || 7==tvisited) OK = true;
           if(OK) ++tvisited;
           return OK;
        },
        //4: 2-->3
        [&tvisited]()->bool{
          ++tvisited;
          return true;
        },
        //5: 3-->1
        [&tvisited]()->bool{
           bool OK = false;
           if(5>tvisited) OK = true;
           if(OK) ++tvisited;
           return OK;
        },
        //6: 3-->0
        [&tvisited]()->bool{
           bool OK = false;
           if(9==tvisited) OK = true;
           if(OK) ++tvisited;
           return OK;
        }
      });

    u16 cycle = 0;
    while((tvisited<10) && (svisited!=30)){
      CAPTURE( cycle );
      REQUIRE( cycle == tvisited );
      sm.step();
      REQUIRE( refsumlist[cycle] == svisited );
      ++cycle;
    }

    /*#define pvar(_x) {cout << #_x << ": " << (_x) << "\n";}
    pvar(svisited)
    pvar(tvisited)*/
  }
}
