#!/bin/bash

pushd $(dirname "$0")

./vendor/premake5 --file=Build.lua gmake2

# Go back to the original directory
popd
