#!/bin/sh

S_STAMP=$(date +%Y.%m.%d)
X_STAMP=$(date +%Y%m%d)

cat > version.h <<XXXX

#define VERSION_OF_MODULE "$S_STAMP"
#define VERSION_ID_MODULE 0x$X_STAMP

XXXX
