#!/bin/bash

# Cleanup all merged branches
for branch in `git branch -v  | grep gone | cut -d ' ' -f 1,2,3`; do git branch -D $branch;done;
git fetch --prune

# TODO: Make sure we can cleanup other branches.
