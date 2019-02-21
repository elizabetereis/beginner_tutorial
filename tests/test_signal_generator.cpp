#include <hyro/test/ComponentTester.h>
#include <hyro/test/FakeChannels.h>
#include <gtest/gtest.h>

#include <../src/internal/SignalGenerator.h>
#include <../src/internal/Thresholding.h>
#include <hyro/SignalGeneratorComponent.h>
#include <hyro/DigitalConverterComponent.h>


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
			"output: { protocol: 'api' }"
		"}"
	"}";

	StateMachine signal_generator_sm(std::make_shared<SignalGeneratorComponent>("/signal"_uri));
	ASSERT_EQ(hyro::Result::RESULT_OK, signal_generator_sm.init(ComponentConfiguration(signal_conf)));
  ASSERT_EQ(hyro::Result::RESULT_OK, signal_generator_sm.start());
  ASSERT_EQ(hyro::Result::RESULT_OK, signal_generator_sm.check());

	auto inputSignal = std::make_shared<FakeInput<double>>("/fake_input"_uri, "api", "/signal/output");
  ASSERT_TRUE(inputSignal->connect());

	signal_generator_sm.update();
	auto valueSignal = std::shared_ptr<const double>();

	ReceiveStatus rec_signal = inputSignal->receive(valueSignal, 500ms);
	ASSERT_EQ(ReceiveStatus::RECEIVE_OK, rec_signal);
  EXPECT_EQ(*valueSignal, 0);

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
//don't change here
int main (int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
} // main
} // namespace test
} // namespace hyro