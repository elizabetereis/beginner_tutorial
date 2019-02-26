hyro_init_package(signal_generator VERSION 0.0.1) 

hyro_find_package(hyro_utils REQUIRED)
hyro_find_package(hyro_msgs REQUIRED)
hyro_find_package(hyro_python REQUIRED)
hyro_find_package(hyro_widgets REQUIRED)

set(SIGNAL_HEADERS
  include/hyro/SignalGeneratorComponent.h
  include/hyro/DigitalConverterComponent.h
  include/hyro/msgs/Signal.h
  include/hyro/msgs/Signal.proto.h
  src/internal/SignalGenerator.h
  src/internal/Thresholding.h
)
set(SIGNAL_SOURCES
  src/components/SignalGeneratorComponent.cpp
  src/components/DigitalConverterComponent.cpp
  src/internal/SignalGenerator.cpp
  src/internal/Thresholding.cpp
)

hyro_add_library(
  ${PROJECT_NAME}_components
  HEADERS 
    ${SIGNAL_HEADERS}
  SOURCES 
    ${SIGNAL_SOURCES}
  DEPENDS_ON
    hyro_utils
    hyro_msgs
    hyro_widgets
)

hyro_compile_messages(
  ${PROJECT_NAME}_components
    SOURCES
      msgs/hyro/msgs/Signal.proto
)

hyro_add_executable(${PROJECT_NAME}
  SOURCES src/examples/signal_generator.cpp
  DEPENDS_ON ${PROJECT_NAME}_components
)

hyro_add_executable(${PROJECT_NAME}_dynamic
  SOURCES src/examples/main_dynamic_properties.cpp
  DEPENDS_ON ${PROJECT_NAME}_components
)

hyro_add_python_extension(${PROJECT_NAME}
  PACKAGE hyro
  SOURCES
    src/bindings/python/signal_generator.cpp
  DEPENDS_ON
    ${PROJECT_NAME}_components
    hyro_python
)

hyro_install_package(${PROJECT_NAME}
  LIBS
    ${PROJECT_NAME}_components
  EXES
    ${PROJECT_NAME}
)

hyro_add_gtest(${PROJECT_NAME}
  SOURCES
    tests/test_signal_generator.cpp
  DEPENDS_ON
    ${PROJECT_NAME}_components
)