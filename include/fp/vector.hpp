#pragma once
#include "functor.hpp"
#include "monad.hpp"
using namespace fp;

#include <vector>
using namespace std;

namespace fp {
  template <>
  struct Functor<vector> {
    template <typename A, typename B>
    static function <vector<B>(vector<A>)> fmap(function <B(A)> f) {
      return [f](vector<A> v){
        vector<B> result;
        transform(v.begin(), v.end(), back_inserter(result), f);
        return result;
      };
    }
  };

  template <>
  struct Monad<vector> {
    template <typename A>
    static vector<A> return_(A v){
      return vector<A>{v};
    }

    template <typename A, typename B>
    static vector<B> bind(vector<A> m, function<vector<B>(A)> f){
      vector<B> v;
      for_each(m.begin(), m.end(), [f, &v](A a){
          vector<B> result = f(a);
          copy(result.begin(), result.end(), back_inserter(v));
        });
      return v;
    }
  };
}
