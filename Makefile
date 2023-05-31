CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 `pkg-config gtkmm-3.0 --cflags`
LDFLAGS = `pkg-config gtkmm-3.0 --libs`

SRCS = graph.cpp gtk.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean


