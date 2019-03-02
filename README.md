# libsodium File Encryption

In this repo are three implementations for encrypting a moderately large
(50MiB) file using `libsodium` and the Salsa20 cipher.

A 64KiB `test_data` file is located in the repo root, replace this with
your own test file if you want.

All benchmark results were obtained from a machine with a Kaby Lake i7-7700K
at 4.20GHz, and 32GiB of RAM running at 2400MHz, and using a 50MiB file
generated from `/dev/urandom`

## C

The [reference implementation](c/native.c) loads the `test_data` file into
a 64 MiB buffer, encrypts it using the default `libsodium` parameters, then
decrypts the buffer again and compares the result.

### Results

Compiled with `-O2` and run on a 50MiB file from `/dev/urandom`

```
Massif Profile:

peak cost: "164.0 MiB" heap "11.8 KiB" heap extra "0 B" stacks


Perf run:

Performance counter stats for './native':

            126.86 msec task-clock:u              #    0.998 CPUs utilized
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
            25,676      page-faults:u             # 203777.778 M/sec
       308,436,162      cycles:u                  # 2447906.048 GHz
       918,622,395      instructions:u            #    2.98  insn per cycle
         4,826,082      branches:u                # 38302238.095 M/sec
             5,821      branch-misses:u           #    0.12% of all branches

       0.127135287 seconds time elapsed

       0.076854000 seconds user
       0.050093000 seconds sys
```

164.0MiB of heap space was allocated, which matches exactly the three calls to malloc
in the implementation [ 1x 64MiB, 2x FileSize (50MiB) ]


## Python

Two implementations are tested, the [simple example](pynacl/encrypt_decrypt.py) encrypts
and decrypts just the bytes read from `test_data`.

A [more realistic version](pynacl/b64_mode.py) converts the test data first by
encoding it as base64, then by converting the encrypted ciphertext to a UTF-8
string and back again before performing the decryption.

### Results

**Simple Byte Encryption / Decryption**

```
Massif Profile:

peak cost: "351.0 MiB" heap "74.5 KiB" heap extra "0 B" stacks


Perf run:

Performance counter stats for 'python encrypt_decrypt.py':

            286.29 msec task-clock:u              #    0.996 CPUs utilized
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
           116,319      page-faults:u             # 406709.790 M/sec
       591,738,575      cycles:u                  # 2069015.997 GHz
       918,744,913      instructions:u            #    1.55  insn per cycle
        26,569,884      branches:u                # 92901692.308 M/sec
           766,737      branch-misses:u           #    2.89% of all branches

       0.287306264 seconds time elapsed

       0.159652000 seconds user
       0.126394000 seconds sys
```


**Base64 / String Version**

```
Massif Profile:

peak cost: "673.2 MiB" heap "33.4 MiB" heap extra "0 B" stacks


Perf run:

Performance counter stats for 'python b64_mode.py':

            986.83 msec task-clock:u              #    1.000 CPUs utilized
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
           269,916      page-faults:u             # 273748.479 M/sec
     2,756,648,569      cycles:u                  # 2795789.624 GHz
     8,843,524,463      instructions:u            #    3.21  insn per cycle
     1,602,604,518      branches:u                # 1625359551.724 M/sec
           796,823      branch-misses:u           #    0.05% of all branches

       0.987324762 seconds time elapsed

       0.706483000 seconds user
       0.279910000 seconds sys
```

Using `massif-visualizer` we can see that the Python implementations both need to
allocate 2x FileSize (50MiB) buffers to pass the plaintext and ciphertext to
and from `libsodium`, so in these examples a minimum of 100MiB of heap space
is expected.

Taking this into account, it appears that converting to and from base64 / UTF-8
strings results in a doubling of interpreter's memory allocations, and so these
should probably be avoided wherever possible.
