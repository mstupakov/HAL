ifeq (,$(CROSS_COMPILE))
  export CROSS_COMPILE :=
endif

BASE_DIR := $(CURDIR)

INCLUDE_DIR := -I ${BASE_DIR}/export
INCLUDE_DIR += -I ${BASE_DIR}/export/common
INCLUDE_DIR += -I ${BASE_DIR}/source/include
INCLUDE_DIR += -I ${BASE_DIR}/source/bsp/dummy
INCLUDE_DIR += -I ${BASE_DIR}/source/switch/stub

CXX_FLAGS := -O0 -ggdb3 -std=c++1z

SOURCE_CXX := $(wildcard ${BASE_DIR}/source/export/*.cpp)
SOURCE_CXX += $(wildcard ${BASE_DIR}/source/export/common/*.cpp)

SOURCE_OBJ := $(patsubst %.cpp,%.o,${SOURCE_CXX})

all: ${BASE_DIR}/example/main.o ${BASE_DIR}/libhal.a
	${CROSSS_COMPILE}g++ ${CXX_FLAGS} ${INCLUDE_DIR} $^ -o all

${BASE_DIR}/libhal.a: ${SOURCE_OBJ}
	${CROSS_COMPILE}ar crs $@ $^

%.o: %.cpp
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${INCLUDE_DIR} $^ -o $@

clean:
	rm -rf all main.o libhal.a
	find . -name '*.o' -exec rm -f {} \;
