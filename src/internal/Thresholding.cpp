#include "Thresholding.h"

Thresholding::~Thresholding()
{
}

bool Thresholding::setThreshold(const double thresh)
{
  m_thresh = thresh;
  return true;
}

double Thresholding::getThreshold()
{
  return m_thresh;
}
bool Thresholding::setAmplitude(const double amp)
{
  m_amp = amp;
  return true;
}
double Thresholding::getAmplitude()
{
  return m_amp;
}

double Thresholding::getSignalThreshold(const double signal)
{
  return signal > m_thresh ? m_amp : 0;
}