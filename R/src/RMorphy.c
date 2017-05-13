// Useful resource for R-C interface: http://adv-r.had.co.nz/C-interface.html
// Useful tool for debugging: function "cfunction" in R package "inline"

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <float.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>


// Morphy handler for R
// Simplest implementation (good place to start!): 
// Return the number of steps associated with each character 
// This can then be multiplied by a vector of character weights in R
// This will probably be quick enough to allow tree search to proceed in R, even without 
// subsequent optimisations (e.g. using 'normal' fitch optimization for characters with <2 inapp tokens)

SEXP Morphy(SEXP r_n_char, SEXP r_n_taxa, SEXP r_descendants, SEXP r_ancestors, SEXP r_rawmatrix) {
  
  // Convert arguments passed from R into C variables
  
  int n_char=asInteger(r_n_char), n_taxa=asInteger(r_n_taxa);  // asInteger converts length 1 R vector into scalar
  int *descendants=INTEGER(r_descendants), *ancestors=INTEGER(r_ancestors);  // INTEGER gives pointer to first element of length n R vector
  const char *rawmatrix=CHAR(asChar(r_rawmatrix));
 
  Rprintf("Hello, world! I read some data!\n");
  Rprintf(" - nchar = %i, ntaxa = %i \n", n_char, n_taxa);
  Rprintf(" - character string = %s\n", rawmatrix);
  
  // Calculate relevant properties of the tree and dataset
  int n_internal = n_taxa - 1;
  int max_node = n_taxa + n_internal;
  Rprintf(" - %i nodes, of which %i are internal, %i are tips, and one is the root\n", max_node, n_internal, n_taxa);
  
  // This is just to test that the ancestor identification is working as expected
  int root_node_left_child = n_taxa + 1;
  Rprintf(" - Root node's left child has node %i as its ancestor, and %i and %i as its left and right children\n",
    ancestors[root_node_left_child],
    descendants[(root_node_left_child - n_taxa) * 2],
    descendants[((root_node_left_child - n_taxa) * 2)+ 1]
  );
  return R_NilValue;
  
  
  // Run relevant functions from Morphy Library
  
  // Some of said functions are as yet unwritten, others I'm not quite clear how to call (or in what order)
  // I'm sure that MDB will be able to flesh this out very quickly.
  /*
  morphy = mpl_new_Morphy(); 
  mpl_init_Morphy(&n_taxa, &n_char, morphy);
  mpl_attach_rawmatrix(rawmatrix, morphy);
  
  for (i = 0; i < *n_char; i++) {
    mpl_set_parsim_t(i, FITCH_T, morphy);
  }
  mpl_apply_tipdata(morphinstance);
 
  for (i = max_node - 1; i > n_taxa; i--) { // First Downpass 
    mpl_NA_fitch_first_downpass(&i, descendants + (i - n_taxa * 2), descendants + ((i * 2) + 1), ancestors + i)
  }
 
  for (i = n_taxa; i < max_node; i++) { // First uppass: internal nodes
    mpl_NA_fitch_first_uppass(&i, descendants + (i * 2), descendants((i * 2) + 1), ancestors + i)
  }
 
  for (i = 0; i < n_taxa; i++) { // First uppass: update tips
    mpl_update_tip(&i, ancestors + i)
  }
 
  mpl_prelim_recon(morphy);
  
  for (i = max_node - 1; i > n_taxa; i--) { // Second Downpass 
    mpl_NA_fitch_second_downpass(&i, descendants + (i * 2), descendants + ((i * 2) + 1), ancestors + i)
  }
 
  for (i = n_taxa; i < max_node; i++) { // Second uppass: internal nodes
    mpl_NA_fitch_first_uppass(&i, descendants + (i * 2), descendants((i * 2) + 1), ancestors + i)
  }
  
 for (i = 0; i < n_taxa; i++) { // Second uppass: update tips.  (This is probably redundant.)
    mpl_finalize_tip(&i, ancestors + i) 
  }
  
  mpl_final_recon();
  mpl_store_final_active_states();
  
  
  // store variables in an format that can be returned to R
  SET_VECTOR_ELT(RESULT, 0, pscore);
 
  // Unprotect allocated memory
 UNPROTECT(0); // TODO update to correct value
 
  // And return.
  return(RESULT);
  */
  return R_NilValue;
}