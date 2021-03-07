TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_DEBUG   += -Wall -Wextra
QMAKE_CXXFLAGS_DEBUG += -Wall -Wextra  -Wshadow -std=c++17 -O0

QMAKE_CFLAGS_RELEASE   += -Wall -Wextra
QMAKE_CXXFLAGS_RELEASE += -Wall -Wextra -Wshadow -std=c++17 -Ofast -fexpensive-optimizations -mtune=native -march=native -flto -funroll-loops -frename-registers -fvisibility=hidden -fvisibility-inlines-hidden

SOURCES += \
        $$PWD/../../src/targets/unittest/main.cpp \
        $$PWD/../../the_framework/math/math_discrete.cpp \
        $$PWD/../../the_framework/unittest/utest_bitmacros.cpp \
        $$PWD/../../the_framework/unittest/utest_math_discrete.cpp \
        $$PWD/../../the_framework/unittest/utest_meta.cpp \
        $$PWD/../../src/unittest/utest_statemachine.cpp

HEADERS += \
  $$PWD/../../the_framework/core/bitmacros.h \
  $$PWD/../../the_framework/core/compile_guards.h \
  $$PWD/../../the_framework/core/dtypes.h \
  $$PWD/../../the_framework/core/meta.h \
  $$PWD/../../the_framework/math/math_discrete.h \
  $$PWD/../../src/runtime/statemachine.h \
  $$PWD/../../src/targets/unittest/global_config.h \
  $$PWD/../../extern/Catch2/single_include/catch2/catch.hpp \
  $$PWD/../../the_framework/math/simple_hash_fct.h

INCLUDEPATH += $$PWD/../../extern/Catch2/single_include/catch2
INCLUDEPATH += $$PWD/../../the_framework/core
INCLUDEPATH += $$PWD/../../the_framework/math
INCLUDEPATH += $$PWD/../../the_framework/targets/unittest
INCLUDEPATH += $$PWD/../../src/runtime
INCLUDEPATH += $$PWD/../../src/targets
INCLUDEPATH += $$PWD/../../src/targets/unittest
INCLUDEPATH += $$PWD

DISTFILES += \
    $$PWD/../../src/unittest/catch_test_list.txt
