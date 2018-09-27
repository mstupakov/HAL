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

SOURCE_OBJ := $(patsubst ${BASE_DIR}/%.cpp,${BUILD_DIR}/%.o,${SOURCE_CXX})
SOURCE_DEP := $(patsubst ${BASE_DIR}/%.cpp,${BUILD_DIR}/%.d,${SOURCE_CXX})

all: ${BUILD_DIR}/test

${BUILD_DIR}/test: ${BUILD_DIR}/example/main.o ${LIB}
	${CROSS_COMPILE}g++ ${CXX_FLAGS} ${EXPORT_DIR} ${LD_FLAGS} $^ -o $@

${BUILD_DIR}/example/main.o: ${BASE_DIR}/example/main.cpp
	mkdir -p $(dir $@)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${EXPORT_DIR} $^ -o $@

${LIB}: ${SDK_LIBS} ${SOURCE_OBJ}
	${CROSS_COMPILE}ar crs $@ $^

${BUILD_DIR}/%.o: ${BASE_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${INCLUDE_DIR} ${SDK_INCLUDE_DIR} $< -o $@

${SDK_LIBS}:
	make -C ${SDK_DIR}

clean:
	-rm -rf ${BUILD_DIR} ${LIB} ${SOURCE_OBJ} ${SOURCE_DEP}

cleanall: clean
	-rm -rf ${BASE_DIR}/output
	make -C ${SDK_DIR} cleanall

-include $(SOURCE_DEP)
