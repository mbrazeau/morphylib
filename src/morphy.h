//
//  morphy.h
//  MorPhy2
//
//  Created by mbrazeau on 23/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef morphy_h
#define morphy_h

#ifdef DEBUG
#include <stdio.h>
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include <glib.h>

//typedef struct mplarray_s {
//    int nelems;
//    int maxelems;
//    size_t elemsize;
//    void **data;
//} MPLarray;

/* Function prototypes */

Morphyp         mpl_new_Morphy_t(void);
void*           mpl_get_from_matrix(const int row, const int col, const int ncol, const size_t size, const void* data);
int             mpl_set_numtaxa(const int ntax, Morphyp m);
int             mpl_set_num_charac(const int ncharac, Morphyp m);
int             mpl_get_gaphandl(Morphyp handl);
int             mpl_check_data_loaded(Morphyp m);
char            mpl_get_gap_symbol(Morphyp handl);
MPLchtype*      mpl_get_charac_types(Morphyp handl);
int             mpl_part_push_index(int newint, MPLpartition* part);
int             mpl_delete_partition(MPLpartition* part);
MPLpartition*   mpl_new_partition(const MPLchtype chtype, const bool hasNA);
int             mpl_count_gaps_in_columns(Morphyp handl);
//MPLarray*   mpl_new_array(size_t elemsize);
//void        mpl_destroy_array(MPLarray* arr);
#endif /* morphy_h */
