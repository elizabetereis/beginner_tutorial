#include <hyro/SignalGeneratorComponent.h>

namespace hyro
{

std::shared_ptr<hyro::HyroLogger> SignalGeneratorComponent::s_logger = hyro::HyroLoggerManager::CreateLogger("SignalGeneratorComponent");

SignalGeneratorComponent::SignalGeneratorComponent (const URI & uri)
	: Component_<MyInternalStatus>(uri)
{   
	auto internal = std::make_shared<ComponentStatus<MyInternalStatus>>();
  internal->addStatusDescription(MyInternalStatus::OK, "Everything is ok");
  internal->addStatusDescription(MyInternalStatus::BAD, "Bad boy");
  internal->addStatusDescription(MyInternalStatus::SOMETHING, "Something good... or wrong");
  internal->addStatusDescription(MyInternalStatus::NONE, "No comment");

  this->registerStatus(internal);
};


hyro::Result
SignalGeneratorComponent::init (const hyro::ComponentConfiguration & config)
{
	this->setStatus(MyInternalStatus::OK);
  
	m_output = this->registerOutput<Signal>("output"_uri, config);
  
  return hyro::Result::RESULT_OK;
}

hyro::Result
SignalGeneratorComponent::reset ()
{
	this->setStatus(MyInternalStatus::OK);

	m_output.reset();
  this->stopCurrentStatusStream();

  return hyro::Result::RESULT_OK;
}

hyro::Result
SignalGeneratorComponent::check ()
{
  return hyro::Result::RESULT_OK;
}

hyro::Result
SignalGeneratorComponent::start ()
{ 
	this->startCurrentStatusStream(2);
  
	return hyro::Result::RESULT_OK;
}

hyro::Result
SignalGeneratorComponent::update()
{
  Signal value;
  value.timestamp = std::time(0);
  value.frame_id = "Signal";
  value.value = m_signal_generator.getSignalValue();
  m_output->sendAsync(value);

  auto currentStatus = this->getStatus();

  if (currentStatus == MyInternalStatus::OK)
    this->setStatus(MyInternalStatus::BAD);
  else if (currentStatus == MyInternalStatus::BAD)
    this->setStatus(MyInternalStatus::NONE);
  else if (currentStatus == MyInternalStatus::NONE)
    this->setStatus(MyInternalStatus::SOMETHING);
  else if (currentStatus == MyInternalStatus::SOMETHING)
    this->setStatus(MyInternalStatus::OK);

  if (m_callback)
    m_callback(this->getStatus());

  return hyro::Result::RESULT_OK;
}

bool SignalGeneratorComponent::setAmplitude(double amp)
{ 
  return m_signal_generator.setAmplitude(amp);
}

double SignalGeneratorComponent::getAmplitude()
{
  return m_signal_generator.getAmplitude();
}

bool SignalGeneratorComponent::setFrequency(double freq)
{
  return m_signal_generator.setFrequency(freq);
}

double SignalGeneratorComponent::getFrequency()
{
  return m_signal_generator.getFrequency();
}

bool SignalGeneratorComponent::setCosine(bool cosine)
{
  return m_signal_generator.setCosine(cosine);
}

bool SignalGeneratorComponent::getCosine()
{
  return m_signal_generator.getCosine();
}

} // namespace hyro