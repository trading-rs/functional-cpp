#pragma once

#include <functional>
using namespace std;

namespace fp {
  template <template <typename...> class F>
  struct Monad {
    template <typename A>
    static F<A> return_(A);

    template <typename A, typename B>
    static F<B> bind(F<A>, function<F<B>(A)>);
  };

  template <template <typename...> class F, typename A, typename B>
  static F<B> bind(F<A> m, function<F<B>(A)> f) {
    return Monad<F>::bind(m, f);
  }

  template <template <typename...> class F, typename A>
  static F<A> return_(A a) {
    return Monad<F>::return_(a);
  }

  template <template <typename...> class F, typename A, typename B>
  static F<B> operator >=(F<A> m, function<F<B>(A)> f) {
    return Monad<F>::bind(m, f);
  }

  template <template <typename...> class F, typename A, typename B>
  static F<B> operator >>(F<A> a, F<B> b) {
    function<F<B>(A)> f = [b](A){ return b; };
    return a >= f;
  }
}
