#!/bin/bash

pwd_temp=$(pwd)

SCRIPT_PATH=$(realpath "${BASH_SOURCE[0]}")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")
cd "$SCRIPT_DIR"

export ROOT_DIR=$(git rev-parse --show-toplevel)

export PATH="$SCRIPT_DIR:$PATH"

alias fb="$SCRIPT_DIR/fast-build.sh"
alias gr='cd $ROOT_DIR'

cd "$pwd_temp"