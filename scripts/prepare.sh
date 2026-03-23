#!/bin/bash

ORIGINAL_DIR=$(pwd)

SCRIPT_PATH=$(realpath "${BASH_SOURCE[0]}")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")
cd "$SCRIPT_DIR"

export ROOT_DIR=$(git rev-parse --show-toplevel)

case ":$PATH:" in
    *:"$SCRIPT_DIR":*) ;;
    *) export PATH="$SCRIPT_DIR:$PATH" ;;
esac

alias fb="$SCRIPT_DIR/fast-build.sh"
alias gr='cd $ROOT_DIR'

cd "$ORIGINAL_DIR"