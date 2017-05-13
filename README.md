# MorphyLib     [![Build Status](https://travis-ci.org/mbrazeau/MorphyLib.svg?branch=master)](https://travis-ci.org/mbrazeau/MorphyLib)
Phylogenetic reconstruction library using inapplicable data reconstruction

**This library will supersede any older versions of MorPhy, Morphy, or any other project I've written with that name**

**Building Morphy**

So far, building is supported for Unix-like systems (e.g. Linux, Mac OS). A makefile in the root MorphyLib directory builds both static and dynamic libraries. 

Additionally, a Python file is included and maintained which supplies all the bindings for that language. 

**Using MorphyLib**
The API is documented in the `mpl.h` file. 

The caller creates an instance of the Morphy object through the `mpl_new_Morphy` function. No direct access to the members of this structure are required, and all interaction is through functions declared in `mpl.h`.
