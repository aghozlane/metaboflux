cmake_minimum_required(VERSION 2.6)

project(MetaBoFlux)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_VERBOSE_MAKEFILE=ON)

#-----------------------------------------------------------------------------------------------
# Compilation option
#-----------------------------------------------------------------------------------------------
add_definitions(-Wall -lstdc++ -o3)

#-----------------------------------------------------------------------------------------------
# Find need packages
#-----------------------------------------------------------------------------------------------
INCLUDE (FindMPI.cmake)
if ( NOT MPI_FOUND )
    message(FATAL_ERROR "MPI not found !")
endif ( NOT MPI_FOUND )

INCLUDE (FindGSL.cmake)
if ( NOT GSL_FOUND )
    message(FATAL_ERROR "GSL not found !")
endif ( NOT GSL_FOUND )

INCLUDE (FindLibXml2.cmake)
if ( NOT LIBXML2_FOUND )
    message(FATAL_ERROR "LIBXML2 not found !")
endif ( NOT LIBXML2_FOUND )

INCLUDE (FindLibsbml.cmake)
if ( NOT LIBSBML_FOUND )
    message(FATAL_ERROR "LIBSBML not found !")
endif ( NOT LIBSBML_FOUND )

#-----------------------------------------------------------------------------------------------
# Include path
#-----------------------------------------------------------------------------------------------
include_directories(include)
include_directories(${MPI_INCLUDE_PATH})
include_directories(${GSL_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})

add_subdirectory (doc)

#-----------------------------------------------------------------------------------------------
# Find files
#-----------------------------------------------------------------------------------------------
file(
        GLOB_RECURSE
        source_files
        src/*
)
file(
        GLOB_RECURSE
        include_files
        include/*
)
file(GLOB py_file "./bin/MetaBoFlux.py")
file(GLOB r_file "./bin/MetaBoFlux_stat.R")
file(
        GLOB_RECURSE
        lib_files
        lib/*
)


if(UNIX)
    link_libraries(m)
endif(UNIX)

add_executable(
  MetaBoFlux.exe
  ${source_files}
  ${include_files}
)

target_link_libraries(
    MetaBoFlux.exe	
    ${GSL_LIBRARIES}
    ${MPI_LIBRARY}
    ${MPI_EXTRA_LIBRARY}
    ${LIBXML2_LIBRARIES}
    ${LIBSBML_LIBRARIES}
)


install(TARGETS MetaBoFlux.exe RUNTIME DESTINATION bin)
install(FILES  ${lib_files}  DESTINATION lib)
install(FILES  ${py_file}  DESTINATION bin)
install(FILES  ${r_file}  DESTINATION bin)
install(FILES  ${source_files}  DESTINATION src)
install(FILES  ${include_files}  DESTINATION include)