cmake_minimum_required (VERSION 3.8)

set (HomeWork_SRC
  "Homework.cpp"
  "Homework.h"
  "Widget/Buttom/Buttom.h"
  "Widget/Buttom/Buttom.cpp"
  "Tools/Tools.h"
  "Tools/Tools.cpp"
  "Data/StringSkipList/StringSkipList.h"
  "Data/StringSkipList/StringSkipList.cpp"
  "Data/BaseStruct/BaseStruct.h"
 "Widget/ListButtom/ListButtom.h" "Widget/ListButtom/ListButtom.cpp" "Data/DataFactory/DataFactory.h" "UI/Sidebar/Sidebar.h" "UI/Sidebar/Sidebar.cpp" "Widget/MaterialList/MaterialList.h" "Widget/MaterialList/MaterialList.cpp")

add_executable (Homework ${HomeWork_SRC})

set (HomeWork_Require
  xege
)

target_link_libraries (Homework PUBLIC ${HomeWork_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Homework PROPERTY C_STANDARD 17)
endif()
