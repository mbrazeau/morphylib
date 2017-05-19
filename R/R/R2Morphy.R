#' @title R to morphy interface
#'
#' @description Uses the MorPhy library to calcuate the length of a phylogenetic tree
#'
#' @param \list {
#'   \item{tree} A tree in \code{PhyDat} format;
#'   \item{data} A object of class phyDat containing sequences
#'   \item{site} return either 'pscore' or 'site' wise parsimony scores
#' 
#' @examples
#' ## Load a tree and a matrix
#' tree <- ape::read.tree(text = "((a,b),(c,d));")
#' matrix <- matrix(c('0', '1', '-', '-', '?', '{01}', '0', '1'), nrow=2, ncol=2)
#' 
#' ## Calculate the length
#' morphy.length(tree, matrix, site='pscore')
#' 
#' @seealso \code{\link{apply.reconstruction}}
#' 
#' @author Thomas Guillerme, Martin Smith
#' @export

library('phangorn')
  dat <- read.nexus.data('C:/Work/parsimony_inapp/capaetal/capa.nex')  ## Use phyDat
 data <- phyDat(dat, type='USER', levels=unlist(strsplit('+012345678q-', '')))
 tax.names <- names(data)
 n.tip <- length(tax.names)
tree <- rtree(n.tip, tip.label=tax.names, br=NULL)


if (class(data) == 'phyDat') {
  tax.names <- names(data)
  n.char <- length(data[[1]])
  weight <- attr(data, 'weight')
  contrast <- attr(data, 'contrast')
  dim.contrast <- dim(contrast)
  nrow.contrast <- dim.contrast[1]
  contrast <- matrix(as.logical(contrast), nrow.contrast, dim.contrast[2])
  levels <- attr(data, 'levels')
  dat <- vapply(data, as.integer, data[[1]])
  matrix_as_string <- paste0(c(vapply(as.integer(unlist(data)), function (x) {
    if (x == nrow.contrast) return('?')
    tokens <- levels[contrast[x ,]]
    if (length(tokens) > 1) return (paste0('{', tokens, '}', collapse=''))
    return (tokens)
  }, character(1)), ';'), collapse='')
} else if (class(data) == 'list') {
  tax.names <- rownames(dat)
  dim.dat <- dim(dat.matrix)
  n.char <- dim.dat[1]
  n.tip  <- dim.dat[2]
  n.internal <- 
  dat.matrix <- vapply(dat, as.vector, dat[[1]])
  matrix_as_string <- paste0(t(dat.matrix), collapse='')
  weight <- rep(1, n.char)
} else {
  stop ("Unrecognized data format")
}


n.tip <- length(tax.names)
edge <- tree$edge
parent <- edge[, 1]
child  <- edge[, 2]

max.node <- n.tip * 2 - 1
root.node <- n.tip + 1
dummy.root.node <- max.node + 1
n.internal <- max.node - n.tip + 1 # +1 for dummy root node

if (max.node != max(parent)) stop ("Tree must be binary")
if (root.node != min(parent)) stop ("Root node miscalculated")

tips <- seq_len(n.tip)
postorder <- max.node:root.node
preorder <- root.node:max.node

ancestor <- function (x) parent[child==x]
descendant <- function (x) child[parent==x]
ancestors <- as.integer(c(vapply(1:n.tip, ancestor, double(1)), 0, vapply((n.tip + 2):(n.tip * 2 - 1), ancestor, double(1))))
descendants <- as.integer(vapply(preorder, descendant, double(2))) # children of each node, a pair at a time, right-left, right-left
right_ids <- left_ids <- integer(max.node)
odds <- as.logical(seq_along(descendants) %% 2)
right_ids[root.node:max.node] <- descendants[odds]
 left_ids[root.node:max.node] <- descendants[!odds]

dyn.load("../src/RMorphyEx.dll")
source("../src/Morphyex.R")
morphy <- mpl_new_Morphy()
if (mpl_init_Morphy(n.tip, n.char, morphy)) stop("Error in mpl_init_Morphy")
if (mpl_get_numtaxa(morphy) != n.tip) warning("Taxon count mismatch with mpl_get_numtaxa")
if (mpl_get_num_charac(morphy) != n.char) warning("Character count mismatch in mpl_get_num_charac")
if (msg <- mpl_attach_rawdata(as.character(matrix_as_string), morphy)) stop("mpl_attach_rawdata Couldn't attach symbols: error", msg)
for (i in (1:n.char) - 1) if (msg <- mpl_set_parsim_t(i, tname=as.character("FITCH"), morphy)) stop("mpl_set_parsim_t error with char", i, msg)
if (mpl_get_symbols(morphy) != paste0(levels[levels != '-'], collapse='')) stop ("Symbols mismatch in mpl_get_symbols")

if (mpl_set_num_internal_nodes(n.internal, morphy)) stop ("mpl_set_num_internal_nodes failed")
if (mpl_get_num_internal_nodes(morphy) != n.internal) stop('mpl_get_num_internal_nodes mismatch')
if (err <- mpl_apply_tipdata(morphy)) stop("mpl_apply_tipdata failed", err)

for (node in postorder) {
  score <- mpl_first_down_recon(node - 1L, left_ids[node] - 1L, right_ids[node] - 1L, morphy)
  #cat("Node ", node, "left: ", left_ids[node], " right, ", right_ids[node], "\n")
}
score
if (mpl_update_lower_root(dummy.root.node - 1L, root.node - 1L, morphy)) stop ("mpl_update_lower_root Error")
for (node in preorder[-1]) {
  #cat("Node:", node, "left:", left_ids[node], " right:", right_ids[node], "anc:", ancestors[node], "\n")
  score <- mpl_first_up_recon(node - 1L,  left_ids[node] - 1L, right_ids[node] - 1L, ancestors[node] - 1L, morphy) 
}
score
for (tip in tips) {
  mpl_update_tip(tip - 1L, ancestors[tip] - 1L, morphy)
}
for (node in postorder) {
  score <- mpl_second_down_recon(node - 1L, left_ids[node] - 1L, right_ids[node] - 1L, morphy)
}
if (mpl_update_lower_root(dummy.root.node - 1L, root.node - 1L, morphy)) stop ("Second mpl_update_lower_root Error")
for (node in preorder[-1]) {
  score <- mpl_second_up_recon(node - 1L,  left_ids[node] - 1L, right_ids[node] - 1L, ancestors[node] - 1L, morphy) 
}
score

mpl_delete_Morphy(morphy)

#ret <- .Call('Morphy', as.integer(n.char), as.integer(n.tip), as.integer(descendants), as.integer(ancestors), as.character(matrix_as_string))
#ret # Returns a list with two entries: [1], the root node; [2], the descendants of the root node's left child.



