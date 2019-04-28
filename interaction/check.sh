#!/bin/sh -e

RESULT=$(echo "adam" | nc -q 1 "$1" "$2")
echo "$RESULT" | grep "More racing? Haven't you had enough?"
echo "$RESULT" | grep "Yes?"
echo "$RESULT" | grep "Whatever"
echo "$RESULT" | grep "Peace out."
