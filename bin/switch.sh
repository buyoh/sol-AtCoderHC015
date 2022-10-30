#!/bin/bash

set -eu
cd `dirname $0`/

SELECTED_LANG=$1

if [[ x$SELECTED_LANG == x ]]; then
  echo 'switch language: [cpp] [cpp_profile]'
  exit 2
fi

if [[ ! -d $SELECTED_LANG ]]; then
  echo 'invalid keyword: $SELECTED_LANG'
  exit 1
fi

if [[ ! -f $SELECTED_LANG/run.sh ]]; then
  echo 'invalid directory: $SELECTED_LANG'
  exit 1
fi

rm -f ./run.sh
ln -s $SELECTED_LANG/run.sh ./run.sh
rm -f ./build.sh
ln -s $SELECTED_LANG/build.sh ./build.sh
