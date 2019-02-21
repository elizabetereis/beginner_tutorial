#include "SignalGenerator.h"

SignalGenerator::~SignalGenerator()
{

}

bool SignalGenerator::setAmplitude(const double amp)
{
  m_amp = amp;
  return true;
}

double SignalGenerator::getAmplitude()
{
  return m_amp;
}

bool SignalGenerator::setFrequency(const double freq)
{
  m_freq = freq;
  return true;
}

double SignalGenerator::getFrequency()
{
  return m_freq;
}

bool SignalGenerator::setCosine(const bool cosine)
{
  m_cosine = cosine;
  return true;
}

bool SignalGenerator::getCosine()
{
  return m_cosine;
}

double SignalGenerator::getSignalValue()
{
  double rad = m_deg / 180.0 * M_PI;
  m_deg += m_freq*0.360;
  return m_cosine?(m_amp * cos(rad)):(m_amp * sin(rad));
}