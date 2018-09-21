BASE_DIR := $(CURDIR)

INCLUDE_DIR := -I ${BASE_DIR}/export
INCLUDE_DIR += -I ${BASE_DIR}/export/common
INCLUDE_DIR += -I ${BASE_DIR}/source/include
INCLUDE_DIR += -I ${BASE_DIR}/source/bsp/dummy
INCLUDE_DIR += -I ${BASE_DIR}/source/switch/stub

all: example/main.o              \
     source/export/iboard.o      \
     source/export/iport.o       \
     source/export/common/port.o
	g++ -ggdb3 -std=c++1z ${INCLUDE_DIR} $^ -o $@

%.o: %.cpp
	g++ -ggdb3 -std=c++1z -c ${INCLUDE_DIR} $^ -o $@

clean:
	rm -rf all main.o
	find . -name '*.o' -exec rm -f {} \;
