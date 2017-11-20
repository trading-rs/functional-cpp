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

    template <typename A, typename B>
    static F<A, B> return2(B);

    template <typename A, typename B, typename C>
    static F<A, C> bind2(F<A, B>, function<F<A, C>(B)>);
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
  static F<B> operator >>=(F<A> m, function<F<B>(A)> f) {
    return Monad<F>::bind(m, f);
  }

  template <template <typename...> class F, typename A, typename B>
  static F<B> operator >>(F<A> a, F<B> b) {
    function<F<B>(A)> f = [b](A){ return b; };
    return a >>= f;
  }

  template <template <typename...> class F, typename A, typename B, typename C>
  static F<A, C> bind2(F<A, B> m, function<F<A, C>(B)> f) {
    return Monad<F>::bind2(m, f);
  }

  template <template <typename...> class F, typename A, typename B>
  static F<A, B> return2(B b) {
    return Monad<F>::return2(b);
  }

  template <template <typename...> class F, typename A, typename B, typename C>
  static F<A, C> operator |=(F<A, B> m, function<F<A, C>(B)> f) {
    return Monad<F>::bind2(m, f);
  }

  template <template <typename...> class F, typename A, typename B, typename C>
  static F<A, C> operator |(F<A, B> a, F<A, C> c) {
    function<F<A, C>(B)> f = [c](B){ return c; };
    return a |= f;
  }
}
