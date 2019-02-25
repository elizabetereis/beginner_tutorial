#pragma once

#ifndef HYRO_DIGITALCONVERTERCOMPONENT_H
#define HYRO_DIGITALCONVERTERCOMPONENT_H

#include <signal_generator_components_export.h>
#include "../../src/internal/Thresholding.h"
#include <hyro/core/Component.h>

#include <hyro/msgs/common/Basic.h>
#include  <hyro/msgs/Signal.h>

namespace hyro
{

class SIGNAL_GENERATOR_COMPONENTS_EXPORT DigitalConverterComponent : public hyro::Component
{
public:

  DigitalConverterComponent (const URI & uri);
  virtual ~DigitalConverterComponent () override = default;

  virtual hyro::Result init (const hyro::ComponentConfiguration & config) override;
  virtual hyro::Result reset () override;
  virtual hyro::Result check () override;
  virtual hyro::Result start () override;
  virtual hyro::Result update () override;

private:

  bool setAmplitude(double amp);
  double getAmplitude();
  bool setThreshold(double thresh);
  double getThreshold();

  Thresholding m_thresh;

  std::shared_ptr<hyro::ChannelInput<Signal>> m_input;
  std::shared_ptr<hyro::ChannelOutput<double>> m_output;

  static std::shared_ptr<hyro::HyroLogger> s_logger;

};
} // namespace hyro
#endif // HYRO_DIGITALCONVERTERCOMPONENT_H