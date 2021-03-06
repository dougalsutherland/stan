#include <stan/agrad/rev/Phi_approx.hpp>
#include <test/agrad/util.hpp>
#include <gtest/gtest.h>
#include <stan/agrad/agrad.hpp>
#include <stan/math/functions/Phi_approx.hpp>

TEST(AgradRev, Phi_approx) {
  using stan::agrad::var;
  using std::abs;
  using stan::math::Phi_approx;

  std::vector<double> y_values;
  y_values.push_back(0.0);
  y_values.push_back(0.9);
  y_values.push_back(-5.0);

  for (size_t i = 0; i < y_values.size(); i++) {
    var y, phi_y, phi_approx_y;
    y = y_values[i];
    phi_y = stan::agrad::Phi(y);
    phi_approx_y = Phi_approx(y);
    EXPECT_NEAR(phi_y.val(), phi_approx_y.val(), 0.00014);

    std::vector<double> g;
    std::vector<var> as;
    var a = y_values[i];
    as.push_back(a);
    var f = Phi_approx(a);
    f.grad(as,g);

    std::vector<double> g2;
    std::vector<var> as2;
    var a2 = y_values[i];
    as2.push_back(a2);
    var f2 = inv_logit(0.07056 * pow(a2,3.0) + 1.5976 * a2);
    f2.grad(as2,g2);
    
    EXPECT_EQ(1U,g.size());
    EXPECT_EQ(1U,g2.size());

    EXPECT_FLOAT_EQ(g2[0], g[0]);
  }
}
