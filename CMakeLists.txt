#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# B U I L D State Machine development project         
# Danny Schneider, 2020
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-


#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
if(${CMAKE_VERSION} VERSION_LESS "3.15.0")
  message("Please consider to switch to CMake >=3.15.0")
endif()
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-


#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

set(ApplName unittest)
message(STATUS "${ApplName}")
string(CONCAT ApplName_AS ${ApplName} "_sanitize")
message(STATUS "${ApplName_AS}")

project(${ApplName} LANGUAGES C CXX)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

##################################################

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

##################################################

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# List of Sourcefiles
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

#framework stuff
set(SOURCES_FW the_framework/math/math_discrete.cpp
               the_framework/unittest/utest_bitmacros.cpp
               the_framework/unittest/utest_math_discrete.cpp
               the_framework/unittest/utest_meta.cpp
)

#---

#Unittest Stuff
set(SOURCES_UTEST src/targets/unittest/main.cpp
                  "${SOURCES_FW}"
                  src/unittest/utest_statemachine.cpp
)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# Target Setup
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

# add the executable
add_executable(${ApplName} "${SOURCES_UTEST}")

add_executable(${ApplName_AS} "${SOURCES_UTEST}")

# add the executable
#if(CMAKE_BUILD_TYPE MATCHES ASAN) 
#endif(CMAKE_BUILD_TYPE MATCHES ASAN)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# Includes
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

set(INCLUDES_SRC
  src/runtime  
  src/unittest
)

set(INCLUDES_TF
  the_framework/core  
  the_framework/math
  the_framework/unittest
)

set(INCLUDES_TARGET_UNITTEST
  src/targets/unittest
)

set(INCLUDES_EXTERNAL
  extern/Catch2/single_include/catch2
)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

#Unit-Test Executeable
target_include_directories("${ApplName}"
  PRIVATE
  ${INCLUDES_SRC}
  ${INCLUDES_TF}
  ${INCLUDES_TARGET_UNITTEST}
  ${INCLUDES_EXTERNAL}  
)

#Unit-Test Executeable with Sanitizer
target_include_directories("${ApplName_AS}"
  PRIVATE
  ${INCLUDES_SRC}
  ${INCLUDES_TF}
  ${INCLUDES_TARGET_UNITTEST}
  ${INCLUDES_EXTERNAL}
)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# Linker Options
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

message("System Name = ${CMAKE_SYSTEM_NAME}")
message("Used Linker = ${CMAKE_LINKER}")
message("is MINGW    = ${MINGW}")
message("is MSYS     = ${MSYS}")
message("is ISMSYS   = ${ISMSYS}")
message("is CYGWIN   = ${CYGWIN}")

#set(CMAKE_EXE_LINKER_FLAGS "-s -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${exe_name}.map")
set(MAPFILE ${CMAKE_CURRENT_BINARY_DIR}/${exe_name}.map)
target_link_options("${ApplName}" PRIVATE
  "-s"
  "-Wl,-Map=${MAPFILE}"
)

target_link_options("${ApplName_AS}" PRIVATE
  "-s"
  "-Wl,-Map=${MAPFILE}"
  "-fsanitize=address"
)

#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-
# Compile Options
#-=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=-

set(OPTS_WARNING   "-Wcpp"
                   "-Wall"
                   "-Wextra"
                   "-Wshadow"
                   "-Wno-deprecated"
)

set(OPTS_DEBUG     "-O0"
)

set(OPTS_RELEASE   "-Ofast"
                   "-fexpensive-optimizations"
)

set(OPTS_TUNE      "-mtune=native"
                   "-march=native"
)

set(OPTS_TUNEPERF  "-flto"
                   "-funroll-loops"
		   "-frename-registers"

)

set(OPTS_SANITIZER "-ggdb"
                   "-fsanitize=address"
		   "-fno-omit-frame-pointer"
                   "-static-libstdc++"
                   "-static-libasan"
		   "-lrt"
)

#Unit-Test Executeable
target_compile_options("${ApplName}" PUBLIC -std=c++17 "${OPTS_WARNING}" "${OPTS_DEBUG}" "${OPTS_TUNE}")

#Unit-Test Executeable with Sanitizer
target_compile_options("${ApplName_AS}" PUBLIC -std=c++17 "${OPTS_WARNING}"  "${OPTS_DEBUG}" "${OPTS_TUNE}")

#Unit-Test Executeable with Sanitizer
#if(CMAKE_BUILD_TYPE MATCHES ASAN)
#endif(CMAKE_BUILD_TYPE MATCHES ASAN)

# End of CMakeLists.Txt file
