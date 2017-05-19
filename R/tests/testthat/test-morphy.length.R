library(Inapp)

context("morphy.length")
test_that("right counting compared to inapp", {
    ## Tree
    tree <- read.tree(text = "((((((1,2),3),4),5),6),(7,(8,(9,(10,(11,12))))));")
    ## Data
    characters <- c("23--1??--032", # 0
                    "1---1111---1", # 1
                    "1100----1100", # 2
                    "11-------100", # 3
                    "----1111---1", # 4
                    "01----010101", # 5
                    "01---1010101", # 6
                    "1??--??--100", # 7
                    "21--3??--032", # 8
                    "11--1??--111", # 9
                    "11--1000001-", # 10
                    "01------0101", # 11
                    "110--?---100", # 12
                    "11--1??--111", # 13
                    "210--100--21", # 14
                    "????----1???", # 15
                    "23--1----032", # 16
                    "1----1----1-", # 17
                    "-1-1-1--1-1-", # 18
                    "23--1??--032", # 19
                    "--------0101", # 20
                    "10101-----01", # 21
                    "011--?--0011", # 22
                    "110--??--100", # 23
                    "11--1000001-", # 24
                    "21--1----012", # 25
                    "11----111111", # 26
                    "10101-----01", # 27
                    "210210------", # 28
                    "----1111----", # 29
                    "230--??1--32", # 30
                    "023--??1--32", # 31
                    "023-???1--32", # 32
                    "23--1?1--023", # 33
                    "----1010----", # 34
                    "------11---1", # 35
                    "10----11---1", # 36
                    "320--??3--21") # 37
    ## Data as a matrices
    matrices <- sapply(characters, strsplit, split = "")
    matrices <- lapply(matrices, function(X) matrix(data=X, ncol = 1))
    ## Results
    expected_results <- c(5, 2, 3, 2, 1, 5, 5, 2, 5, 2, 2, 4, 3, 2, 5, 0, 5, 2, 4, 5, 2, 4, 3, 3, 2, 5, 1, 4, 4, 0, 5, 5, 4, 5, 2, 1, 3, 5)

    ## INAPP
    for(test in 1:length(characters)) {
        suppressWarnings(output <- apply.reconstruction(tree, characters[test], passes = 4, method = "NA", inapplicable = NULL))
        tree_length <- output$regions + ifelse(length(output$changes) > 0, length(output$changes), 0)
        expect_equal(tree_length, expected_results[test])
    }

    ## MORPHYLIB
    for(test in 1:length(characters)) {
        tree_length <- morphy.length(tree, matrices[[test]])
        expect_equal(tree_length, expected_results[test])
    }
})

# tree <- rtree(50, br = NULL)
# characters <- sample(c("1", "0", "-"), 50, replace = TRUE)

# time_inapp <- system.time(test <- replicate(100, apply.reconstruction(tree, characters, passes = 4, method = "NA", inapplicable = NULL)))

# matrix <- matrix(data = c(rep(characters, 100)), ncol = 100, nrow = 50, byrow = FALSE)
# time_inapp <- system.time(test2 <- morphy.length(tree, matrix))




