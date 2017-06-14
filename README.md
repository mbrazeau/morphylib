# MorphyLib     [![Build Status](https://travis-ci.org/mbrazeau/MorphyLib.svg?branch=master)](https://travis-ci.org/mbrazeau/MorphyLib)
Phylogenetic reconstruction library using inapplicable data reconstruction

**About MorphyLib right now**

MorphyLib is a work in progress. It is currently only in development and, while it can now be called with real-world data, it is untested in its current form and not ready for general implementation.

MorphyLib is a library for phylogenetic analysis programming and is therefore likely to be of use or interest only to those who are writing phylogenetic software. However, if you are interested in MorphyLib's functionality at a user level, stay tuned as we will most certainly be making this possible through a 'front-end' application. 

A forthcoming paper by myself and co-authors will describe the algorithms used by MorphyLib. 

**Building Morphy**

So far, building is supported for Unix-like systems (e.g. Linux, Mac OS). A makefile in the root MorphyLib directory builds both static and dynamic libraries. 

Additionally, Python and R bindings are being supplied and maintained, but may not be entirely up to date with the core library. 

**Using MorphyLib**

The API is [documented](http://htmlpreview.github.io/?https://github.com/mbrazeau/MorphyLib/blob/master/Documentation/html/mpl_8h.html) in the [`mpl.h`](https://github.com/mbrazeau/MorphyLib/blob/master/src/mpl.h) file. 

The caller creates an instance of the Morphy object through the `mpl_new_Morphy` function. No direct access to the members of this structure are required, and all interaction is through functions declared in `mpl.h`.

MorphyLib has its own rules for 'packing' state data as set bits in integers and therefore you are required to supply a data matrix and dimensions to it in order to perform any calculations.


*Basic algorithm and application*

A currently in-prep paper in collaboration with Thomas Guillerme and Martin R. Smith describes the algorithm for reconstructing exact ancestral state sets when inapplicable data are in play. The general principle is that inapplicable data imply an underlying presence/absence character. Because of this, MorphyLib uses a bitmask to reconstruct ancestral state sets as partially resolved on the downpass when there is an inapplicable token involved. The second uppass will finalise any state sets not affected by inapplicable values, but will also make a final estimation of whether or not the reconstruction will be inapplicable or applicable. Unfortunately, because exact reconstruction of the applicable states requires exact reconstruction of an implicit binary character, two further passes are required for final ancestral state calculations

Thus, for complete ancestral states reconstructions, 

*Setting up a MorphyLib session*

A C or C++ program using MorphyLib needs to include the `mpl.h` header file.

- Creating an instance of the MorphyLib handle:
```C
#include "mpl.h"

...

Morphy mplobj = mpl_new_Morphy();
```

- Setting the dimensions of the data set:
  - Set the initial dimensions:
  ```C
  mpl_init_Morphy(ntax, nchar, mplobj);
  ```
  - Num extra internal nodes needed: Different calling programs may have different ways of structuring trees, either as lists or linked data structures. Therefore, it is up to the calling program to decide how many additional nodes are required.
  
  ```C
  mpl_set_num_internal_nodes(num_nodes, mplobj);
  ```
  
  Note, however, that MorphyLib functions currently only support calculations on completely binary trees. Therefore, you should have at least as many internal nodes as are required to for the number of terminals in your dataset. It is also recommended that you use a 'dummy root' ancestral to the tree's main calculation root and which has its own ancestral state set. It is probably not necessary, but given MorphyLib's current state of development and internal architecture, this is probably safest until now.
  
- Supplying the input data
  - Row-ordered format
  - PAUP (Nexus) style currently supported; TNT/xread format forthcoming.
  
 - Setting weights and parsimony type
  - Real-number weights will be estimated as nearest rational approximations
  
 - Setting inapplicable treatment: By default, MorphyLib treats the standard gap symbol '-' as inapplicable. However, the calling program can set the treatment of the gap symbol as either missing data or as a new state, to emulate the behaviour of other programs.
  
 - Applying the data: Once the caller is satisfied with the settings, these parameters need to be applied to the dataset and will remain enforced until they are later changed. **Any changes must be applied again** otherwise, unexpected behaviour or a crash will occur.
  
 *What happens inside MorphyLib?*: When the parameters are applied, the data are partitioned according to type of parsimony and then whether or not there is a significant number of inapplicable tokens (i.e. 3 or more per column).

*Calculating ancestral states and obtaining a tree length*

If treating the gap symbol as inapplicable, then four whole passes are required on an initial tree in order to get the full ancestral state set calculations and lengths for the tree.

- First downpass
- Update the root values
- First uppass
- Update the tips
- Second downpass
- Second uppass

*Getting ancestral state sets*

**Future development**

This workflow is sufficient for getting the length of a given tree and for outputting the ancestral state sets when inapplicable data are treated as such. However, applying this to a tree search will be extremely slow. Most programs doing heuristic tree searches do not optimize over the whole tree, but use 'shortcuts' that allow length calculations by optimising only at proposed insertion sites. Support for these procedures will soon be added. However, inapplicable data add complications to the process that can't always be foreseen. Thus, partial re-optimisation of subtrees may sometimes be required for accurate length counts on characters with inapplicable scores.
