
# bwt
A naive implementation of the Burrows-Wheeler Transform block compression algorithm.

### Building
```bash
gcc bwt.c -o bwt
```

### Notes
This code is slightly broken. The tuples in the decoding step need to be sorted using a *stable* sorting algorithm, but the `qsort` standard library sorting function isn't stable.
