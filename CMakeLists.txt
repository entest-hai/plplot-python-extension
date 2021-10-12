cmake_minimum_required(VERSION 2.8.12) 

set(PROJECT_NAME ctypes)

include_directories(${CMAKE_SOURCE_DIR}/src/
                    ${CMAKE_SOURCE_DIR}/
                    /usr/local/include/
                    /usr/local/include/gsl/
                    /usr/local/include/plplot/)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/plplot_ctg_paper.c)

add_library(test SHARED ${SOURCE_FILES})

set(LINK_LIBS "-lwavelib -lm -lgsl -lgslcblas -lplplot")

target_link_libraries(test ${LINK_LIBS} ${LINK_LIBS})