ifeq ($(CXX),g++)
 CXX11FLAG = -std=c++0x
else
 CXX11FLAG = -std=c++11
endif

TARGETS := main.out
SUBDIRS := std_libs

SRCS := *.cpp

CXXFLAGS := -g $(CXX11FLAG)
CFLAGS := -g -Wall -W -Os

main.out_DEPS = $(OBJS_$(d)) $(TARGETS_$(d)/std_libs)

ifeq ($(BUILD_MODE),release)
	CXXFLAGS += -DNDEBUG
	OPT_FLAGS := -O3
else
	CXXFLAGS += -D_DEBUG
	OPT_FLAGS := -O0
endif

main.out_LIBS += -lboost_program_options 
