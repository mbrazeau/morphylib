/*!
 @file mpl.h
 
 @brief Defines the Morphy Phylogenetic Library API: a library for phylogenetic
 computation accommodating morphological character hierarchies.
 
 Copyright (C) 2017  Martin D. Brazeau
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 @discussion This header includes all the externally exported definitions and
 function prototypes. A calling program creates an instance of a Morphy object 
 and interacts with its elements through the functions described in this 
 interface. The Morphy object contains no tree objects, but requires a 
 pre-specified list of indices (integers) corresponding to the node indices in 
 the calling program. Morphy will not keep track of the relationships between
 the nodes, and it is up to the caller to keep track of these. Each character
 *must* be assigned a type, and Morphy will make no default assumptions. Once 
 one or more characters are assigned a function type (which creates internal
 partitions), and a postorder list of nodes is known, then the library functions
 can be called to reconstruct state sets and deliver length estimates for each
 node.
 
 Morphy will provide functions for local reoptimisation, partial reoptimisation
 and optimisation of subtrees.
 
 */

#ifndef mpl_h
#define mpl_h


#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */
    
#include <stdbool.h>
#include "morphydefs.h"
#include "mplerror.h"
    
    
// Public functions

/*!
 
 @brief Creates a new instance of a Morphy object

 @discussion Creates a new empty Morphy object. All fields are unpopulated and
 uninitialised.
 
 @return A void pointer to the Morphy instance. NULL if unsuccessful.
 
 */
Morphy  mpl_new_Morphy
    
        (void);


/*!
 
 @brief Destroys an instance of a Morphy object.

 @discussion Destroys an instance of the Morphy object, calling all destructors
 for internal object completely returning the memory to the system.
 
 @param m A Morphy object to be destroyed.
 
 @return A Morphy error code.
 
 */
int     mpl_delete_Morphy
    
        (Morphy m);


/*!

 @brief Sets up the dimensions of the dataset.

 @discussion Provides initial dimensions for the dataset, which will constrain
 any input matrix supplied to Morphy.

 @param ntax The number of taxa (or tips/terminals).

 @param nchar The number of characters (i.e. transformation series) in the
 data set.
 
 @param m An instance of the Morphy object.

 @return Morphy error code.

 */
int     mpl_init_Morphy
    
        (const int  ntax,
         const int  nchar,
         Morphy     m);
    

/*!

 @brief Retrieve the number of taxa (rows) in the dataset.
 
 @discussion Retrieves the number of taxa (rows) in the dataset.

 @param m An instance of the Morphy object.

 @return The number of taxa if success, otherwise an error code.

 */
int     mpl_get_numtaxa
    
        (Morphy m);


/*!

 @brief Retrieve the number of taxa (rows) in the dataset.

 @discussion Retrieves the number of taxa (rows) in the dataset.

 @param m An instance of the Morphy object.

 @return The number of taxa if success, otherwise an error code.

 */
int     mpl_get_num_charac
    
        (Morphy     m);
    

/*!

 @brief Attach a caller-specified list of symbols.

 @discussion Allows the caller to specify a list of symbols in the data matrix,
 otherwise, the symbols list used by Morphy will be extracted from the matrix. 
 The symbols list must match the symbols provided in the matrix. When Morphy 
 extracts symbols from the matrix, their ordering is alphanumeric, according to
 their ASCII codes (i.e. "+0123...ABCD...abcd..."). Loading a user-specified
 symbols list will override this ordering. Symbols loaded in either the list or
 the matrix must be valid Morphy character state symbols as defined in the 
 statedata.h header file.

 @param symbols A C-style (i.e. NULL-terminated) string of valid state symbols.

 @param m An instance of the Morphy object.

 @return Morphy error code.

 */
int     mpl_attach_symbols
    
        (const char*    symbols,
         Morphy         m);
    

/*!
 
 @brief Retrieves the current list of symbols.
 
 @discussion Returns a pointer to the string of character state symbols
 currently being used by Morphy (i.e. either the list of symbols extracted
 from the matrix, or the caller-specified values).
 
 @param m An instance of the Morphy object.
 
 @return A C-style (null-terminated) string of the character state symbols being
 used. NULL if failure.
 
 */
char*   mpl_get_symbols

        (const Morphy   m);


/*!

 @brief Attach raw character state data (i.e. tip data).
 

 @discussion Attaches a raw data character state matrix in the form of a C-style
 (i.e. NULL-terminated string). This can be the matrix block extracted from a
 Nexus file or an xread table format. The matrix should contain no terminal or
 tip labels.

 @param rawmatrix C-style string corresponding to the tip data.

 @param m An instance of the Morphy object.

 @return Morphy error code.

 */
int     mpl_attach_rawdata

        (const char*    rawmatrix,
         Morphy         m);


/*!
 
 @brief Deletes the caller-input data
 
 @discussion Deletes all of the user-input data and restores all parameters to
 their original values, except for the dimensions of the matrix.
 
 @param m An instance of the Morphy object.
 
 @return Morphy error code.
*/
int     mpl_delete_rawdata

        (Morphy     m);

    
int     mpl_set_gap_symbol

        (const char gapsymb,
         Morphy     m);


int     mpl_set_missing_symbol
        (const char missymb,
         Morphy     m);


/*!
 
 @brief Commits parameters prior to nodal set calculations.

 @discussion Once the caller is satisfied with the setup of types, weights, and
 partitioning, this function must be called, thereby committing the parameters
 until any changes are made. If no character types have been assigned, the 
 function will fail with an error code.
 
 @param m An instance of the Morphy object.
 
 @return A Morphy error code.
*/
int     mpl_apply_tipdata
    
        (Morphy m);
    

int     mpl_set_postorder

        (const int  nodeID,
         const int  index,
         Morphy     m);


int     mpl_incl_charac

        (const int  charID,
         Morphy     m);


int     mpl_excl_charac

        (const int  charID,
         Morphy     m);


int     mpl_set_charac_weight

        (const int  charID,
         Mflt       weight);


int     mpl_set_parsim_t

        (const int charID,
         const MPLchtype parsfxn,
         Morphy           m);


int     mpl_set_gaphandl

        (const gap_t gaptype,
         Morphy      m);


int     mpl_query_gaphandl

        (Morphy     m);


int     mpl_resolve_applic_down

        (const int  nodeID,
         const int  lChild,
         const int  rChild,
         Morphy     m);


int     mpl_resolve_applic_up

        (const int  nodeID,
         const int  lChild,
         const int  rChild,
         const int  parentID,
         Morphy     m);

int     mpl_prelim_recon
    
        (const int  nodeID,
         const int  lChild,
         const int  rChild,
         Morphy     m);
    

int     mpl_final_recon

        (const int  nodeID,
         const int  lChild,
         const int  rChild,
         const int  parentID,
         Morphy     m);


int     mpl_update_tip

        (const int  nodeID,
         Morphy     m);


int     mpl_finalize_tip

        (const int  nodeID,
         Morphy     m);

    
int     mpl_get_insertcost

        (const int  srcID,
         const int  tgt1ID,
         const int  tgt2ID,
         bool       max,
         int        cutoff,
         Morphy     m);


int     mpl_get_packed_states

        (const int  nodeID,
         const int  character,
         int        passnum,
         Morphy     m);


int     mpl_get_stateset

        (char*      res,
         const int  nodeID,
         const int  character,
         int        passnum,
         Morphy     m);

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* mpl_h */
