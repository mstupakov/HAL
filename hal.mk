EXPORT_DIR := -I ${HAL}/export
EXPORT_DIR += -I ${HAL}/export/common

TARGET_DIR := build-$(notdir ${CROSS_COMPILE})
TARGET_DIR := ${TARGET_DIR:-=}

BUILD_DIR := ${HAL}/output/$(TARGET_DIR)

LIB := ${BUILD_DIR}/libhal.a
LD_FLAGS := ${LIB} -pthread
