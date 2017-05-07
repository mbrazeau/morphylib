//
//  mpl.h
//  MorPhy2
//
//  Created by mbrazeau on 04/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#ifndef mpl_h
#define mpl_h


// Public functions
Morphy  mpl_new_Morphy          (void);
int     mpl_delete_Morphy       (Morphy m);
int     mpl_init_Morphy         (const int ntax, const int nchar, Morphy m);
int     mpl_get_numtaxa         (Morphy m);
int     mpl_get_num_charac      (Morphy m);
int     mpl_attach_symbols      (const char* symbols, Morphy m);
char*   mpl_get_symbols         (const Morphy m);
int     mpl_attach_rawdata      (const char* rawmatrix, Morphy m);
int     mpl_delete_rawdata      (Morphy m);
int     mpl_set_gap_symbol      (const char gapsymb, Morphy m);
int     mpl_set_missing_symbol  (const char missymb, Morphy m);

int     mpl_set_postorder       (const int nodeID, const int index, Morphy m);

int     mpl_incl_charac         (const int charID, Morphy m);
int     mpl_excl_charac         (const int charID, Morphy m);

int     mpl_set_charac_weight   (const int charID, Mflt weight);

int     mpl_set_parsim_t        (const int charID, const ptype_t parsfxn, Morphy m);
int     mpl_set_gaphandl        (const gap_t gaptype, Morphy m);
int     mpl_query_gaphandl      (Morphy m);

int     mpl_down_recon          (const int nodeID, const int lChild, const int rChild, Morphy m);
int     mpl_up_recon            (const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
int     mpl_up_final_recon      (const int nodeID, const int lChild, const int rChild, const int parentID, Morphy m);
int     mpl_update_tip          (const int nodeID, Morphy m);
int     mpl_finalize_tip        (const int nodeID, Morphy m);

int     mpl_get_insertcost_max  (const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);
int     mpl_get_insertcost_min  (const int srcID, const int tgt1ID, const int tgt2ID, Morphy m);

int     mpl_get_packed_states   (const int nodeID, const int character, int passnum, Morphy m);
int     mpl_get_stateset        (char* res, const int nodeID, const int character, int passnum, Morphy m);

#endif /* mpl_h */
