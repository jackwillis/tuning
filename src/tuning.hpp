#pragma once

#include <cmath>
#include <iostream>
#include <variant>
#include <vector>

#include <boost/rational.hpp>

// Refer to source of haskell package hmt
// https://hackage.haskell.org/package/hmt-0.16/docs/

const int CENTS_PER_OCTAVE = 1200;

// https://stackoverflow.com/a/11714601/3551701
static int euclidean_remainder(int a, int b)
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

  double cents() const;
  double ratio() const;
  TuningInterval add_cents(double delta) const;
  TuningInterval transpose_octaves(int octaves) const;
  std::string str() const;
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