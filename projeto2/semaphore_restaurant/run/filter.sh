#!/bin/bash

ngroups=$( head -2 config.txt | tail -1 )

./probSemSharedMemRestaurant | awk -f filter_log.awk -v ngroups=$ngroups

