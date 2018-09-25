ifeq (,$(CROSS_COMPILE))
  export CROSS_COMPILE :=
endif

BASE_DIR := $(CURDIR)
SDK_DIR  := ${BASE_DIR}/vendor/sdk
HAL      := ${BASE_DIR}

include hal.mk
include vendor/sdk/sdk.mk

INCLUDE_DIR := ${EXPORT_DIR}
INCLUDE_DIR += -I ${HAL}/source/include
INCLUDE_DIR += -I ${HAL}/source/bsp/dummy
INCLUDE_DIR += -I ${HAL}/source/switch/stub

CXX_FLAGS := -O0 -ggdb3 -std=c++1z -MMD

SOURCE_CXX := $(wildcard ${BASE_DIR}/source/export/*.cpp)
SOURCE_CXX += $(wildcard ${BASE_DIR}/source/export/common/*.cpp)

SOURCE_OBJ := $(patsubst %.cpp,%.o,${SOURCE_CXX})
SOURCE_DEP := $(SOURCE_OBJ:%.o=%.d)

all: ${BASE_DIR}/example/main.o ${LIB}
	${CROSS_COMPILE}g++ ${CXX_FLAGS} ${EXPORT_DIR} ${LD_FLAGS} $^ -o all

${LIB}: ${SDK_LIBS} ${SOURCE_OBJ}
	${CROSS_COMPILE}ar crs $@ $^

%.o: %.cpp
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${INCLUDE_DIR} ${SDK_INCLUDE_DIR} $< -o $@

${SDK_LIBS}:
	make -C ${SDK_DIR}

clean:
	-rm -rf all example/main.o example/main.d ${LIB} ${SOURCE_OBJ} ${SOURCE_DEP}

cleanall: clean
	make -C ${SDK_DIR} cleanall

-include $(SOURCE_DEP)
