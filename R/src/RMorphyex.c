#include <R.h>
#include <Rinternals.h>
#include "../../src/mpl.h"

SEXP _R_wrap_mpl_new_Morphy(void)
{
	Morphy new = mpl_new_Morphy();
	SEXP result = R_MakeExternalPtr(new, R_NilValue, R_NilValue);

	return result;
}

SEXP _R_wrap_mpl_init_Morphy(SEXP Rntax, SEXP Rnchar, SEXP MorphHandl)
{
	int ret = 0;
	SEXP Rret = PROTECT(allocVector(INTSXP, 1));

	Morphy handl = R_ExternalPtrAddr(MorphHandl);
	int ntax = INTEGER(Rntax)[0];
	int nchar = INTEGER(Rnchar)[0];

	ret = mpl_init_Morphy(ntax, nchar, handl);

	INTEGER(Rret)[0] = ret;
	UNPROTECT(1);
	return Rret;
}
	
SEXP _R_wrap_mpl_get_numtaxa(SEXP MorphHandl)
{
	SEXP Rret = PROTECT(allocVector(INTSXP, 1));

	INTEGER(Rret)[0] = mpl_get_numtaxa(R_ExternalPtrAddr(MorphHandl));

	UNPROTECT(1);

	return Rret;
}

SEXP _R_wrap_mpl_get_num_charac(SEXP MorphHandl)
{
	SEXP Rret = PROTECT(allocVector(INTSXP, 1));
	
	INTEGER(Rret)[0] = mpl_get_num_charac(R_ExternalPtrAddr(MorphHandl));

	UNPROTECT(1);

	return Rret;
}

SEXP _R_wrap_mpl_attach_symbols(SEXP Rsymbols, SEXP MorphyHandl)
{
	SEXP Rret = PROTECT(allocVector(INTSXP, 1));

	UNPROTECT(1);

	int Mret = 0;
	const char *Msymbols = CHAR(asChar(Rsymbols));

	Mret = mpl_attach_symbols(Msymbols, R_ExternalPtrAddr(MorphyHandl));
	INTEGER(Rret)[0] = Mret;

	UNPROTECT(1);

	return Rret;
}

SEXP _R_wrap_mpl_get_symbols(SEXP MorphyHandl)
{
	SEXP Rret = PROTECT(allocVector(STRSXP, 1));

	char* symbols = mpl_get_symbols(R_ExternalPtrAddr(MorphyHandl));

	Rret = mkString(symbols);
	UNPROTECT(1);

	return Rret;
}

SEXP _R_wrap_mpl_attach_rawdata(SEXP Rmatrix, SEXP MorphyHandl)
{
	SEXP Rret = PROTECT(allocVector(INTSXP, 1));

	int Mret = 0;
	const char *Mmatrix = CHAR(asChar(Rmatrix));

	Mret = mpl_attach_rawdata(Mmatrix, R_ExternalPtrAddr(MorphyHandl));
	INTEGER(Rret)[0] = Mret;

	UNPROTECT(1);

	return Rret;
}
