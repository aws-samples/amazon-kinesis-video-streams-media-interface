#!/bin/bash

BASEDIR=$(dirname $0)
PATCHFILES=$(ls ${BASEDIR}/*.patch)


if [ "$(git rev-parse --is-inside-work-tree 2>/dev/null)" ]; then
    git reset --hard
    for PATCH in ${PATCHFILES}; do
        git apply "${PATCH}"
    done
fi
