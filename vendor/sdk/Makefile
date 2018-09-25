BASE_DIR := ${CURDIR}
TEMP_DIR := ${BASE_DIR}/temp
PACK_DIR := ${BASE_DIR}/packages

sdk: sdk_untar
	${BASE_DIR}/build.sh build

sdk_untar:
	mkdir -p ${TEMP_DIR}
	cp -rf ${PACK_DIR}/* ${TEMP_DIR}/
	touch sdk_untar

clean:
	@[ -d ${TEMP_DIR} ] && ${BASE_DIR}/build.sh clean ||:

cleanall: clean
	-rm -rf ${TEMP_DIR} ${BASE_DIR}/sdk_untar

.PHONY: sdk clean cleanall
