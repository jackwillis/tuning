#include <cmath>
#include <iostream>
#include <variant>
#include <vector>
#include <boost/rational.hpp>

struct CentsVisitor
{
  const int CENTS_PER_OCTAVE = 1200;

  double operator()(double cents) const
  {
    return cents;
  }

  double operator()(boost::rational<int> ratio) const
  {
    double f_ratio = boost::rational_cast<double>(ratio);
    return std::log2(f_ratio) * CENTS_PER_OCTAVE;
  }
};

class ScaleInterval
{
private:
  const std::variant<double, boost::rational<int>> v;

public:
  ScaleInterval(double cents) : v(cents){};
  ScaleInterval(int numer, int denom) : v(boost::rational<int>(numer, denom)){};

  double cents()
  {
    return std::visit(CentsVisitor{}, v);
  }
};

int main()
{
  std::vector<ScaleInterval> bremmer_ebvt3 = {
      ScaleInterval{94.87252},
      ScaleInterval{197.05899},
      ScaleInterval{297.80000},
      ScaleInterval{395.79561},
      ScaleInterval{4, 3},
      ScaleInterval{595.89736},
      ScaleInterval{699.31190},
      ScaleInterval{796.82704},
      ScaleInterval{896.20299},
      ScaleInterval{999.10000},
      ScaleInterval{1096.17389},
      ScaleInterval{2, 1}};

  for (auto itv : bremmer_ebvt3)
  {
    std::cout << itv.cents() << '\n';
  }
  return EXIT_SUCCESS;
}
