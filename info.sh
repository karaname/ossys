#!/bin/bash
lsb_release -drc | cut -f2 > "$1"
