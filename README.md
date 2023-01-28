# Bee Killer

solving the NYT spelling bee

---

with the given letters and the required letter we create a solution cypher:

```
cats (letters given, c is the required letter)
2 0 18 19 (n-th bit positions starting from right to left)
00000011000000000000000000000101

!cats
11111100111111111111111111111010
```

compare the solution cypher to each word in the dictionary:

```
tac (example word from dict)
19 0 2
00000010000000000000000000000101
```

two step verification:

- tac && !cats = 0
- then check if the position at the required bit is 1

usage:

compile binary for your platform

```
gcc bee-killer.c
```

execute the project

```
./a.out cats c
```

note: once you compile the binary structure once, you can re-use it against any combination of words and required letter you'd like.
