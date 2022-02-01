// math.hpp
// Misc math functions

#pragma once

#include <cmath>
#include <iostream>

#include <boost/rational.hpp>

// http://www.johndcook.com/blog/2010/10/20/best-rational-approximation/
boost::rational<int> farey(double x, int max_denom)
{
  int a = 0;
  int b = 1;
  int c = 1;
  int d = 1;
  double mediant;

  while (b <= max_denom && d <= max_denom)
  {
    mediant = (double)(a + c) / (double)(b + d);
    if (x == mediant)
    {
      if (b + d <= max_denom)
      {
        return boost::rational<int>{a + c, b + d};
      }
      else if (d > b)
      {
        return boost::rational<int>{c, d};
      }
      else
      {
        return boost::rational<int>{a, b};
      }
    } // end if (x == mediant)
    else if (x > mediant)
    {
      a += c;
      b += d;
    }
    else
    {
      c += a;
      d += b;
    }
  } // end while

  if (b > max_denom)
  {
    return boost::rational<int>{c, d};
  }
  else
  {
    return boost::rational<int>{a, b};
  }
}

boost::rational<int> closest_rational(double x)
{
  const int integral_part = (int)std::trunc(x);
  const double fractional_part = x - integral_part;

  const int max_denom = 200;
  const auto rational_fractional_part = farey(fractional_part, max_denom);
  return integral_part + rational_fractional_part;
}

// https://stackoverflow.com/a/11714601/3551701
int euclidean_remainder(int a, int b)
{
  assert(b != 0);
  int r = a % b;
  return r >= 0 ? r : r + std::abs(b);
}
