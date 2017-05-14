dyn.load("RMorphyex.so")

mpl_new_Morphy <- function() 
{
	return(.Call("_R_wrap_mpl_new_Morphy"))
}

mpl_init_Morphy <- function(numtaxa, numchars, morphyobj)
{
	return(.Call("_R_wrap_mpl_init_Morphy", as.integer(numtaxa), as.integer(numchars), morphyobj))
}

mpl_get_numtaxa <- function(morphyobj)
{
	return(.Call("_R_wrap_mpl_get_numtaxa", morphyobj))
}

mpl_get_num_charac <- function(morphyobj)
{
	return(.Call("_R_wrap_mpl_get_num_charac", morphyobj))
}

mpl_attach_symbols <- function(symbols, morphyobj)
{
	return(.Call("_R_wrap_mpl_attach_symbols", symbols, morphyobj))
}

mpl_attach_rawdata <- function(rawdata, morphyobj)
{
	return(.Call("_R_wrap_mpl_attach_rawdata", rawdata, morphyobj))
}

mpl_get_symbols <- function(morphyobj)
{
	str = .Call("_R_wrap_mpl_get_symbols", morphyobj)
	return(str)
}