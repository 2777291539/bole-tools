CXX = g++
CXXFLAG = -m64 -std=c++17 -O3 -I$(INCLUDE)

OS := $(shell uname)

ifeq ($(OS), Linux)
	CXXFLAG += -D__LINUX__
else ifeq ($(OS), Darwin)
	CXXFLAG += -D__MACOS__
endif

INCLUDE = $(CURDIR)/src/gbf/Include

SRCS := $(shell find src/gbf -type f -name "*.cpp")

TARGET = bin/gbf

All: $(TARGET)

$(TARGET): $(SRCS)
	@$(CXX) $(CXXFLAG) -o $@ $^

clean:
	@rm bin/gbf

run:
	@bin/gbf

.PHONY: clean run
