#!/bin/bash

export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export AR=${CROSS_COMPILE}ar

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
