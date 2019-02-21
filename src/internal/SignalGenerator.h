#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <math.h>
#include <iostream>

#define NUM_TO_HERTZ  0.360

class SignalGenerator
{
public:
  SignalGenerator()  
    : m_amp(1)
    , m_freq(1)
    , m_cosine(false)
    , m_deg(0) {}

  SignalGenerator(const double amp,const double freq,const bool cosine)
    : m_amp(amp)
    , m_freq(freq)
    , m_cosine(cosine)
    , m_deg(0) {}

  ~SignalGenerator();
    
	double getSignalValue();

  bool    setAmplitude(const double amp);
  double  getAmplitude();
  bool    setFrequency(const double freq);
  double  getFrequency();
  bool    setCosine(const bool cosine);
  bool    getCosine();

	private:
  double  m_amp;
  double  m_freq;
  bool    m_cosine;
  double  m_deg;
};

#endif // SIGNALGENERATOR_H