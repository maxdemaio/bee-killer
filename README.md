# Bee Killer

solving the NYT spelling bee

---

to start, we generate a binary structure that maps each dictionary word to a 32-bit binary representation of itself:

```
tac (example word from dict)
19 0 2
00000010000000000000000000000101
```

with the given letters and the required letter we create a solution cypher:

```
cats (letters given, c is the required letter)
2 0 18 19 (n-th bit positions starting from right to left)
00000011000000000000000000000101

!cats
11111100111111111111111111111010
```

compare the solution cypher to each word in our binary structure we generated with a two step verification process:

- tac && !cats = 0
- then check if the position at the required bit is 1

usage:

in the `main()` function: uncomment the `makeWordBinaryMap()` function, comment out everything else in the `main()` function, and compile the binary structure for your platform:

```
gcc bee-killer.c
```

comment out the `makeBinaryMap()`, uncomment everything else in the `main()` function, and execute the project

```
./a.out cats c
```

note: once you compile the binary structure once, you can re-use it against any combination of words and required letter you'd like.
