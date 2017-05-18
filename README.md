# MorphyLib     [![Build Status](https://travis-ci.org/mbrazeau/MorphyLib.svg?branch=master)](https://travis-ci.org/mbrazeau/MorphyLib)
Phylogenetic reconstruction library using inapplicable data reconstruction

**About MorphyLib right now**

MorphyLib is a work in progress. It is currently only in development and, while it can now be called with real-world data, it is untested in its current form and not ready for general implementation.

MorphyLib is a library for phylogenetic analysis programming and is therefore likely to be of use or interest only to those who are writing phylogenetic software. However, if you are interested in MorphyLib's functionality at a user level, stay tuned as we will most certainly be making this possible through a 'front-end' application. 

A forthcoming paper by myself and co-authors will describe the algorithms used by MorphyLib. 

**Building Morphy**

So far, building is supported for Unix-like systems (e.g. Linux, Mac OS). A makefile in the root MorphyLib directory builds both static and dynamic libraries. 

Additionally, a Python file is included and maintained which supplies all the bindings for that language. 

**Using MorphyLib**

The API is documented in the [`mpl.h`](http://htmlpreview.github.io/?https://github.com/mbrazeau/MorphyLib/blob/master/Documentation/html/mpl_8h.html) file. 

The caller creates an instance of the Morphy object through the `mpl_new_Morphy` function. No direct access to the members of this structure are required, and all interaction is through functions declared in `mpl.h`.

MorphyLib has its own rules for 'packing' state data as set bits in integers and therefore you are required to supply a data matrix and dimensions to it in order to perform any calculations.
