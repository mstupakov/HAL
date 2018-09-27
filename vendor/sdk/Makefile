BASE_DIR := ${CURDIR}
TARG_DIR := build-$(notdir ${CROSS_COMPILE})
TEMP_DIR := ${BASE_DIR}/temp/$(TARG_DIR:-=)
PACK_DIR := ${BASE_DIR}/packages

${TEMP_DIR}/sdk: ${TEMP_DIR}/sdk_untar
	${BASE_DIR}/build.sh build

${TEMP_DIR}/sdk_untar:
	mkdir -p ${TEMP_DIR}
	cp -rf ${PACK_DIR}/* ${TEMP_DIR}/
	touch $@

clean:
	@[ -d ${TEMP_DIR} ] && ${BASE_DIR}/build.sh clean ||:

cleanall: clean
	-rm -rf ${BASE_DIR}/temp

.PHONY: sdk clean cleanall
