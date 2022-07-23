#include "gtest/gtest.h"

#include "polynomial.hpp"

TEST(DummyTest, Success)
{
  EXPECT_EQ(0, 0);
}

TEST(Polynomial, CtorAndGet)
{
  uint64_t alpha = 1;
  uint64_t modulo = 100;

  Polynomial poly(alpha, modulo);

  EXPECT_EQ(poly.get_alpha(), alpha);
  EXPECT_EQ(poly.get_modulo(), modulo);
}
