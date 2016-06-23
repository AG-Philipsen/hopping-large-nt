ifeq ($(CXX),g++)
 CXX11FLAG = -std=c++0x
else
 CXX11FLAG = -std=c++11
endif

SUBDIRS := position

CXXFLAGS_$(d) := -Wall $(CXX11FLAG) -pthread
INCLUDES_$(d) := $(TOP)/gtest/include $(TOP)

SRCS := *.cpp
