CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = sigconecta
SRC = src/sigconecta.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
