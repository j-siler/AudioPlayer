# Define the compiler
CXX = g++

# Define the compiler flags
CXXFLAGS = -std=c++11 -Wall -pthread

# Define the linker flags
LDFLAGS = -L/opt/homebrew/lib -lsndfile -lportaudio

# Define the target executable
TARGET = audio_player_producer_consumer

# Define the source files
SRCS = main.cpp producer.cpp paCallback.cpp

# Define the header files
HEADERS = AudioBuffer.h producer.h

# Define the object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
