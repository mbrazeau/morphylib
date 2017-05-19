library('phangorn')
dyn.load('../src/RMorphy.dll')  

tree <- read.tree(text="(((a, b), c), (d, (e, f)));")
matrix_as_grid <- t(matrix(c('0', '0', '1', '1', '0', '0',
                             '0', '0', '-', '-', '-', '1',
                             '0', '+', '?', '-', '1', '1', 
                             #'0', '+', '?', '-', '1', '{01}', 
                             '0', '1', '-', '-', '-', '0'), nrow=4, ncol=6, byrow=T))
rownames(matrix_as_grid) <- tree$tip.label
source('../RMorphy.c')
MorphyParsimony(tree, matrix_as_grid)

# devtools::install_github('ms609/inapplicable')
library(inapplicable)
data(SigSut)
phy <- SigSut.phy
tip.names <- names(phy)[-1]
tree <- rtree(length(tip.names), tip.label=tip.names, br=NULL)
MorphyParsimony(tree, phy)


dyn.unload('../src/RMorphy.dll')  
