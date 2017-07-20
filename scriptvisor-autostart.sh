#!/bin/bash

set -e
killall scriptvisor || true
mkdir -p ~/.config/scriptvisor
PATH="$PATH:$PWD/bin" exec scriptvisor -autostart -config ~/.config/scriptvisor/scriptvisor.json -logdir ~/.config/scriptvisor/scriptvisor-logs
