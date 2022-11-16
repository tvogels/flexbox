cmake_minimum_required(VERSION 3.15...3.19)

project(flexbox VERSION "0.0.1")

if(NOT SKBUILD)
  message(FATAL_ERROR "This CMake file should be executed via scikit-build. "
                      "Please run\n$ pip install .")
endif()

# Constrain FindPython to find the Python version used by scikit-build
if(SKBUILD)
  message(STATUS "Python_VERSION ${PYTHON_VERSION_STRING}")
  message(STATUS "Python_EXECUTABLE ${PYTHON_EXECUTABLE}")
  message(STATUS "Python_INCLUDE_DIR ${PYTHON_INCLUDE_DIR}")
  message(STATUS "Python_LIBRARIES ${PYTHON_LIBRARY}")
  set(Python_VERSION "${PYTHON_VERSION_STRING}")
  set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
  set(Python_LIBRARIES "${PYTHON_LIBRARY}")
endif()
find_package(
  Python
  COMPONENTS Interpreter Development.Module
  REQUIRED)

if(SKBUILD)
  # Scikit-Build does not add your site-packages to the search path
  # automatically, so we need to add it _or_ the pybind11 specific directory
  # here.
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
            "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE _tmp_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
endif()

# Now we can find pybind11
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
  _core
  MODULE
  src/main.cpp
  src/sources/YGValue.cpp
  src/sources/YGStyle.cpp
  src/sources/YGNodePrint.cpp
  src/sources/YGNode.cpp
  src/sources/YGLayout.cpp
  src/sources/YGEnums.cpp
  src/sources/YGConfig.cpp
  src/sources/Utils.cpp
  src/sources/Yoga.cpp
  src/sources/log.cpp
  src/sources/event/event.cpp
  src/sources/internal/experiments.cpp)

target_compile_features(_core PRIVATE cxx_std_17)

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _core DESTINATION .)
