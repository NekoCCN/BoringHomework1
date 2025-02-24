cmake_minimum_required (VERSION 3.8)

set (HomeWork_SRC
  "Homework.cpp"
  "Homework.h"
  "Widget/Button/Button.h"
  "Widget/Button/Button.cpp"
  "Tools/Tools.h"
  "Tools/Tools.cpp"
  "Data/StringSkipList/StringSkipList.h"
  "Data/StringSkipList/StringSkipList.cpp"
  "Data/BaseStruct/BaseStruct.h"
  "Widget/ListButton/ListButton.h"
  "Widget/ListButton/ListButton.cpp"
  "Data/DataFactory/DataFactory.h"
  "UI/Sidebar/Sidebar.h"
  "UI/Sidebar/Sidebar.cpp"
  "Widget/MaterialList/MaterialList.h"
  "Widget/MaterialList/MaterialList.cpp"
  "Widget/MaterialInput/MaterialInput.h"
  "Widget/MaterialInput/MaterialInput.cpp"
  "UI/QueryBook/QueryBook.h"
  "UI/QueryBook/QueryBook.cpp"
  "UI/Insert/Insert.h" 
  "UI/QueryBookType/QueryBookType.h"
  "Data/Vector/Vector.h"
  "Data/Vector/Vector.cpp"
  "Data/SRVector/SRVector.h"
  "Data/SRVector/SRVector.cpp"
  "UI/QueryBookType/QueryBookType.cpp"
  "Data/DataFactory/DataFactory.cpp"
 "UI/Insert/Insert.cpp")

add_executable (Homework ${HomeWork_SRC})

set (HomeWork_Require
  xege
  # imm32.lib 现在不需要显式链接它
  # 补充 现在不需要它了
)

target_link_libraries (Homework PUBLIC ${HomeWork_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Homework PROPERTY C_STANDARD 17)
endif()
