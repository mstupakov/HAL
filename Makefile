ifeq (,$(CROSS_COMPILE))
  export CROSS_COMPILE :=
endif

BASE_DIR   := $(CURDIR)
VENDOR_DIR := ${BASE_DIR}/vendor
SDK_DIR    := ${VENDOR_DIR}/sdk
SOL2_DIR   := ${VENDOR_DIR}/sol2
HAL        := ${BASE_DIR}

ifeq (,$(LUA))
  LUA := $(shell pkg-config --libs --cflags lua5.3 2> /dev/null || \
                 pkg-config --libs --cflags lua5.2 2> /dev/null || \
                 pkg-config --libs --cflags lua    2> /dev/null)
endif

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
cli: ${BUILD_DIR}/cli/cli
	@cp example/cli/*.lua ${BUILD_DIR}/cli/

${BUILD_DIR}/test: ${BUILD_DIR}/example/main.o ${LIB}
	${CROSS_COMPILE}g++ ${CXX_FLAGS} ${EXPORT_DIR} ${LD_FLAGS} $^ -o $@

${BUILD_DIR}/example/main.o: ${BASE_DIR}/example/main.cpp
	@mkdir -p $(@D)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${EXPORT_DIR} $< -o $@

${BUILD_DIR}/cli/cli: ${BUILD_DIR}/example/cli/main.o ${LIB}
	@mkdir -p $(@D)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} \
		${EXPORT_DIR} ${LD_FLAGS} $^ ${LUA} -lreadline -o $@

${BUILD_DIR}/example/cli/main.o: ${BASE_DIR}/example/cli/main.cpp ${SOL2_DIR}
ifeq "$(LUA)" ""
	$(error Lua 5.2 or higher has not been found!)
endif
	@mkdir -p $(@D)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -std=c++14 \
		-I ${SOL2_DIR} ${LUA} -c ${EXPORT_DIR} $< -o $@

${LIB}: ${SDK_LIBS} ${SOURCE_OBJ}
	${CROSS_COMPILE}ar crs $@ $^

${BUILD_DIR}/%.o: ${BASE_DIR}/%.cpp
	@mkdir -p $(@D)
	${CROSS_COMPILE}g++ ${CXX_FLAGS} -c ${INCLUDE_DIR} ${SDK_INCLUDE_DIR} $< -o $@

${SOL2_DIR}: .force
ifeq "$(wildcard ${SOL2_DIR}/*)" ""
	git submodule update --init --recursive
endif

${SDK_LIBS}:
	make -C ${SDK_DIR}

clean:
	@rm -rf ${BUILD_DIR} ${LIB} ${SOURCE_OBJ} ${SOURCE_DEP}

cleanall: clean
	@rm -rf ${BASE_DIR}/output
	make -C ${SDK_DIR} cleanall

.force:

-include $(SOURCE_DEP)
