#include "functor.hpp"
#include "monad.hpp"
using namespace fp;

#include <iostream>
using namespace std;

namespace fp {
  template <typename T>
  class Maybe {
  public:
    Maybe() : _empty(true){};
    explicit Maybe(T value) : _empty(false), _value(value){};

    T fromJust() const {
      if (isJust()) {
        return _value;
      } else {
        throw "Cannot get value from Nothing";
      }
    }

    T fromMaybe(T default_val) const {
      if (isJust()) {
        return _value;
      } else {
        return default_val;
      }
    }

    bool isJust() const { return !_empty; }
    bool isNothing() const { return _empty; }

    static bool isJust(Maybe &m) { return m.isJust(); }
    static bool isNothing(Maybe &m) { return m.isNothing(); }
  private:
    bool _empty;
    T _value;
  };

  template <typename T>
  Maybe<T> Nothing = Maybe<T>();

  template <typename T>
  ostream& operator<<(ostream& s, const Maybe<T> m)
  {
    if (m.isJust()) {
      return s << "Just " << m.fromJust();
    } else {
      return s << "Nothing";
    }
  }

  template <typename T>
  bool operator ==(const Maybe<T> m1, const Maybe<T> m2) {
    if (m1.isNothing() && m2.isNothing()) {
      return true;
    } else if (m1.isJust() && m2.isJust()) {
      return m1.fromJust() == m2.fromJust();
    } else {
      return false;
    }
  }

  template <typename A, typename B>
  static B maybe(B b, function<B(A)> f, Maybe<A> v) {
    if (v.isJust()) {
      return f(v.fromJust());
    } else {
      return b;
    }
  }

  template <>
  struct Functor<Maybe> {
    template <typename A, typename B>
    static function <Maybe<B>(Maybe<A>)> fmap(function <B(A)> f) {
      return [f](Maybe<A> m) -> Maybe<B> {
        if (m.isNothing()) {
          return Maybe<B>();
        } else {
          return Maybe<B>(f(m.fromJust()));
        }
      };
    };
  };

  template <>
  struct Monad<Maybe> {
    template <typename A>
    static Maybe<A> return_(A v){
      return Maybe<A>(v);
    }

    template <typename A, typename B>
    static Maybe<B> bind(Maybe<A> m, function<Maybe<B>(A)> f){
      if (m.isNothing()){
        return Maybe<B>();
      } else {
        return f(m.fromJust());
      }
    }
  };
}
