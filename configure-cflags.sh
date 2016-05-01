#!/bin/bash
#
# Set CFLAGS to be strict for supported versions on Travis

#containsElement () {
#  local e
#  for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
#  return 1
#}


# Bash return statements are not useful. You have to echo the value
# and then capture it by calling the function with $( foo )
function contains() {
    local n=$#
    local value=${!n}
    for ((i=1;i < $#;i++)) {
        if [ "${!i}" == "${value}" ]; then
            echo "1"
            return 0
        fi
    }
    echo "0"
    return 0
}



strictPHPVersions=()
strictPHPVersions+=("5.6")
strictPHPVersions+=("7")
strictPHPVersions+=("7.0")
strictPHPVersions+=("7.1")

strictImageMagickVersions=()
strictImageMagickVersions+=("dev")
strictImageMagickVersions+=("6.8.7-0")
strictImageMagickVersions+=("6.7.5-3")
strictImageMagickVersions+=("6.6.0-9")

echo "TRAVIS_PHP_VERSION is ${TRAVIS_PHP_VERSION}"
echo "IMAGEMAGICK_VERSION is ${IMAGEMAGICK_VERSION}"

strictImageMagick=$(contains "${strictImageMagickVersions[@]}" "${IMAGEMAGICK_VERSION}" )
strictPHP=$(contains "${strictPHPVersions[@]}" "${TRAVIS_PHP_VERSION}" )

echo "strictImageMagick is ${strictImageMagick}"
echo "strictPHP is ${strictPHP}"

if [[ $strictPHP = '1' ]] && [[ $strictImageMagick = '1' ]]; then 
	CFLAGS="-Wno-deprecated-declarations -Wdeclaration-after-statement -Werror -Wall -Wimplicit-function-declaration"; 
else 
	CFLAGS="-Wno-deprecated-declarations";
fi

echo "Setting CFLAGS to ${CFLAGS}"

export CFLAGS=$CFLAGS