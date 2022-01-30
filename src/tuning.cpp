#include "tuning.hpp"

double TuningInterval::cents() const
{
  return std::visit(TuningIntervalCentsVisitor{}, this->v);
}

double TuningInterval::ratio() const
{
  return std::visit(TuningIntervalRatioVisitor{}, this->v);
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
  return std::visit(TuningIntervalStrVisitor{}, this->v);
}
