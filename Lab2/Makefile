CXX = g++
MODE = DEBUG
ifeq ("$(MODE)", "DEBUG")
CXXFLAGS = -std=c++14 -Iinclude -g
else
CXXFLAGS = -std=c++14 -Iinclude
endif
BIN = raytracer

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = Makefile.depend

ifneq ("$(wildcard $(DEPS))", "")
include $(DEPS)
endif

.PHONY: all

all: $(BIN) depend

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJS)

depend:
	$(CXX) -MM $(CXXFLAGS) $(SRCS) > $(DEPS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cpp, $^)

clean:
	rm -rf *.o
	rm -rf $(BIN)
