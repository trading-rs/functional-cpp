#include <functional>
using namespace std;

namespace fp {
  template <typename A, typename B, typename C>
  static function<C(A)> compose(function<B(A)> f1, function<C(B)> f2) {
    return [f1,f2](A v) -> C {
      return f2(f1(v));
    };
  }
}
