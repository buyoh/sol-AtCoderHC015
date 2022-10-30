#!/bin/bash

set -eu
cd `dirname $0`/..

cargo build --bin main
