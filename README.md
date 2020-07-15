Mitosis Workload Btree
===================================================

This repository contains the source for the workload Btree used in the
paper **Mitosis - Mitosis: Transparently Self-Replicating Page-Tables
for Large-Memory Machines** by Reto Achermann, Jayneel Gandhi,
Timothy Roscoe, Abhishek Bhattacharjee, and Ashish Panwar.

License
-------

See the LICENSE file

Authors
-------

Reto Achermann
Ashish Panwar


Dependencies
------------

```
sudo apt install libnuma-dev
```

Building
--------

Just type

```
make
```

to build all.

Running
-------

Running the benchmark (single threaded)

```
./bin/bench_btree_st
```

Running the benchmark (multi threaded)

```
./bin/bench_btree_mt
```

Config Parameters
```
 -o FILE --
```

Run parameters:
```
 -n <numelements>
 -l <numlookups>
 -o <order>
```