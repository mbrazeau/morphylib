# MDB: This is what a real MorphyLib interface should look like. It is much
# simpler than what is being attempted. If we want to can some of the
# more time-intensive tasks inside of some C code, we can do that. But
# as it stands, the existing C-R interface for Morphy is attempting too
# many things within the body of one function. Let the user initiate a 
# Morphy session in R, then add dimensions, raw data, symbols, and any
# other parameters they wish. They should be able to do all of this in 
# R through these bindings. If, for instance, we then want to write a 
# function that calculates the length of a tree, we can do that with R by
# any number of designs we choose. We could iterate over the tree in R, 
# but calculating the length by calling only MorphyLib functions. 
#
# If we want to can the iteration over the nodes in C, we can write some C
# to do this, but it doesn't need a giant 5-argument function that tries
# to do everything in one go. It just needs 2: the tree and the Morphy 
# object that was initialised in R through the functions below.
#

dyn.load("RMorphyex.so")

mpl_new_Morphy <- function() 
{
	return(.Call("_R_wrap_mpl_new_Morphy"))
}

mpl_delete_Morphy <- function(morphyobj)
{
	return(.Call("_R_wrap_mpl_delete_Morphy", morphyobj))
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
