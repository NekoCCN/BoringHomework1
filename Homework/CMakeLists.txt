cmake_minimum_required (VERSION 3.8)

set (HomeWork_SRC
  "Homework.c"
  "Homework.h"
)
add_executable (Homework ${HomeWork_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Homework PROPERTY C_STANDARD 17)
endif()
