#include <hyro/core/StateMachine.h>
#include <hyro/utils/StateMachineSpinner.h>
#include <hyro/widgets/WidgetCollection.h>
#include <hyro/factory/CommandFactory.h>
#include <hyro/utils/DynamicPropertyAccess.h>
#include <hyro/msgs/Signal.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/DigitalConverterComponent.h>

using namespace hyro;
using namespace std::string_literals;

int main ()
{
  hyro::LogConfig config;
  config.level = hyro::LogLevel::CRITICAL;
  hyro::HyroLoggerManager::Configure(config);

  StateMachine sig_sm(std::make_shared<SignalGeneratorComponent>("/signal_generator"_uri));
  StateMachine dig_sm(std::make_shared<DigitalConverterComponent>("/digital_converter"_uri));

  ComponentConfiguration signal_config = ComponentConfiguration(
    "{"
       "parameters: {"
          "amplitude: 0.5,"
          "frequency: 0.5,"
          "cosine: true},"
       "outputs: {"
         "output: { protocol: 'api' },"
         "fix_dynamic: { protocol: 'api' }"
      "}"
    "}");

  ComponentConfiguration digital_config =    ComponentConfiguration(
    "{"
       "parameters: {"
          "amplitude: 2,"
          "threshold: 0.0},"
       "inputs: {"
         "signals: {protocol: 'api'}},"
       "outputs: {"
          "digital_signals: {protocol: 'api'},"
          "fix_dynamic: { protocol: 'api' }"
       "}"
    "}");

  sig_sm.init(signal_config);
  dig_sm.init(digital_config);

  sig_sm.start();
  dig_sm.start();

	dig_sm.connect(URI("signals"),RuntimeConfiguration("{ endpoint: '/signal_generator/output' }"));

  sig_sm.check();
  dig_sm.check();

  sig_sm.update();
  dig_sm.update();

  CancellationTokenSource cancellation_token;

  StateMachineSpinner sig_spinner(sig_sm, cancellation_token, 1ms);
  StateMachineSpinner dig_spinner(dig_sm, cancellation_token, 1ms);

  std::this_thread::sleep_for(1s);

  std::thread input_thread([]()
  {
    DynamicPropertyAccess dynamic_property_access_sig("/signal_generator"_uri);
    DynamicPropertyAccess dynamic_property_access_dig("/digital_converter"_uri);
    char ch;
    do
    {
      double  amplitude_sig = 0.0,frequency = 0.0;
      double  amplitude_dig = 0.0,threshold = 0.0;
      bool    cosine = false;

      std::cout << "Set Signal Properties" << std::endl;
      std::cout << "Please inform the Signal Amplitude(Double): ";
      std::cin >> amplitude_sig;
      std::cout << "Please inform the Signal Frequency[Hz](Double): ";
      std::cin >> frequency;
      std::cout << "Please inform cosine (Int): 1 for Cosine, 0 to Sine: ";
      std::cin >> cosine;
      std::cout << "Please inform the Converter Amplitude(Double): ";
      std::cin >> amplitude_dig;
      std::cout << "Please inform the Converter Threshlod(Double): ";
      std::cin >> threshold;

      dynamic_property_access_sig.set<double>("amplitude", amplitude_sig);
      dynamic_property_access_sig.set<double>("frequency", frequency);
      dynamic_property_access_sig.set<bool>("cosine", cosine);
      dynamic_property_access_dig.set<double>("amplitude", amplitude_dig);
      dynamic_property_access_dig.set<double>("threshold", threshold);

      std::cout << "Continue (Y/n): ";
      std::cin >> ch;

    }while (ch != 'n' && ch != 'N');
 	});

	widgets::registerChannelListener<hyro::Signal>("/signal_generator/output", "api", [](Signal signal_msg)
	{
		widgets::plot2d<float>("Signal", "/signal_generator/output/value", signal_msg.value);
	});

	widgets::registerChannelListener<double>("/digital_converter/digital_signals", "api", [](double dgt){
		widgets::plot2d<float>("Digital", "/digital_converter/digital_sig1nals", dgt);
	});

  widgets::exec();

  std::this_thread::sleep_for(2s);

  cancellation_token.cancel();

  input_thread.join();
  sig_spinner.wait();
  dig_spinner.wait();

  return 0;
}