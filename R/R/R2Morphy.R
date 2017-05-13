#' @title R to morphy interface
#'
#' @description Uses the MorPhy library to calcuate the length of a phylogenetic tree
#'
#' @param \list {
#'   \item{tree} A tree in \code{PhyDat} format;
#'   \item{data} A object of class phyDat containing sequences
#'   \item{site} return iether 'pscore' or 'site' wise parsimony scores
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
  matrix_as_string <- paste0(vapply(as.integer(unlist(data)), function (x) {
    if (x == nrow.contrast) return('?')
    tokens <- levels[contrast[x ,]]
    if (length(tokens) > 1) return (paste0('{', tokens, '}', collapse=''))
    return (tokens)
  }, character(1)), collapse='')
} else if (class(data) == 'list') {
  tax.names <- rownames(dat)
  dim.dat <- dim(dat.matrix)
  n.char <- dim.dat[1]
  n.tip  <- dim.dat[2]
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

if (max.node != max(parent)) stop ("Tree must be binary")
if (root.node != min(parent)) stop ("Root node miscalculated")

tips <- seq_len(n.tip)
postorder <- max.node:root.node
preorder <- root.node:max.node

ancestor <- function (x) parent[child==x]
descendant <- function (x) child[parent==x]
ancestors <- as.integer(c(vapply(1:n.tip, ancestor, double(1)), 0, vapply((n.tip + 2):(n.tip * 2 - 1), ancestor, double(1))))
descendants <- as.integer(vapply(preorder, descendant, double(2))) # children of each node, a pair at a time, left-right, left-right


dyn.load('C:/Users/ms609/RMorphy.dll') ## Replace with a sensible path to RMorphy.o
.Call('Morphy', as.integer(n.char), as.integer(n.tip), as.integer(descendants), as.integer(ancestors), as.character(matrix_as_string))
dyn.unload('C:/Users/ms609/RMorphy.dll') ## Replace with a sensible path to RMorphy.o




