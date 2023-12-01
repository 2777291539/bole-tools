CXX = g++
FLAG = -m64 -std=c++17 -I$(CURDIR)/src/Include/

SRC = $(wildcard src/*.cpp)
TARGET = bin/gbf

All: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(FLAG) -o $@ $^

clean:
	@rm bin/*

run:
	bin/gbf

.PHONY: clean run
