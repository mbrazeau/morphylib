# MorphyLib     [![Build Status](https://travis-ci.org/mbrazeau/MorphyLib.svg?branch=master)](https://travis-ci.org/mbrazeau/MorphyLib) [![Coverage Status](https://coveralls.io/repos/github/mbrazeau/MorphyLib/badge.svg?branch=development)](https://coveralls.io/github/mbrazeau/MorphyLib?branch=development) ![](https://zenodo.org/badge/doi/10.5281/zenodo.8475.svg)
Phylogenetic reconstruction library using inapplicable data reconstruction

## About MorphyLib right now

MorphyLib is a work in progress. It is currently only in development and, while it can now be called with real-world data, it is untested in its current form and not ready for general implementation.

MorphyLib is a library for phylogenetic analysis programming and is therefore likely to be of use or interest only to those who are writing phylogenetic software. However, if you are interested in MorphyLib's functionality at a user level, stay tuned as we will most certainly be making this possible through a 'front-end' application. 

A forthcoming paper by myself and co-authors will describe the algorithms used by MorphyLib. 

## Building Morphy

For Linux and Mac, the Makefile is supplied that builds builds both static and dynamic libraries. For Windows, the Makefile.win file is supplied. Simply specify `nmake -f Makefile.win` and builds only the static library (at this time 27/06/2017).

Additionally, Python and R bindings are being supplied and maintained, but may not be entirely up to date with the core library. 

## Using MorphyLib

The API is [documented](http://htmlpreview.github.io/?https://github.com/mbrazeau/MorphyLib/blob/master/Documentation/html/mpl_8h.html) in the [`mpl.h`](https://github.com/mbrazeau/MorphyLib/blob/master/src/mpl.h) file. 

The caller creates an instance of the Morphy object through the `mpl_new_Morphy` function. No direct access to the members of this structure are required, and all interaction is through functions declared in `mpl.h`.

MorphyLib has its own rules for 'packing' state data as set bits in integers and therefore you are required to supply a data matrix and dimensions to it in order to perform any calculations.


### Basic algorithm and application

A currently in-prep paper in collaboration with Thomas Guillerme and Martin R. Smith describes the algorithm for reconstructing exact ancestral state sets when inapplicable data are in play. The general principle is that inapplicable data imply an underlying presence/absence character. Because of this, MorphyLib uses a bitmask to reconstruct ancestral state sets as partially resolved on the downpass when there is an inapplicable token involved. The second uppass will finalise any state sets not affected by inapplicable values, but will also make a final estimation of whether or not the reconstruction will be inapplicable or applicable. Unfortunately, because exact reconstruction of the applicable states requires exact reconstruction of an implicit binary character, two further passes are required for final ancestral state calculations

Thus, for complete ancestral states reconstructions, 

### Setting up a MorphyLib session

A C or C++ program using MorphyLib needs to include the `mpl.h` header file.

- Creating an instance of the MorphyLib handle:
```C
#include "mpl.h"

...

Morphy mplobj = mpl_new_Morphy();
```

### Setting the dimensions of the data set:

- Set the initial dimensions:
  ```C
  mpl_init_Morphy(ntax, nchar, mplobj);
  ```
  - Set the number of internal nodes needed: Different calling programs may have different ways of structuring trees, either as lists or linked data structures (the latter possibly using rings of records for internal nodes). Therefore, it is up to the calling program to decide how many additional nodes are required.
  
  ```C
  mpl_set_num_internal_nodes(num_nodes, mplobj);
  ```
  
  Note, however, that MorphyLib functions currently only support calculations on completely binary trees. Therefore, you should have at least as many internal nodes as are required to for the number of terminals in your dataset. It is also recommended that you use a 'dummy root' ancestral to the tree's main calculation root and which has its own ancestral state set. It is probably not necessary, but given MorphyLib's current state of development and internal architecture, this is probably safest until now.
  
### Supplying the input data

The input data should be supplied in a 'row-ordered' format and as a single C-style (null-terminated) string and with a terminal semicolon. Currently, 
 - The data are supplied to MorphyLib as follows:
 ```C
 char* matrix = "000...{01}11??---01;";
 
 ...
 
 mpl_attach_raw_data(matrix, mplobj);
 ```
 
 If the data set supplies a symbols list (i.e. from a Nexus file), then these can be supplied to MorphyLib in turn with
 
 ```C
 mpl_attach_symbols(symbols, mplobj);
 ```
 Again, as a null-terminated C-style string.
 
 This feature is not necessary, as MorphyLib will extract all of the symbols from the dataset. However, once ordered characters are implemented, the order of a symbols list will determine the ordering of characters, as in PAUP*.
  
### Setting the parsimony type

Currently, MorphyLib only supports parsimony. Currently, Fitch (unordered) parsimony is the only type fully implemented. Wagner (ordered) parsimony will soon become available. Different possible parsimony types are defined in a enumerated data type in `morphydefs.h`. 

```C
typedef enum {
    
    NONE_T          = 0,
    FITCH_T         = 1,
    WAGNER_T        = 2,
    DOLLO_T         = 3,
    IRREVERSIBLE_T  = 4,
    USERTYPE_T      = 5,
    
    MAX_CTYPE,
    
} MPLchtype;
```

To set the parsimony type of a particular character, the following function is used:
```C
int mpl_set_parsim_t(const int charID, const MPLchtype chtype, Morphy m);
```
The useage might be as follows:
```C
mpl_set_parsim_t(12, FITCH_T, mplobj);
```
At present, however, all characters are default type FITCH_T (unordered).

### Setting character weights

Weights are set in an analolgous fashion using the function
```C
int mpl_set_charac_weight(const int charID, const double weight, Morphy m);
 ```
MorphyLib will allow weights specified as a fractional (floating point) value, but will estimate the rational factors of this value to avoid the need for floating point arithmetic. All characters will be rescaled according to the least common multiple of the refactored weights. Thus, you may find strangely long tree lengths until I write a function to rescale them.
  
### Setting inapplicable treatment

By default, MorphyLib treats the standard gap symbol '-' as inapplicable. However, the calling program can set the treatment of the gap symbol as either missing data or as a new state, to emulate the behaviour of other programs.
  

### Applying the data

Once the caller is satisfied with the settings, these parameters need to be applied to the dataset and will remain enforced until they are later changed.

```C
mpl_apply_tipdata(mplobj);
```

**Any parameter changes must be applied by calling this function again** otherwise, unexpected behaviour or a crash will occur.
  
### *What happens inside MorphyLib?* 

When the parameters are applied, the data are partitioned according to type of parsimony and then whether or not there is a significant number of inapplicable tokens (i.e. 3 or more per column).

## Calculating ancestral states and obtaining a tree length

If treating the gap symbol as inapplicable, then four whole passes are required on an initial tree in order to get the full ancestral state set calculations and lengths for the tree.

MorphyLib stores a list of ancestral state sets for each node (internal and terminal) in the data set. It does not store any information

The examples below will assume a generic node structure like the following:
```C
struct node {
  struct node* anc, *left, *right; // Pointers to the neighboring nodes
  int index // Unique index identifier of the node
} node;
```
However, this would be specific to the calling program and is irrelevant to MorphyLib: other possibilities such as "Felsenstein-type" ring nodes, or edge lists could work as well. It is up to the calling program to keep track of the relationship between members and be able to uniquely identify them with an index value.

### First downpass:

Assuming `n` is a pointer to a node currently being visited by a downpass, the first downpass set at a node would be calculated as:

```C
mpl_first_down_recon(n->index, n->left->index, n->right->index, mplobj);
```

The return value of this function is the length added at that set, so if you need to store the length, the following pattern would be used:

```C
int length = 0;

...

length += mpl_first_down_recon(n->index, n->left->index, n->right->index, mplobj);
```

### Update the root values

MorphyLib assumes a calculation root is being used to calculate the length of the tree (thus trees must be at least implicitly rooted while counting). The root of the tree must be updated if treating inapplicable data as such. This resolves any ambiguity between applicables and the inapplicable state in favour of the applicable value(s) (this is is consistent with both the auxiliary principle and with minimizing length of the tree).

```C
int mpl_update_lower_root(const int l_root_id, const int root_id, Morphy m);
```

Assuming that `n` is currently pointing to the calculation root of the tree.

Two patterns of use are possible. The first assumes a dummy root is present (as is implemented in my own programs to facilitate uppasses without needing to check for a valid ancestor.

```C
mpl_update_lower_root(n->anc, n, mplobj);
```

Alternatively, if your tree structure does not require a dummy root ancestor:

```C
mpl_update_lower_root(n, n, mplobj);
```

Should work as well, but this has not been as extensively tested.

### First uppass

Final ancestral state sets for all-applicable characters and second-pass sets for characters with inapplicable values are calculated on the first uppass.

### Update the tips

Ambiguous or missing data at tips needs to be updated in order for correct ancestral state reconstructions to occur for characters with inapplicability. This is also necessary for accurate indirect optimisation ($ref:goloboff95), when this feature is supported by MorphyLib. Ambiguous or missing data at tips is updated according to the reconstructed value at the immediate ancestor.

The function
```C
int mpl_update_tip(const int tip_id, const int anc_id, Morphy m);
```
should be called for all tips in the tree.

### Second downpass
- Initial pass updating inapplicable/applicable ambiguities
- Counts steps associated with state changes

### Second uppass
- Finalises the ancestral state sets
- Adds length for the number of additional regions required (after $ref:delaet2014)

### Getting ancestral state sets

After complete four-pass optimisation has been completed, it is possible to retrieve state sets for any pass, in any character state, for any internal node or tip. These can be retrieved in either of two formats: integer-type data with bits set for each state in the set or as a character string of ASCII values representing the symbols in that set.

To retrieve the state sets in an integer:
```C
unsigned int mpl_get_packed_states(const int nodeID, const int character, const int passnum, Morphy m);
```

To retrieve a C-style (null-terminated) string:

```C
char* mpl_get_stateset(const int nodeID, const int character, const int passnum, Morphy m);
```

Note that the pass number is 1-based, not 0-based, because these values do not index into an array. Therefore, the `passnum` variable should be either 1, 2, 3, or 4.

Passes 3 and 4 should simply return null values for characters without significant inapplicable data.


## Future development

This workflow is sufficient for getting the length of a given tree and for outputting the ancestral state sets when inapplicable data are treated as such. However, applying this to a tree search will be extremely slow. Most programs doing heuristic tree searches do not optimize over the whole tree, but use 'shortcuts' that allow length calculations by optimising only at proposed insertion sites. Support for these procedures will soon be added. However, inapplicable data add complications to the process that can't always be foreseen. Thus, partial re-optimisation of subtrees may sometimes be required for accurate length counts on characters with inapplicable scores.

## Acknowledgments

This project has been and continues to be a long-standing passion project over a number of years, attempts, and re-developments (much of which can be explored on my GitHub page going back several years now). It would not have been possible to get this far in the process without the help of collaborators [Thomas Guillerme](https://github.com/TGuillerme) and [Martin R. Smith](https://github.com/ms609). Additionally, I acknowledge generous support from the European Research Council under the European Union’s Seventh Framework Programme (FP/2007–2013)/ERC Grant Agreement number 311092.

## References
All of this code depends on both published and currently in-prep work by myself and others. The details of the algorithm will be discussed in a forcoming series of papers.
