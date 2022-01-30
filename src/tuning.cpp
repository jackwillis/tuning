#include <sstream>

#include "tuning.hpp"

double TuningInterval::cents() const
{
  return std::visit(
      overloaded{
          [](double cents)
          {
            return cents;
          },
          // convert ratio to cents
          [](boost::rational<int> ratio)
          {
            const double f_ratio = boost::rational_cast<double>(ratio);
            return std::log2(f_ratio) * CENTS_PER_OCTAVE;
          }},
      this->v);
}

double TuningInterval::ratio() const
{
  return std::visit(
      overloaded{
          // convert cents to ratio
          [](double cents)
          {
            return std::pow(2, cents / CENTS_PER_OCTAVE);
          },

          [](boost::rational<int> ratio)
          {
            return boost::rational_cast<double>(ratio);
          }},
      this->v);
}

std::string TuningInterval::str() const
{
  return std::visit(
      overloaded{
          [](double cents)
          {
            std::stringstream ss;
            ss << std::defaultfloat << cents;
            return ss.str();
          },
          [](boost::rational<int> ratio)
          {
            std::stringstream ss;
            ss << ratio;
            return ss.str();
          }},
      this->v);
}

TuningInterval TuningInterval::add_cents(double delta) const
{
  return TuningInterval{this->cents() + delta};
}

TuningInterval TuningInterval::transpose_octaves(int octaves) const
{
  return this->add_cents(octaves * CENTS_PER_OCTAVE);
}
