#' @title Count a tree length
#'
#' @description Count a tree length using the Morphy Lib implementation
#'
#' @param tree a \code{phylo} object.
#' @param matrix a discrete morphological matrix
#' 
#' @examples
#'
#' @seealso
#' 
#' @author Thomas Guillerme
#' @export

morphy.length <- function(tree, matrix) {
    ## Check tree


    # tree <- read.tree(text = "((((((1,2),3),4),5),6),(7,(8,(9,(10,(11,12))))));") ; stop("DEBUG")
    # ## Data
    # matrix <- matrix(data = c("2", "3", "-", "-", "1", "?", "?", "-", "-", "0", "3", "2"), ncol = 1) ; stop("DEBUG")

    ## Check matrix and replace get dimensions

    ## morphy variables
    rawmatrix <- paste(apply(matrix, 2, paste, collapse = ""), ";", sep = "")
    ntax <- ape::Ntip(tree)
    nnode <- ape::Nnode(tree)
    nchar <- ncol(matrix)

    ## Initialise the morphy object
    morphy_object <- mpl_new_Morphy()
    catch_error <- mpl_init_Morphy(ntax, nchar, morphy_object) #TG: add a catch error handler in a safer version
    catch_error <- mpl_attach_rawdata(rawmatrix, morphy_object)
    catch_error <- mpl_set_num_internal_nodes((nnode+1), morphy_object) #+1 for the dummy root
    catch_error <- mpl_apply_tipdata(morphy_object)
    
    ## Get the list of ancestors and descendants
    edge <- tree$edge
    descendant <- function (x) edge[, 2][edge[, 1]==x]
    descendants <- as.integer(vapply((ntax+1):(ntax+nnode), descendant, double(2)))
    left_descendants <- rev(descendants[seq(1, length(descendants), 2)])
    right_descendants <- rev(descendants[seq(2, length(descendants), 2)])
    tips_ancestors <- edge[match(1:ntax, edge[,2]),1]
    ancestors <- nodes <- rev((ntax+1):(nnode+ntax))

    ## Change indexing to start from 0
    left_descendants <- left_descendants - 1
    right_descendants <- right_descendants - 1
    tips_ancestors <- tips_ancestors - 1
    ancestors <- ancestors - 1
    nodes <- nodes - 1

    ## Initialise the tree length
    length <- 0

    ## First downpass
    sapply_first_down <- function(i, nodes, left_descendants, right_descendants, morphy_object) {
        return(mpl_first_down_recon(nodes[i], left_descendants[i], right_descendants[i], morphy_object))
    }
    increment <- sum(sapply(1:(ntax-1), sapply_first_down, nodes, left_descendants, right_descendants, morphy_object))
    length <- length + increment #TG: note that this is pretty useless (no length increment possible here). Just left it for consistency.

    ## Update the root
    catch_error <- mpl_update_lower_root(nnode+ntax, ntax, morphy_object)

    ## First uppass
    sapply_first_up <- function(i, nodes, left_descendants, right_descendants, ancestors, morphy_object) {
        return(mpl_first_up_recon(nodes[i], left_descendants[i], right_descendants[i], ancestors[i], morphy_object))
    }
    increment <- sum(sapply((ntax-2):1, sapply_first_up, nodes, left_descendants, right_descendants, ancestors, morphy_object))
    length <- length + increment #TG: note that this is pretty useless (no length increment possible here). Just left it for consistency.

    ## Update the tips
    sapply_update_tip <- function(i, tips_ancestors, morphy_object) {
        return(mpl_update_tip(i, tips_ancestors[i], morphy_object))
    }
    catch_error <- sapply(1:ntax, sapply_update_tip, tips_ancestors, morphy_object)

    ## Second downpass
    sapply_second_down <- function(i, nodes, left_descendants, right_descendants, morphy_object) {
        return(mpl_second_down_recon(nodes[i], left_descendants[i], right_descendants[i], morphy_object))
    }
    increment <- sum(sapply(1:(ntax-1), sapply_second_down, nodes, left_descendants, right_descendants, morphy_object))
    length <- length + increment

    ## Second uppass
    sapply_second_up <- function(i, nodes, left_descendants, right_descendants, ancestors, morphy_object) {
        return(mpl_second_up_recon(nodes[i], left_descendants[i], right_descendants[i], ancestors[i], morphy_object))
    }
    increment <- sum(sapply((ntax-2):1, sapply_second_up, nodes, left_descendants, right_descendants, ancestors, morphy_object))
    length <- length + increment
  
    ## Finalise tips TG: this bit is not useful for length counting.
    # for(i in 1:ntax) {
    #     catch_error <- mpl_finalize_tip(i, tips_ancestors[i], morphy_object)
    # }
 
    return(length)
}