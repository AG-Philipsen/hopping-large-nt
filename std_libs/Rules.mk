ifeq ($(CXX),g++)
 CXX11FLAG = -std=c++0x
else
 CXX11FLAG = -std=c++11
endif

CXXFLAGS_$(d) := $(CXX11FLAG)

TARGETS := std_lib.a
SRCS := *.cpp

std_lib.a_DEPS = $(OBJS_$(d))

INHERIT_DIR_VARS_$(d) := CXXFLAGS
