#pragma once

#include "functor.hpp"
#include "monad.hpp"
using namespace fp;

#include <memory>
#include <iostream>
using namespace std;

namespace fp {
  template<class L,class R>
  class Either {
  public:
    static Either<L,R> left (L l) {
      return Either(make_shared<L>(l), nullptr);
    }
    static Either<L,R> right(R r) {
      return Either(nullptr, make_shared<R>(r));
    }
    bool isLeft () const { return (_l != nullptr); }
    bool isRight() const { return (_r != nullptr); }
    L fromLeft  () const { assert(isLeft ()); return *_l.get(); }
    R fromRight () const { assert(isRight()); return *_r.get(); }
  private:
    shared_ptr<L> _l; shared_ptr<R> _r;
    Either(const shared_ptr<L> l,const shared_ptr<R> r)
      : _l(l != nullptr ? l : nullptr)
      , _r(r != nullptr ? r : nullptr) {};
  };

  template<class L,class R>
  ostream& operator<<(ostream& out,const Either<L,R> &e) {
    if (e.isLeft())  out << "<Left:"  << e.fromLeft()  << ">";
    if (e.isRight()) out << "<Right:" << e.fromRight() << ">";
    return out;
  }

  template <typename L, typename R>
  bool operator ==(const Either<L, R> e1, const Either<L, R> e2) {
    if (e1.isLeft() && e2.isLeft()) {
      return e1.fromLeft() == e2.fromLeft();
    } else if (e1.isRight() && e2.isRight()) {
      return e1.fromRight() == e2.fromRight();
    } else {
      return false;
    }
  }

  template <typename L, typename R>
  bool operator !=(const Either<L, R> e1, const Either<L, R> e2) {
    return !(e1 == e2);
  }

  template <>
  struct Functor<Either> {
    template <typename L, typename R1, typename R2>
    static function <Either<L, R2>(Either<L, R1>)> fmap2(function <R2(R1)> f) {
      return [f](Either<L, R1> e) -> Either<L, R2> {
        if (e.isLeft()) {
          return e;
        } else {
          return Either<L, R2>::right(f(e.fromRight()));
        }
      };
    };
  };

  template <>
  struct Monad<Either> {
    template <typename L, typename R>
    static Either<L, R> return2(R v){
      return Either<L, R>::right(v);
    }

    template <typename L, typename R1, typename R2>
    static Either<L, R2> bind2(Either<L, R1> e, function<Either<L, R2>(R1)> f){
      if (e.isLeft()){
        return e;
      } else {
        return f(e.fromRight());
      }
    }
  };
}
