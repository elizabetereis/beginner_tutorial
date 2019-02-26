  #include <hyro/core/StateMachine.h>
  #include <hyro/utils/StateMachineSpinner.h>
  #include <hyro/widgets/WidgetCollection.h>
  #include <hyro/factory/CommandFactory.h>
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
			"outputs: {"
				"output: { protocol: 'api' }"
				"}"
		"}");      

    ComponentConfiguration digital_config = ComponentConfiguration(
		"{"
				"inputs: {"
					"signals: { protocol: 'api' }" 
				"},"
				"outputs: {"
					"digital_signals: { protocol: 'api' }"
				"}"
		"}");

    sig_sm.init(signal_config);
    dig_sm.init(digital_config);

    sig_sm.start();
    dig_sm.start();

    dig_sm.connect(URI("signals"), RuntimeConfiguration("{ endpoint: '/signal_generator/output' }"));

    sig_sm.check();
    dig_sm.check();

    sig_sm.update();
    dig_sm.update();

    CancellationTokenSource cancellation_token;

    StateMachineSpinner sig_spinner(sig_sm, cancellation_token, 1ms);
    StateMachineSpinner dig_spinner(dig_sm, cancellation_token, 1ms);

    widgets::registerChannelListener<hyro::Signal>("/signal_generator/output", "api", [](Signal signal_msg)
    {
      widgets::plot2d<float>("Signal", "/signal_generator/output/value", signal_msg.value);
    });

    widgets::registerChannelListener<double>("/digital_converter/digital_signals", "api", [](double dgt){
      widgets::plot2d<float>("Digital", "/digital_converter/digital_signals", dgt);
    });

    widgets::exec();

    cancellation_token.cancel();

    sig_spinner.wait();
    dig_spinner.wait();
    widgets::reset();

    return 0;
  }