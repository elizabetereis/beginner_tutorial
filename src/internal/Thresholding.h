#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <math.h>

class Thresholding
{
public:
  Thresholding()
    : m_amp(1)
    , m_thresh(0) {}

  Thresholding(const double amp,const double thresh)
    : m_amp(amp)
    , m_thresh(thresh) {}

  ~Thresholding();

  double getSignalThreshold(const double signal);

  bool   setThreshold(const double thresh);
  double getThreshold();
  bool   setAmplitude(const double amp);
  double getAmplitude();

private:
  double m_amp;
  double m_thresh;   
};

#endif // THRESHOLDING_H