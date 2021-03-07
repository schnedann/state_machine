#ifndef META_H
#define META_H

#include <iostream>
#include <limits>
#include <utility>
#include <initializer_list>


#include "dtypes.h"
#include "compile_guards.h"


namespace Meta{

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace Constants{

template<typename T> struct UINTMAX{
  constexpr void typecheck(){
    Compile::Guards::IsUnsigned<T>();
  }
  constexpr static const T value = ((((1ull<<((sizeof(T)<<3)-1))-1)<<1)|1);
};

template<typename T> struct TYPEBITS{
  constexpr static const T value = sizeof(T)<<3;
};

} //namespace

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace Varadic {

template<typename ...Args> constexpr auto sum(Args ...args){
  return (args + ...);
}//            ^~~~~ fold expression

//---

template<typename ...Args> constexpr void printf(Args&&... args){
  (std::cout << ... << std::forward<Args>(args)) << '\n';
  return;
}

//---

// compile-time endianness swap based on http://stackoverflow.com/a/36937049
template<typename T, std::size_t... N> constexpr T bswap_impl(T _i, std::index_sequence<N...>) {
  return ((((_i >> (N * Meta::Constants::TYPEBITS<u8>::value)) & T(Meta::Constants::UINTMAX<u8>::value)) <<
           ((sizeof(T)-1-N) * Meta::Constants::TYPEBITS<u8>::value)) | ...);
}; //                                                                ^~~~~ fold expression
template<typename T, typename U = std::make_unsigned_t<T>> constexpr U bswap(T _i){
  return bswap_impl<U>(static_cast<U>(_i), std::make_index_sequence<sizeof(T)>{});
}

//---

  /**
   * sum sizes of types in varadic template
   */
  template<std::size_t N, typename Ti, typename ...T> struct varadic_size;

  template<std::size_t N, typename Ti, typename ...T> struct varadic_size{
    constexpr static std::size_t const value = sizeof(Ti)+varadic_size<N-1,T...>::value;
  };

  template<typename Ti, typename ...T> struct varadic_size<0,Ti,T...>{
    constexpr static std::size_t const value = sizeof(Ti);
  };

//---

  /**
   * Print for all std:: containers...
   * https://eli.thegreenplace.net/2014/variadic-templates-in-c/
   */
  template <template <typename, typename...> class ContainerType,
            typename ValueType, typename... Args>
  constexpr void print_container(ContainerType<ValueType, Args...> const& c) {
    for (const auto& v : c) {
      std::cout << v << ' ';
    }
    std::cout << '\n';
    return;
  }

} //namespace

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace Math {

namespace Convert {

template<typename FloatType, typename RT> constexpr RT sattFloatToInt(FloatType const num) {
  Compile::Guards::IsFloatType<FloatType>();
  Compile::Guards::IsInteger<RT>();
  RT res = static_cast<RT>(num); //Assume it is safe to cast
  //check if float fits into integer
  if ( std::numeric_limits<RT>::digits < std::numeric_limits<FloatType>::digits) {
    auto const max = std::numeric_limits<RT>::max();
    if(num > static_cast<FloatType>(max)) res = max;
    auto const min = std::numeric_limits<RT>::min();
    if(num < static_cast<FloatType>(min)) res = min;
  }
  return res;
}

} //namespace

template <typename T> struct Discrete{
  constexpr void typecheck(){
    Compile::Guards::IsUnsigned<T>();
  }
  constexpr static const T allbitsset = ((((T(1)<<((sizeof(T)<<3)-1))-1)<<1)|1);

  //---

  constexpr static bool isPrime_R(T const _v, T _d){
    bool res = true;
    while(res && (_d > 1)){
      res = (0 != (_v/_d));
      --_d;
    }
    return true;
  }

  constexpr static bool isPrime(T const _v){
    return (4>_v)?(true):(
           (0==(_v%2))?(false):(
            isPrime_R(_v,(_v-1))));
  }
};

} //namespace

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace Types {

template <typename T> struct DeConst {
  typedef T R;
};
template <typename T> struct DeConst<const T> {
  typedef T R;
};

//--- Select Type by condition

template<bool cond, typename TRUE, typename FALSE> struct if_else{
  using type = FALSE;
};

template<typename TRUE, typename FALSE> struct if_else<true,TRUE,FALSE>{
  using type = TRUE;
};

template<bool cond, typename TRUE, typename FALSE> using if_else_t = typename if_else<cond,TRUE,FALSE>::type;

//--- construct nested Initializer List

template<typename T, size_t LEVELS> struct nested_init_lists_st{
  using type = std::initializer_list<typename nested_init_lists_st<T,(LEVELS-1)>::type>;
  //using type = if_else_t<(LEVELS>1),std::initializer_list<typename nested_init_lists_st<T,(LEVELS-1)>::type>,std::initializer_list<T>>;
};

template<typename T> struct nested_init_lists_st<T,1>{
  using type = std::initializer_list<T>;
};

template<typename T> struct nested_init_lists_st<T,0>{
  using type = std::initializer_list<T>;
};

//---

} //namespace

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/*namespace datastruct{

template<typename T1, typename T2> struct pair{
  T1 first;
  T2 second;
};*/

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

};

#endif // META_H
