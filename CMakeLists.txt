cmake_minimum_required(VERSION 2.8.12) 

set(PROJECT_NAME ctypes)

include_directories(${CMAKE_SOURCE_DIR}/src/)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/add.c)

# add_executable(app ${SOURCE_FILES})

add_library(test SHARED ${SOURCE_FILES})

# target_link_libraries(app -lm -lgsl -lgslcblas -lwavelib -lplplot)

# set(LINK_LIBS "-lwavelib -lm -lgsl -lgslcblas -lplplot")

# target_link_libraries(femom ${LINK_LIBS} ${LINK_LIBS})

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Bin)

# set_property(TARGET app PROPERTY C_STANDARD 99)

# set_property(TARGET femom PROPERTY C_STANDARD 99)