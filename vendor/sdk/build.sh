#!/bin/bash

export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export AR=${CROSS_COMPILE}ar

TGT_DIR=build-

if [ ${CROSS_COMPILE}"x" != "x" ]; then
  TGT_DIR=${TGT_DIR}$(basename ${CROSS_COMPILE})
fi

TGT_DIR=${TGT_DIR:0:-1}

do_build() {
  echo "Build"
}

do_clean() {
  echo "Clean"
}

case "$1" in
  build)
        do_build
        ;;
  clean)
        do_clean
        ;;
esac
