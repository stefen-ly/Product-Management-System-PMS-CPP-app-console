cmake_minimum_required(VERSION 3.14)
project(ExcelTableApp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch xlnt
FetchContent_Declare(
  xlnt
  GIT_REPOSITORY https://github.com/tfussell/xlnt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(xlnt)

# Fetch tabulate
FetchContent_Declare(
  tabulate
  GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
  GIT_TAG master
)
FetchContent_MakeAvailable(tabulate)

# Add your main.cpp or other source files
add_executable(ExcelTableApp main.cpp src/Student.cpp src/ExcelUtils.cpp src/MenuUtils.cpp)
# Tell the compiler where to find all the headers 
target_include_directories(ExcelTableApp PRIVATE include )
# Link libraries
target_link_libraries(ExcelTableApp PRIVATE xlnt tabulate)
