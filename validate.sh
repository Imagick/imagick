#!/bin/bash
#
# Simple shell script to validate releases
#

EXTNUM=$(grep PHP_IMAGICK_EXTNUM php_imagick.h | awk '{print $3'})
echo "php_imagick.h EXTNUM:          $EXTNUM"

REL_VERSION=$(xpath package.xml 'string(/package/version/release)' 2>/dev/null)
API_VERSION=$(xpath package.xml 'string(/package/version/api)' 2>/dev/null)

echo "package.xml release version:   ${REL_VERSION}"
echo "package.xml api version:       ${API_VERSION}"

REL_STABILITY=$(xpath package.xml 'string(/package/stability/release)' 2>/dev/null)
API_STABILITY=$(xpath package.xml 'string(/package/stability/api)' 2>/dev/null)

echo "package.xml release stability: ${REL_STABILITY}"
echo "package.xml api stability:     ${API_STABILITY}"

echo ""
echo "Validation results: "

if test "${REL_STABILITY}" = "beta"; then
	$(echo ${REL_VERSION} | egrep -i 'b|rc' >/dev/null 2>&1)

	if test $? != 0; then
		echo "Error: release is marked 'beta' but version doesn't include b or RC";
		exit 1;
	fi
fi

EXTNUM_FROM_PKG_XML=$(echo $REL_VERSION | gsed 's/^\([0-9]*\.[0-9]*\.[0-9]*\).*/\1/' | awk -F . '{ printf ("%d%02d%02d", $1, $2, $3); }')

if test $EXTNUM_FROM_PKG_XML != $EXTNUM; then
	echo "Error: package.xml and php_imagick.h EXTNUM mismatch"
	exit 1
fi



if test "${REL_STABILITY}" != "${API_STABILITY}"; then
	echo "Warning: API and release stability differ";
fi