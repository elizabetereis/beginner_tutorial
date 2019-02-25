#pragma once

#ifndef HYRO_SIGNALGENERATORCOMPONENT_H
#define HYRO_SIGNALGENERATORCOMPONENT_H

#include <signal_generator_components_export.h>  

#include <string>
#include <hyro/core/Component.h>
#include <hyro/msgs/common/Basic.h>

#include "../src/internal/SignalGenerator.h"
#include  <hyro/msgs/Signal.h>

namespace hyro
{

enum class MyInternalStatus
{
  NONE, OK, BAD, SOMETHING
};

namespace
{
  static const std::vector<const char *> MyInternalStatus_string = { "NONE", "OK", "BAD", "SOMETHING" };
}

std::ostream &
operator << (std::ostream & os,
             const MyInternalStatus & status)
{
  return os << MyInternalStatus_string[static_cast<size_t>(status)];
}


class SIGNAL_GENERATOR_COMPONENTS_EXPORT SignalGeneratorComponent : public Component_<MyInternalStatus>
{
public:

	SignalGeneratorComponent (const URI & uri);
  virtual ~SignalGeneratorComponent () override = default;

  virtual hyro::Result init (const hyro::ComponentConfiguration & config) override;
  virtual hyro::Result reset () override;
  virtual hyro::Result check () override;
  virtual hyro::Result start () override;
  virtual hyro::Result update () override;

  void
  registerCallback(std::function<void (MyInternalStatus)> f)
  {
    m_callback = f;
  }


private:

  bool   setAmplitude(double amp);
  double getAmplitude();
  bool   setFrequency(double freq);
  double getFrequency();
  bool   setCosine(bool cosine);
  bool   getCosine();

  SignalGenerator m_signal_generator;

  static std::shared_ptr<hyro::HyroLogger> s_logger;

  std::shared_ptr<hyro::ChannelOutput<Signal>> m_output;

  std::function<void (MyInternalStatus)> m_callback;
};

} //namespace hyro
#endif // HYRO_SIGNALGENERATORCOMPONENT_H