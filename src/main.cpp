#include <cmath>
#include <iostream>
#include <variant>
#include <vector>

#include <boost/rational.hpp>

// Refer to source of haskell package hmt
// https://hackage.haskell.org/package/hmt-0.16/docs/

const int CENTS_PER_OCTAVE = 1200;

// TuningInterval#cents(double)
// TuningInterval#cents(rational)
struct TuningIntervalCentsVisitor
{
  double operator()(double cents) const
  {
    return cents;
  }

  // convert ratio to cents
  double operator()(boost::rational<int> ratio) const
  {
    const double f_ratio = boost::rational_cast<double>(ratio);
    return std::log2(f_ratio) * CENTS_PER_OCTAVE;
  }
};

// TuningInterval#ratio(double)
// TuningInterval#ratio(rational)
struct TuningIntervalRatioVisitor
{
  // convert cents to ratio
  double operator()(double cents) const
  {
    return std::pow(2, cents / CENTS_PER_OCTAVE);
  }

  double operator()(boost::rational<int> ratio) const
  {
    return boost::rational_cast<double>(ratio);
  }
};

// TuningInterval#str(double)
// TuningInterval#str(rational)
struct TuningIntervalStrVisitor
{
  std::string operator()(double cents) const
  {
    return std::to_string(cents);
  }

  std::string operator()(boost::rational<int> ratio) const
  {
    std::stringstream rational_ss;
    rational_ss << ratio;
    return rational_ss.str();
  }
};

// Tuning intervals can be written as either
// cents (type double), meaning percent of a semitone, or as a
// ratio (type rational).
// This class is a wrapper for a std::variant of the two,
// which depends on some visitor structs,
// particularly for converting between cents and ratios.
//
class TuningInterval
{
private:
  const std::variant<double, boost::rational<int>> v;

public:
  TuningInterval(double cents) : v(cents){};
  TuningInterval(int numer, int denom) : v(boost::rational<int>(numer, denom)){};

  double cents() const
  {
    return std::visit(TuningIntervalCentsVisitor{}, this->v);
  }

  double ratio() const
  {
    return std::visit(TuningIntervalRatioVisitor{}, this->v);
  }

  std::string str() const
  {
    return std::visit(TuningIntervalStrVisitor{}, this->v);
  }
};

const int column_width = 15; // max length of double
auto pad = std::setw(column_width);

class Tuning
{
private:
  const std::string name;
  const std::string description;
  const std::vector<TuningInterval> intervals;

public:
  Tuning(std::string name,
         std::string description,
         std::vector<TuningInterval> intervals)
      : name(name), description(description), intervals(intervals){};

  int degree() const
  {
    return (int)intervals.size();
  }

  void print_scala() const
  {
    const int column_width = 15; // max length of double
    auto pad = std::setw(column_width);

    std::cout << "! "
              << this->name << "\n! \n"
              << this->description << '\n'
              << this->degree() << "\n!\n";

    for (const auto itv : this->intervals)
    {
      std::cout << itv.str() << '\n';
    }
  }

  void print_table() const
  {
    const int column_width = 15; // max length of double
    auto pad = std::setw(column_width);

    std::cout << "Value" << pad
              << "Ratio" << pad
              << '\n';

    for (const auto itv : this->intervals)
    {
      std::cout
          << pad << itv.str()
          << pad << itv.ratio()
          << '\n';
    }
  }
};

int main()
{
  const Tuning bremmer_ebvt3 = Tuning{
      "bremmer_ebvt3.scl",
      "Bill Bremmer EBVT III temperament (2011)",
      std::vector<TuningInterval>{
          TuningInterval{94.87252},
          TuningInterval{197.05899},
          TuningInterval{297.80000},
          TuningInterval{395.79561},
          TuningInterval{4, 3},
          TuningInterval{595.89736},
          TuningInterval{699.31190},
          TuningInterval{796.82704},
          TuningInterval{896.20299},
          TuningInterval{999.10000},
          TuningInterval{1096.17389},
          TuningInterval{2, 1}}};

  bremmer_ebvt3.print_scala();
  std::cout << "---------------------------------------------\n";
  bremmer_ebvt3.print_table();

  return EXIT_SUCCESS;
}
