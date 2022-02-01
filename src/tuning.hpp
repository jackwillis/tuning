// tuning.hpp:
// Tuning class represents the data in a Scala scale file.

// Refer to source of haskell package hmt which parses Scala files
// https://hackage.haskell.org/package/hmt-0.16/docs/

#pragma once

#include <cmath>
#include <iostream>
#include <variant>
#include <vector>

#include <boost/rational.hpp>

// For Rust-style matching on variant types lol
// https://dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
template <class... Ts>
struct match : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
match(Ts...) -> match<Ts...>;

////////

const int CENTS_PER_OCTAVE = 1200;

// Tuning intervals can be written as either a
// difference in cents (type double), meaning percents of a semitone, or as a
// frequency ratio (type rational<int>).

typedef double Cents;
typedef boost::rational<int> Ratio;

class TuningInterval
{
private:
  const std::variant<Cents, Ratio> v;

public:
  TuningInterval(Cents cents) : v(cents){};
  TuningInterval(int numer, int denom) : v(Ratio{numer, denom}){};

  Cents cents() const;
  Ratio ratio() const;
  double f_ratio() const;

  TuningInterval add_cents(Cents delta) const;
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
         std::vector<TuningInterval> intervals)
      : name(name), description(description), intervals(intervals){};

  int degree() const;
  TuningInterval at(int n) const;

  void stream_scala(std::ostream &out);
  void stream_table(std::ostream &out);
};
