#!/bin/sh
# usage: sh tests/run.sh  (from repo root)
fail=0
out=$(./bf tests/hello.bf)
[ "$out" = "$(cat tests/hello.out)" ] || { echo "FAIL hello"; fail=1; }
out=$(echo "Hello" | ./bf tests/rot13.bf)
[ "$out" = "Uryyb" ] || { echo "FAIL rot13"; fail=1; }
[ $fail = 0 ] && echo "ok"
exit $fail
