cmake_minimum_required(VERSION 3.10)
project(Optane)

set(CMAKE_CXX_STANDARD 14)
set(INC_DIR local/include)
set(LINK_DIR local/lib)
SET(GCC_COVERAGE_COMPILE_FLAGS "-mavx")
SET(GCC_COVERAGE_LINK_FLAGS    "-lpthread")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

#add_executable(string_store_pmdk string_store_pmdk.cpp)
#add_executable(area_calc area_calc.cpp)
add_executable(main main.cpp)
add_executable(bandwidth bandwidth.cpp)

#target_link_libraries(string_store_pmdk PRIVATE pmemobj)
#target_link_libraries(area_calc PRIVATE pmemobj)
target_link_libraries(bandwidth PRIVATE pthread)
