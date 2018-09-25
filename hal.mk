EXPORT_DIR := -I ${HAL}/export
EXPORT_DIR += -I ${HAL}/export/common

LIB := ${HAL}/libhal.a
LD_FLAGS := ${LIB} -pthread
