#include <hyro/python/bindings.h>

#include <hyro/SignalGeneratorComponent.h>
#include <hyro/DigitalConverterComponent.h>

using namespace hyro;

PYBIND11_MODULE(signal_generator, m) {
  component_class<SignalGeneratorComponent> signal_generator(m, "SignalGeneratorComponent");
  signal_generator.def_init();
  signal_generator.def_status();

  component_class<DigitalConverterComponent> digital_converter(m, "DigitalConverterComponent");
  digital_converter.def_init();
  digital_converter.def_status();
}