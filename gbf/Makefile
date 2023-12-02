CXX = g++
CXXFLAG = -m64 -std=c++17 -I$(INCLUDE)

INCLUDE = $(CURDIR)/src/gbf/Include

SRCS := $(shell find src/gbf -type f -name "*.cpp")

TARGET = bin/gbf

All: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAG) -o $@ $^

clean:
	@rm bin/*

run:
	bin/gbf

.PHONY: clean run
