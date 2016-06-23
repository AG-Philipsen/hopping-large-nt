# Effective 3D theory at large Nt

This is an application in C++ written as part of my PhD work to carry out the combinatorics of the 
effective 3D theory for lattice QCD first proposed in [1111.4953](https://arxiv.org/abs/1111.4953).
This is the software that was used to compute the 8th order diagrams first published in
[1512.05195](https://arxiv.org/abs/1512.05195). Eight is in no way a limit for the application, which can in
principle do any order.

## Prerequisites

The compiler requires C++11 support. Boost is also required and assumed to be in the default include-path.

## Use

Simply run `make` to build the application, the executable is by default placed in `obj/debug/main.out`.
One can change the build mode by setting the `BUILD_MODE` environment flag. At the moment the only
value other than `debug` which has any specific behavour is `release` which turns off debug flags and sets
`-O3` compiler optimisation.

Running the program is trivial and is simply

```obj/${BUILD_MODE}/main.out N```

where `N` is the order one wishes to compute. The program creates 3 files in a folder named `Configuration`
which are named `kappaN.terms`, `kappaN.debug` and `kappaN.json`. The files contain the following

### .terms
A simple list of all the terms contributing to the effective action in `W(n,m)` notation as defined in
[1512.05195](https://arxiv.org/abs/1512.05195), followed by a prefactor and the power of the number of
degenerate quark flavours.

### .debug
Similar to .terms however it is also shown which p and m configuration it originated from, which contraction
has been done as well as which spatial index assignment. The appendix of [1512.05195](https://arxiv.org/abs/1512.05195)
should be confronted for more detail.

### .json
Again the same information as in .terms but this time in a computer readable .json format. Json parsers and printers
along with the apropriate storage classes might be published on a separate occation. In this case a link will be posted
here. The json format is the following

```json
{
  "terms":
  [
    {
      "pref": "1\/12",
      "N_tr": "1",
      "factors":
      [
        {
          "n": "1",
          "m": "1",
          "pos": "{0,0,0,0}"
        },
        {}
      ]
    },
    {}
  ]
}
```

## Notes

The software was developed during my PhD studies under the supervision of Prof. Owe Philipsen at Goethe
University Frankfurt.

The makefile system in use is [nonrec-make](https://github.com/aostruszka/nonrec-make) by
[aostruszka](https://github.com/aostruszka).

## TODO

 - [ ] Implement a more configurable user interface with better error messages
 - [ ] Write documentation for the main program, not only the helper function library
 - [ ] If time permits it should be possible to extend the code to compute the full
       effective theory also for small N_t
