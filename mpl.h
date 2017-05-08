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
 the nodes, and it is up to the caller to keep track of these. 
 
 Morphy will provide functions for local reoptimisation, partial reoptimisation
 and optimisation of subtrees.
 
 */

#ifndef mpl_h
#define mpl_h


#ifdef __CPLUSPLUS
extern "C" {
#endif /*__CPLUSPLUS */

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
 @param m An Morphy object to be destroyed.
 @return A Morphy error code.
 */
int mpl_delete_Morphy
    (Morphy m);

/*!
@brief Sets up the dimensions of the dataset.
@discussion Provides initial dimensions for the dataset, which will constrain 
 any input matrix supplied to Morphy.
@param ntax The number of taxa (or tips/terminals).
@param nchar The number of characters (i.e. transformation series) in the 
 data set.
@return Morphy error code.
*/
int mpl_init_Morphy
    (const int  ntax,
     const int  nchar,
     Morphy     m);
    
    
int     mpl_get_numtaxa         (Morphy m);
int     mpl_get_num_charac      (Morphy m);
int     mpl_attach_symbols      (const char* symbols, Morphy m);
char*   mpl_get_symbols         (const Morphy m);
int     mpl_attach_rawdata      (const char* rawmatrix, Morphy m);
int     mpl_delete_rawdata      (Morphy m);
int     mpl_set_gap_symbol      (const char gapsymb, Morphy m);
int     mpl_set_missing_symbol  (const char missymb, Morphy m);

int     mpl_set_postorder       (const int nodeID, const int index, Morphy m);

int     mpl_incl_charac         (const int charID, Morphy m);
int     mpl_excl_charac         (const int charID, Morphy m);

int     mpl_set_charac_weight   (const int charID, Mflt weight);

int     mpl_set_parsim_t        (const int charID, const MPLptype_t parsfxn, Morphy m);
int     mpl_set_gaphandl        (const gap_t gaptype, Morphy m);
int     mpl_query_gaphandl      (Morphy m);

int     mpl_down_recon          (const int nodeID, const int lChild, const int rChild, Morphy m);
int     mpl_up_recon            (const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
int     mpl_up_final_recon      (const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
int     mpl_update_tip          (const int nodeID, Morphy m);
int     mpl_finalize_tip        (const int nodeID, Morphy m);

int     mpl_get_insertcost_max  (const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);
int     mpl_get_insertcost_min  (const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);

int     mpl_get_packed_states   (const int nodeID, const int character, int passnum, Morphy m);
int     mpl_get_stateset        (char* res, const int nodeID, const int character, int passnum, Morphy m);

#ifdef __CPLUSPLUS
}
#endif /*__CPLUSPLUS */

#endif /* mpl_h */
