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

library('ape')
dyn.load('Rinterface.dll')
  dat <- read.nexus.data('C:/Work/parsimony_inapp/capaetal/capa.nex')  ## Use phyDat
 data <- phyDat(dat, type='USER', levels=unlist(strsplit('+012345678q-', '')))
 tax.names <- names(data)
 n.tip <- length(tax.names)
tree <- rtree(n.tip, tip.label=tax.names, br=NULL)


morphy.length <- function (tree, data, site = 'pscore') {
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

  .Call('morphy_handler_R', as.character(matrix_as_string), as.integer(n.tips), as.integer(n.char), ancestors, descendants, as.integer(weight))
  
  ret
}

####    The C code will go something like this:
####    morphy <- new_morphy(); #### ...
####    
####    ask_mpl_to_initialize_characters(n.char)
####    
####    ## SEXP WILL DO THIS
####    initialize_character <- function (i) mpl_set_parsim_t(i, FITCH_T, morphyinstance)
####    vapply(seq_len(n.char), initialize_character, 0)  # Update expected output to what init_char returns
####    # DO THIS IN C
####    #for (i = 0; i < n_char; i++) {
####    #  mpl_set_parsim_t(i, FITCH_T, morphyinstance);
####    #}
####    # mpl_apply_tipdata(morphinstance);
####    
####    
####    for (node in postorder) { # First Downpass 
####      ask_mpl_to_resolve_applic_down(node, descendants[node, 1], descendants[node, 2], ancestors[node])
####    }
####    
####    for (node in preorder) { # First uppass: internal nodes
####      ask_mpl_to_resolve_applic_up(node, descendants[node, 1], descendants[node, 2], ancestors[node])
####    }
####    
####    for (tip in tips) { # First uppass: update tips
####      ask_mpl_to_update_tip(tip, ancestors[tip])
####    }
####    
####    ask_mpl_for_prelim_recon();
####    
####    for (node in postorder) {
####      ask_mpl_to_resolve_applic_up(node, descendants[node, 1], descendants[node, 2], ancestors[node])
####    }
####    
####    for (tip in tips) { 
####      ask_mpl_to_finalize_tip(tip, ancestors[tip]) 
####    } # Second uppass: finalize tips.  Redundant?
####    
####    ask_mpl_for_final_recon();
####    ask_mpl_to_store_final_active_states();
####    
####    
