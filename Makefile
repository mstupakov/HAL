BASE_DIR := $(CURDIR)

INCLUDE_DIR := -I ${BASE_DIR}/export
INCLUDE_DIR += -I ${BASE_DIR}/export/common
INCLUDE_DIR += -I ${BASE_DIR}/source/include
INCLUDE_DIR += -I ${BASE_DIR}/source/bsp/dummy

all: example/main.o source/export/iboard.o source/export/iport.o
	g++ -ggdb3 ${INCLUDE_DIR} $^ -o $@

%.o: %.cpp
	g++ -ggdb3 -c ${INCLUDE_DIR} $^ -o $@

clean:
	rm -rf all main.o
	find . -name '*.o' -exec rm -f {} \;
