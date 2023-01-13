CXX = g++
LIBS = -lGL -lGLU -lglut
INCLUDES = -I include
MODE = DEBUG 
ifeq ("$(MODE)", "DEBUG")
CXXFLAGS = -g $(INCLUDES)
else
CXXFLAGS = -g $(INCLUDES)
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
	$(CXX) -o $(BIN) $(OBJS) $(LIBS)

depend:
	$(CXX) -MM $(CXXFLAGS) $(SRCS) > $(DEPS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cpp, $^)

clean:
	rm -rf *.o
	rm -rf $(BIN)
