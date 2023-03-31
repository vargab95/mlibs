#!/bin/bash

set -e

script_dir=$(dirname $0)

mkdir -p $script_dir/coverage
cd $script_dir/coverage

cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_C_FLAGS_DEBUG="-g -fPIC -fprofile-arcs -ftest-coverage -Wall -Werror" \
    ..

make
make test

lcov \
    --directory ./ \
    --capture \
    --output-file ./code_coverage.info \
    -rc lcov_branch_coverage=1

genhtml \
    code_coverage.info \
    --branch-coverage \
    --output-directory \
    ./code_coverage_report/
