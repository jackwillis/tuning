#include <cmath>
#include <iostream>
#include <vector>
#include <boost/rational.hpp>
using std::ostream;
using std::vector;

class ScaleInterval
{
private:
  double m_cents;

public:
  double cents() { return m_cents; };
};

class CentsInterval : public ScaleInterval
{
private:
  double m_cents;

public:
  CentsInterval(double cents) : m_cents(cents){};
};

class RationalInterval : public ScaleInterval
{
private:
  boost::rational<int> m_rational;
  double m_cents;

public:
  const int CENTS_PER_OCTAVE = 1200;

  RationalInterval(int numer, int denom)
  {
    // std::cout << numer << '\n';
    m_rational = boost::rational<int>(numer, denom);
    // std::cout << m_rational << '\n';

    // calculate m_cents
    double f_rational = boost::rational_cast<double>(m_rational);
    m_cents = CENTS_PER_OCTAVE * std::log2(f_rational);
    // std::cout << m_cents << '\n';
  };
};

////////////////////////////

int main()
{
  const std::vector<ScaleInterval> bremmer_ebvt3 =
      {CentsInterval{94.87252},
       CentsInterval{197.05899},
       CentsInterval{297.80000},
       CentsInterval{395.79561},
       RationalInterval{4, 3},
       CentsInterval{595.89736},
       CentsInterval{699.31190},
       CentsInterval{796.82704},
       CentsInterval{896.20299},
       CentsInterval{999.10000},
       CentsInterval{1096.17389},
       RationalInterval{2, 1}};

  for (auto interval : bremmer_ebvt3)
  {
    std::cout << interval.cents() << '\n';
  }

  std::cout << CentsInterval{395.79561}.cents() << '\n';
  std::cout << RationalInterval{4, 3}.cents() << '\n';

  return EXIT_SUCCESS;
}
