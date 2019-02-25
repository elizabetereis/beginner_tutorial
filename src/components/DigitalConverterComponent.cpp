#include <hyro/DigitalConverterComponent.h>

namespace hyro
{

std::shared_ptr<hyro::HyroLogger> DigitalConverterComponent::s_logger = hyro::HyroLoggerManager::CreateLogger("DigitalConverterComponent");

DigitalConverterComponent::DigitalConverterComponent (const URI & uri)
  : Component(uri)
{   
  
};

hyro::Result
DigitalConverterComponent::init (const hyro::ComponentConfiguration & config)
{
	
	double amp,thresh;
	amp = config.parameters.getParameter<double>("amplitude", 2.0);
	thresh = config.parameters.getParameter<double>("threshold", 0.0);
	m_thresh = Thresholding(amp,thresh);

	m_input = this->registerInput<Signal>("signals"_uri, config);
	m_output = this->registerOutput<double>("digital_signals"_uri, config);
	s_logger->info("Init:");
    
  return hyro::Result::RESULT_OK;
}

hyro::Result
DigitalConverterComponent::reset ()
{
	m_input.reset();
  m_output.reset();
  
  return hyro::Result::RESULT_OK;
}

hyro::Result
DigitalConverterComponent::check ()
{
	
	return hyro::Result::RESULT_OK;

}

hyro::Result
DigitalConverterComponent::start ()
{
	
	return hyro::Result::RESULT_OK;
}

hyro::Result
DigitalConverterComponent::update()
{
	auto value = std::shared_ptr<const Signal>();
  auto ret = m_input->receive(value, 0s);

  if (ret == ReceiveStatus::RECEIVE_OK)
  {
    float thresh_signal = m_thresh.getSignalThreshold(value->value);
    m_output->sendAsync(thresh_signal);
  }

  return hyro::Result::RESULT_OK;
}

bool DigitalConverterComponent::setAmplitude(double amp)
{
  return m_thresh.setAmplitude(amp);
}

double DigitalConverterComponent::getAmplitude()
{
  return m_thresh.getAmplitude();
}

bool DigitalConverterComponent::setThreshold(double thresh)
{
  return m_thresh.setThreshold(thresh);
}

double DigitalConverterComponent::getThreshold()
{
  return m_thresh.getThreshold();
}

} // namespace hyro