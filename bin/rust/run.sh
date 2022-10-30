#!/bin/bash

set -eu
cd `dirname $0`/..

target/debug/main

# An interactive problem style for Atcoder heuristic contest
# cargo run --release --bin tester ../../out/main
