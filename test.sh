#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./90cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5; "
assert 47 "5 + 6 * 7;"
assert 15 "5 * (9 - 6);"
assert 4  "(3 + 5) / 2;"
assert 10 "-10 + 20;"
assert 20 "- -20;"
assert 30 "- - +30;"

assert 1  "0 == 0;"
assert 0  "0 == 1;"
assert 1  "0 != 1;"
assert 0  "0 != 0;"

assert 0  "1 <= 0;"
assert 1  "0 <= 0;"
assert 1  "0 <= 1;"

assert 0  "0 < 0;"
assert 1  "0 < 1;"

assert 0  "0 >= 1;"
assert 1  "0 >= 0;"
assert 1  "1 >= 0;"

assert 0  "0 > 0;"
assert 1  "1 > 0;"
assert 90 "a = 9; b = 10; a * b;"
assert 90 "a = 9; b = 10; a = a * b; a;"
assert 90 "A = 9; B = 10; A * B;"

assert 6 "one = 1; two = 2; three = 3; one + two + three;"
assert 6 "one = 1; two = 2; three = 3; one * two * three;"
assert 6 "one_two = 12; one_two / 2;"
assert 9 "return 9;"
assert 3 "return 3; return 6;"
assert 14  "a = 3; b = (5 * 6) - 8; return a + (b / 2);"

# TODO: incomplete, fix it later.
assert 9 "if (0) return 6; return 9;"
assert 6 "if (1) return 6; return 9;"
assert 19 "a = 9; b = 10; if (a == b) return a * b; return a + b;"
assert 90 "a = 9; b = 10; if (a != b) return a * b; return a + b;"

echo OK
