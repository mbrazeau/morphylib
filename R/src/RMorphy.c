#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <float.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>



// Morphy handler for R
//SEXP Morphy(SEXP r_n_char, SEXP r_n_taxa) {//, SEXP r_descendants, SEXP r_ancestors, SEXP r_rawmatrix) {
//  library('inline')
//  test <- cfunction(c(r_n_char = "integer", r_n_taxa = "integer", r_descendants = "integer",
//                      r_ancestors = "integer", r_rawmatrix = "character"), '
//    int n_char=asInteger(r_n_char), n_taxa=asInteger(r_n_taxa);  // asInteger converts length 1 R vector into scalar
//    int *descendants=INTEGER(r_descendants), *ancestors=INTEGER(r_ancestors);  // INTEGER gives pointer to first element of length n R vector
//    const char *rawmatrix=CHAR(asChar(r_rawmatrix));
//    Rprintf("Hello, world!\\n");
//    Rprintf("nchar = %i, ntaxa = %i \\n", n_char, n_taxa);
//    Rprintf("character string = %s\\n", rawmatrix);
//    return R_NilValue;
//  ')
//}

// Morphy handler for R
SEXP Morphy(SEXP r_n_char, SEXP r_n_taxa, SEXP r_descendants, SEXP r_ancestors, SEXP r_rawmatrix) {
  
  // Convert arguments passed from R into C variables
  
  int n_char=asInteger(r_n_char), n_taxa=asInteger(r_n_taxa);  // asInteger converts length 1 R vector into scalar
  int *descendants=INTEGER(r_descendants), *ancestors=INTEGER(r_ancestors);  // INTEGER gives pointer to first element of length n R vector
  const char *rawmatrix=CHAR(asChar(r_rawmatrix));
 
  Rprintf("Hello, world!\n");
  Rprintf("nchar = %i, ntaxa = %i \n", n_char, n_taxa);
  Rprintf("character string = %s\n", rawmatrix);
 
//  double *pvtmp;
//  int i;
//  
//  // Reserve memory for return parameters
//  SEXP RESULT, pars, pscore, DAT, pvec, APPL;
//  PROTECT(RESULT = allocVector(VECSXP, 5));
//  PROTECT(pars = allocVector(INTSXP, *n_char));
//  PROTECT(pscore = allocVector(REALSXP, 1));
//  PROTECT(DAT = allocMatrix(INTSXP, n_char[0], m));
//  PROTECT(pvec = allocVector(REALSXP, m));
//  PROTECT(APPL = allocMatrix(INTSXP, n_char[0], m));
//  pvtmp = REAL(pvec);
//  
//  // Initialize arrays
//  for(i=0; i<*n_char; i++) {
//    INTEGER(pars)[i] = 0;
//  }
//  REAL(pscore)[0]=0.0;
//  data = INTEGER(DAT);
//  for(i=0; i<(*n_char * n); i++) {
//    data[i] = INTEGER(dat)[i];
//    appl[i] = ((data[i] & ~*inappl) ? 1 : 0) - ((data[i] & *inappl) ? 1 : 0);
//  }
//  
//  // Calculate relevant properties of the tree and dataset
//  int n_internal = n_tip - 2;
//  int max_node = n_tip + n_internal;
//  
//  
//  // Run relevant functions from Morphy Library
//  morphy <- mpl_new_Morphy(); 
//  mpl_init_Morphy(*n_taxa, *n_char, morphy);
//  mpl_attach_rawmatrix(rawmatrix, morphy);
//  
//  for (i = 0; i < n_char; i++) {
//    mpl_set_parsim_t(i, FITCH_T, morphy);
//  }
//  mpl_apply_tipdata(morphinstance);
// 
//  for (i = max_node - 1; i > n_tip; i--) { // First Downpass 
//    mpl_NA_fitch_first_downpass(i, descendants[i * 2], descendants[(i * 2) + 1], ancestors[i])
//  }
// 
//  for (i = n_tip; i < max_node; i++) { // First uppass: internal nodes
//    ask_mpl_to_resolve_applic_up(i, descendants[i * 2], descendants[(i * 2) + 1], ancestors[i])
//  }
// 
//  for (tip in tips) { # First uppass: update tips
//    ask_mpl_to_update_tip(tip, ancestors[tip])
//  }
// 
//  ask_mpl_for_prelim_recon();
// 
//  for (node in postorder) {
//    ask_mpl_to_resolve_applic_up(node, descendants[node, 1], descendants[node, 2], ancestors[node])
//  }
// 
//  for (tip in tips) { 
//    ask_mpl_to_finalize_tip(tip, ancestors[tip]) 
//  } # Second uppass: finalize tips.  Redundant?
// 
//  ask_mpl_for_final_recon();
//  ask_mpl_to_store_final_active_states();
//  
//  
//  // store variables in an format that can be returned to R
//  SET_VECTOR_ELT(RESULT, 0, pscore);
//  SET_VECTOR_ELT(RESULT, 1, pars);
//  SET_VECTOR_ELT(RESULT, 2, DAT);
//  SET_VECTOR_ELT(RESULT, 3, pvec);
//  SET_VECTOR_ELT(RESULT, 4, APPL);
// 
//  // Unprotect allocated memory
//  UNPROTECT(6);
// 
//  // And return.
//  return(RESULT);
  return R_NilValue;
}