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
add_executable(ExcelTableApp main.cpp src/category.cpp src/product_management_system.cpp src/order.cpp src/product.cpp src/restock_log.cpp src/user.cpp src/utils.cpp )
# Tell the compiler where to find all the headers 
target_include_directories(ExcelTableApp PRIVATE include )
# Link libraries
target_link_libraries(ExcelTableApp PRIVATE xlnt tabulate)

add_custom_command(TARGET ExcelTableApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/build/_deps/xlnt-build/source/Release/xlnt.dll"
        $<TARGET_FILE_DIR:ExcelTableApp>)