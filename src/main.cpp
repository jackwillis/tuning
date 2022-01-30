#include <cmath>
#include <functional>
#include <iostream>
#include <variant>
#include <vector>

#include <boost/rational.hpp>

// Refer to source of haskell package hmt
// https://hackage.haskell.org/package/hmt-0.16/docs/

const int CENTS_PER_OCTAVE = 1200;

// https://stackoverflow.com/a/11714601/3551701
int euclidean_remainder(int a, int b)
{
  assert(b != 0);
  int r = a % b;
  return r >= 0 ? r : r + std::abs(b);
}

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

  TuningInterval add_cents(double delta) const
  {
    return TuningInterval{this->cents() + delta};
  }

  TuningInterval transpose_octaves(int octaves) const
  {
    return this->add_cents(octaves * CENTS_PER_OCTAVE);
  }

  std::string str() const
  {
    return std::visit(TuningIntervalStrVisitor{}, this->v);
  }
};

class Tuning
{
private:
  const std::string name;
  const std::string description;
  const std::vector<TuningInterval> intervals;

public:
  Tuning(std::string name,
         std::string description,
         int degree,
         std::vector<TuningInterval> intervals)
      : name(name), description(description), intervals(intervals)
  {
    if (degree != this->degree())
    {
      throw std::invalid_argument("Could not build scale: Wrong scale degree specified.");
    }
  };

  int degree() const
  {
    return static_cast<int>(intervals.size());
  }

  TuningInterval at(int n) const
  {
    const int index = euclidean_remainder(n - 1, this->degree());
    const auto interval = intervals.at((std::size_t)index);

    const int octave = ((n - 1) / this->degree()) - 1;

    return interval.transpose_octaves(octave);
  }

  void stream_scala(std::ostream &out) const
  {
    out << "! "
        << this->name << "\n! \n"
        << this->description << '\n'
        << this->degree() << "\n!\n";

    for (const auto &interval : this->intervals)
    {
      out << interval.str() << '\n';
    }
  }

  void stream_table(std::ostream &out) const
  {
    const auto small_pad = std::setw(6);
    const auto big_pad = std::setw(15);

    out << small_pad << "Index"
        << big_pad << "Cents"
        << big_pad << "Ratio"
        << '\n';

    for (int i = this->degree() * -1; i != this->degree() * 3; ++i)
    {
      const auto interval = this->at(i);
      out << small_pad << i
          << big_pad << interval.cents()
          << big_pad << interval.ratio()
          << '\n';
    }
  }
};

int main()
{
  const Tuning bremmer_ebvt3 = Tuning{
      "bremmer_ebvt3.scl",
      "Bill Bremmer EBVT III temperament (2011)",
      12,
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

  bremmer_ebvt3.stream_scala(std::cout);
  std::cout << "---------------------------------------------\n";
  bremmer_ebvt3.stream_table(std::cout);

  return EXIT_SUCCESS;
}
