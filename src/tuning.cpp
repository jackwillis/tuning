#include <sstream>

#include "math.hpp"
#include "tuning.hpp"

// TuningInterval class

double cents_to_f_ratio(Cents cents)
{
  return std::pow(2, cents / CENTS_PER_OCTAVE);
}

Cents ratio_to_cents(Ratio ratio)
{
  const double f_ratio = boost::rational_cast<double>(ratio);
  return std::log2(f_ratio) * CENTS_PER_OCTAVE;
}

Cents TuningInterval::cents() const
{
  return std::visit(
      match{[](Cents cents)
            {
              return cents;
            },
            [](Ratio ratio)
            {
              return ratio_to_cents(ratio);
            }},
      this->v);
}

Ratio TuningInterval::ratio() const
{
  return std::visit(
      match{[](Cents cents)
            {
              return closest_rational(cents_to_f_ratio(cents));
            },
            [](Ratio ratio)
            {
              return ratio;
            }},
      this->v);
}

double TuningInterval::f_ratio() const
{
  return std::visit(
      match{[](Cents cents)
            {
              return cents_to_f_ratio(cents);
            },
            [](Ratio ratio)
            {
              return boost::rational_cast<double>(ratio);
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

std::string TuningInterval::str() const
{
  return std::visit(
      match{[](Cents cents)
            {
              std::stringstream ss;
              ss << std::defaultfloat << cents;
              return ss.str();
            },
            [](Ratio ratio)
            {
              std::stringstream ss;
              ss << ratio;
              return ss.str();
            }},
      this->v);
}

// Tuning class

int Tuning::degree() const
{
  return static_cast<int>(intervals.size());
}

TuningInterval Tuning::at(int n) const
{
  const int index = euclidean_remainder(n - 1, this->degree());
  const auto interval = intervals.at((std::size_t)index);

  const int octave = ((n - 1) / this->degree()) - 1;

  return interval.transpose_octaves(octave);
}

void Tuning::stream_scala(std::ostream &out)
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

void Tuning::stream_table(std::ostream &out)
{
  const auto small_pad = std::setw(6);
  const auto big_pad = std::setw(15);

  out << small_pad << "Index"
      << big_pad << "Str"
      << big_pad << "Cents"
      << big_pad << "Ratio"
      << '\n';

  for (int i = this->degree() * -1; i != this->degree() * 3; ++i)
  {
    const auto interval = this->at(i);
    out << small_pad << i
        << big_pad << interval.str()
        << big_pad << interval.cents()
        << big_pad << interval.ratio()
        << '\n';
  }
}
