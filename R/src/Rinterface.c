#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <float.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>


// Morphy handler for R
// SEXP morphy_handler_R(SEXP matrix, SEXP ntaxa_R, SEXP nchar_R, SEXP list_anc, SEXP list_child, SEXP weight) {
SEXP morphy_handler_R(SEXP ntaxa_R, SEXP nchar_R, SEXP list_anc, SEXP list_child, SEXP weight) {

    // Input variables
    int ntaxa = asInteger(ntaxa_R);
    int nchar = asInteger(nchar_R);

    // Vectors from R
    // matrix = coerceVector(matrix, STRSXP);
    // PROTECT(matrix);
    list_anc = coerceVector(list_anc, INTSXP);
    PROTECT(list_anc);
    list_child = coerceVector(list_child, INTSXP);
    PROTECT(list_child);
    weight = coerceVector(weight, INTSXP);
    PROTECT(weight);

    // Variables from C
    int nedges = 2 * ntaxa - 2;
    int nnodes = ntaxa - 1;
    char* symbols = NULL;
    int i = 0;

    // Create a morphy handle

    // Do some stuff on the handle
    printf("ntaxa  = %i \n", ntaxa);
    printf("nchar  = %i \n", nchar);

    // printf("matrix  = ");
    // for(i = 0; i < ntaxa*nchar; ++i) {
    //     printf("%c", STRING(matrix)[i]) ;
    // }
    // printf("\n");

    printf("ancestors  = ");
    for(i = 0; i < nnodes+ntaxa; ++i) {
        printf("%i", INTEGER(list_anc)[i]) ;
    }
    printf("\n");

    printf("children  = ");
    for(i = 0; i < nnodes*2; ++i) {
        printf("%i", INTEGER(list_child)[i]) ;
    }
    printf("\n");

    printf("weights  = ");
    for(i = 0; i < nchar; ++i) {
        printf("%i", INTEGER(weight)[i]) ;
    }
    printf("\n");

    printf("nedges  = %i \n", nedges);
    printf("nnodes  = %i \n", nnodes);


    // Reconvert outputs


    // Some silly output
    SEXP output = PROTECT(allocVector(REALSXP, 1));

    UNPROTECT(5);

    // Return
    return output;
}