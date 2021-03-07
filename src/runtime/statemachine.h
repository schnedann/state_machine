/******************************************************************************/
/** Compact Datatype Definitions                                             **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2021                                               **/
/******************************************************************************/

/*
 * Copyright (c) 2020-2021, Danny Schneider
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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <array>
#include <memory>
#include <functional>
#include <algorithm>

#include "dtypes.h"
#include "meta.h"
#include "compile_guards.h"
#include "bitmacros.h"

/*
 Transition 0 = No Transition
*/

namespace STATE_MACHINE {

  constexpr static u8 const noTr = 0; //Mark for "No Tranaction"

  /**
   * Base Class for State Machine Graph Management
   */
  template<u8 STATES, u8 TRANSITIONS> class adjacency_matrix_base{
  public:
    std::array<std::function<void()>,STATES>      state_lst{};
    std::array<std::function<bool()>,TRANSITIONS> transition_lst{};

    /**
     * @brief adjacency_matrix_base - Constructor
     * @param _st - Array of State Callbacks
     * @param _tr - Array of Transition Callbacks
     */
    adjacency_matrix_base(std::array<std::function<void()>,STATES>      const& _st,
                          std::array<std::function<bool()>,TRANSITIONS> const& _tr):state_lst(_st),transition_lst(_tr){}

    //adjacency_matrix_base()=default;
    //virtual u8 get_transition(u8 const _from, u8 const _to) const =0;
    virtual u8 get_current_state() const=0;
    virtual void step()=0;
  };

  /**
   *
   */
  template<u8 STATES, u8 TRANSITIONS> class adjacency_matrix:
    public adjacency_matrix_base<STATES,TRANSITIONS>{
  public:
    using amb_t = adjacency_matrix_base<STATES,TRANSITIONS>;
    using matrix_t = std::array<std::array<u8,STATES>,STATES>;

  private:

    matrix_t matrix{}; // adjacency State Matrix

    /**
     * @brief init_matrix
     * @param il2d
     */
    void init_matrix(Meta::Types::nested_init_lists_st<u8,2>::type const& il2d){
      size_t idx_2d=0;
      for(auto const& il1d:il2d){
        std::copy(il1d.begin(),il1d.end(),matrix[idx_2d].begin());
        ++idx_2d;
      }
      return;
    }

    u8 state{0}; //current State

    //---

    /**
     * @brief get_transition - Query transaction from adjacency State Matrix
     * @param _from
     * @param _to
     * @return
     */
    [[nodiscard]] u8 get_transition(u8 const _from, u8 const _to) const noexcept{
      return matrix[_from][_to];
    }

  public:
    //Constructors
    adjacency_matrix()=default;
    adjacency_matrix(adjacency_matrix const&)=delete;
    adjacency_matrix(adjacency_matrix &&)=delete;
    adjacency_matrix(Meta::Types::nested_init_lists_st<u8,2>::type const& il,
                     std::array<std::function<void()>,STATES> const& _st,
                     std::array<std::function<bool()>,TRANSITIONS> const& _tr):adjacency_matrix_base<STATES,TRANSITIONS>(_st,_tr){
      init_matrix(il);
      return;
    }
    //Destructor
    ~adjacency_matrix()=default;

    /**
     *
     */
    [[nodiscard]] virtual u8 get_current_state() const noexcept override{
      return state;
    }

    /**
     * Step the State Machine for 1 Cycle
     */
    virtual void step()noexcept override{

      //--- Phase 1: Execute current step (so it is executed at least once...)

      //in bounds?
      if(amb_t::state_lst.size() > state){
        //valid?
        if(amb_t::state_lst[state]){
          //execute Callback for State
          amb_t::state_lst[state]();
        }
      }

      //--- Phase 2: Estimate which is the next step

      u8 const from = state;
      bool go_to = false;
      //Check all possible Transitions to Next States
      u8 to=0;
      for( ;to<STATES; ++to){
        u8 tr = get_transition(from,to);

        //if not the NO Transition Mark is found
        if(tr>0){
          --tr; //C-Array adress correction
          //in bounds?
          if(amb_t::transition_lst.size() > tr){
            //valid?
            if(amb_t::transition_lst[tr]){
              //execute Callback for Transition
              go_to = amb_t::transition_lst[tr]();
              if(go_to){
                break;
              }
            }
          }
        }
      }
      //If a successful Transition was found, Goto this State
      if(go_to){
        state = to;
      }else{
        ;
      }

      return;
    }
  };

//-----

/**
 * Apply 'Or' to all values of an List
 */
template<typename T,size_t S> constexpr T or_list(std::array<T,S> const& lst){
  Compile::Guards::IsUnsigned<T>();
  T res{0};
  for(auto _x:lst){
    res |= _x;
  }
  return res;
}

/**
 * Generate a Mask with one Bit set, with num is the position of the set bit = 0..(N-1)
 */
template<typename T> constexpr T num2mask(u8 num){
  Compile::Guards::IsUnsigned<T>();
  return T(1)<<num;
}

/**
 * Query which Transaction should be executed
 */
template<typename T> constexpr T get_priority_transition(T const tv){
  Compile::Guards::IsUnsigned<T>();
  //Return lowest bit set in transition vector
  return tv&(~(tv-1));
}

 /**
  * Space minimal State machine
  */
 template<u8 STATES, u8 TRANSITIONS, typename T> class adjacency_matrix_spaceopt:
    public adjacency_matrix_base<STATES,TRANSITIONS>{
  private:

  virtual T get_transition(u8 const _from, u8 const _to) const noexcept{

  }
  public:

    adjacency_matrix_spaceopt()=default;
    adjacency_matrix_spaceopt(adjacency_matrix_spaceopt const&)=delete;
    adjacency_matrix_spaceopt(adjacency_matrix_spaceopt &&)=delete;


  };
}




#endif // STATE_MACHINE_H
