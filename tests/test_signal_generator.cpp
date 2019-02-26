#include <hyro/test/ComponentTester.h>
#include <hyro/test/FakeChannels.h>
#include <gtest/gtest.h>

#include <../src/internal/SignalGenerator.h>
#include <../src/internal/Thresholding.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/DigitalConverterComponent.h>
#include <hyro/utils/DynamicPropertyAccess.h>

static void
__assert_and_run_state_machine(hyro::StateMachine& sm,
                              const hyro::ComponentConfiguration& conf)
{
  ASSERT_EQ(hyro::Result::RESULT_OK, sm.init(conf));

  ASSERT_EQ(hyro::Result::RESULT_OK, sm.start());

  ASSERT_EQ(hyro::Result::RESULT_OK, sm.check());
}

#define ASSERT_AND_RUN_STATE_MACHINE(sm, conf) \
__assert_and_run_state_machine(sm, conf)

namespace hyro
{
namespace test
{


TEST (InternalClassTest, TestSignalGenerator)
{
  SignalGenerator signalgenerator;

  double value;
  double amp = 3.0;
  double freq = 2.0;
  bool cosine = false;

  signalgenerator.setAmplitude(amp);
  signalgenerator.setFrequency(freq);
  signalgenerator.setCosine(cosine);
  value = signalgenerator.getSignalValue();
  
  EXPECT_EQ(value, 0);
}

TEST (InternalClassTest, TestThresholding)
{
  double thresh = 0.0;
  double amp = 2.0;
  double digital_value;

  Thresholding thresholding(amp, thresh);
  digital_value = thresholding.getSignalThreshold(4.0);
  
  EXPECT_EQ(digital_value, amp);
  
  digital_value = thresholding.getSignalThreshold(-4.0);
  
  EXPECT_EQ(digital_value, 0);
}


TEST (SignalGeneratorTest, SignalGeneratorComponentCheck)
{
	auto signal_conf = "{"
		"outputs: {"
			"output: { protocol: 'api' },"
      "fix_dynamic: { protocol: 'api' }"
		"}"
	"}";

	StateMachine signal_generator_sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
  ASSERT_AND_RUN_STATE_MACHINE(signal_generator_sm, ComponentConfiguration(signal_conf));

	auto inputSignal = std::make_shared<FakeInput<Signal>>("/fake_input"_uri, "api", "/signal/output");
  ASSERT_TRUE(inputSignal->connect());

	signal_generator_sm.update();
	auto valueSignal = std::shared_ptr<const Signal>();

	ReceiveStatus rec_signal = inputSignal->receive(valueSignal, 500ms);
	ASSERT_EQ(ReceiveStatus::RECEIVE_OK, rec_signal);
  EXPECT_EQ(valueSignal->value, 0);

	signal_generator_sm.reset();
}

TEST (DigitalConverterTest, DigitalConverterCheck)
{
 auto configuration = ComponentConfiguration("{inputs: {signals: { protocol: 'api' } },"
                                              "outputs: {digital_signals: { protocol: 'api' }}}");
	
	auto fake_output_signals = std::make_shared<FakeOutput<double>>("fake_signals"_uri, "api");
  ASSERT_TRUE(fake_output_signals->start());

  StateMachine digital_converter_sm(std::make_shared<DigitalConverterComponent>("/digital_converter"_uri));

	ASSERT_EQ(hyro::Result::RESULT_OK, digital_converter_sm.init(configuration));

  ASSERT_EQ(hyro::Result::RESULT_OK, digital_converter_sm.start());

 	ASSERT_EQ(hyro::Result::RESULT_OK, digital_converter_sm.connect(URI("signals"),
                 RuntimeConfiguration("{ endpoint: 'fake_signals' }")));

  ASSERT_EQ(hyro::Result::RESULT_OK, digital_converter_sm.check());
 	
	auto fake_digital_signals = std::make_shared<FakeInput<double>>("digital_signals"_uri, "api", "/digital_converter/digital_signals");
  ASSERT_TRUE(fake_digital_signals->connect());
}

TEST (TestSignalGenerator, DynamicPropertiesCheck)
{
  auto signal_conf = "{"
		"outputs: {"
			"output: { protocol: 'api' },"
      "fix_dynamic: { protocol: 'api' }"
		"}"
	"}";

	StateMachine signal_generator_sm(std::make_shared<SignalGeneratorComponent>("/signal_generator"_uri));
  ASSERT_AND_RUN_STATE_MACHINE(signal_generator_sm, ComponentConfiguration(signal_conf));
  
  DynamicPropertyAccess dynamic_property_access("/signal_generator"_uri);

  float not_exits;
  ASSERT_ANY_THROW(dynamic_property_access.get("not_exists", not_exits));
  ASSERT_TRUE(dynamic_property_access.set("amplitude", 10.0));
  ASSERT_TRUE(dynamic_property_access.set("frequency", 20.0));
  ASSERT_TRUE(dynamic_property_access.set("cosine", true));

  double amplitude, frequency;
  bool cosine;

  ASSERT_TRUE(dynamic_property_access.get("amplitude", amplitude));
  ASSERT_TRUE(dynamic_property_access.get("frequency", frequency));
  ASSERT_TRUE(dynamic_property_access.get("cosine", cosine));

  ASSERT_NEAR(amplitude, 10.0, 0.001);
  ASSERT_NEAR(frequency, 20.0, 0.001);
  ASSERT_EQ(cosine,true);

  signal_generator_sm.reset();
}

//don't change here
int main (int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
} // main
} // namespace test
} // namespace hyro