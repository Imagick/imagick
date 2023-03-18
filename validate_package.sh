#!/bin/bash

VALIDATE_OUTPUT=`pear package-validate`

echo "VALIDATE_OUTPUT is $VALIDATE_OUTPUT"

if [[ "$VALIDATE_OUTPUT" == *"Error"* ]]; then
  echo "Package appears to contain an error"
  exit 255
fi

echo "Package appears valid"