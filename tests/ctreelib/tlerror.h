//
//  tlerror.h
//  treelib
//
//  Created by mbrazeau on 22/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef tlerror_h
#define tlerror_h

typedef enum {
    ERR_EX_DAT_CONFLICT     = -16,  /*!< Conflict with existing loaded data */
    ERR_SYMBOL              = -15,  /*!< Unrecognized symbol or bad symbol */
    ERR_NWK_END             = -14,  /*!< No terminal semicolon on Newick */
    ERR_CPARENS_EXC         = -13,  /*!< Mismatched parentheses: too many closing */
    ERR_OPARENS_EXC         = -12,  /*!< Mismatched parentheses: too many opening */
    ERR_NO_ROOT_CMD         = -11,  /*!< No rooting command found in Newick */ 
    ERR_TGT_NOT_FOUND       = -10,
    ERR_TRAVERSAL           = -9,
    ERR_INDEX_OUT_OF_RANGE  = -8,   /*!< Attempt to index out of range */
    ERR_EXC_CAPACITY        = -7,   /*!< Attempt to exceet capacity of a buffer */
    ERR_BAD_PARAM           = -6,   /*!< Passing bad parameter as argument */
    ERR_NULL_PTR            = -5,   /*!< Unexpected NULL pointer */
    ERR_BAD_MALLOC          = -4,   /*!< Malloc failed */
    ERR_EXLIB_FXN           = -3,   /*!< External library function error */
    ERR_BAD_FREE            = -2,
    ERR_NOT_IMPLEMENTED     = -1,
    ERR_NOERR               = 0,
} TL_ERR_T;

#endif /* tlerror_h */
