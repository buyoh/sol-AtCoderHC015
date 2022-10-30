#!/bin/bash

set -eu
cd `dirname $0`/..

out/main

# An interactive problem style for Atcoder heuristic contest
# cargo run --release --bin tester ../../out/main
