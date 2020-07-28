#
#
#

all : # default rule

D_BUILD=${PWD}/build
D_BIN=${D_BUILD}/bin
D_SOURCES=${PWD}/sources
BUILD_MAKEFILE=${D_BUILD}/Makefile

${D_BUILD} ${D_BIN} : ; mkdir -p $@
${BUILD_MAKEFILE} : ${D_BUILD} ; cd ${D_BUILD} && cmake ${D_SOURCES}

clean.build : ${D_BUILD} ; rm -rf ${D_BUILD}/*

clean : clean.build
build : ${BUILD_MAKEFILE} ; cd ${D_BUILD} && make 
rebuild : clean all

all : build

test : ; ${D_BUILD}/test-units

.PSEUDO: all clean clean.build build rebuild
