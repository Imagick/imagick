#!/bin/sh

set -x

find /usr -name policy.xml

DIRECTORY="${HOME}/im"

find ${DIRECTORY} -name policy.xml