#!/bin/bash
lsb_release -drc | awk '{print $2}' > "$1"
