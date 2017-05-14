#include "../../src/mpl.h"
#include "RMorphyUtils.h"

MPLchtype _R_mpl_str2chtype(const char *chtypename)
{

	if(!strcasecmp(chtypename, "fitch")){
		return FITCH_T;
	}
	else if(!strcasecmp(chtypename, "wagner")){
		return WAGNER_T;
	}
	else if(!strcasecmp(chtypename, "dollo")){
		return DOLLO_T;
	}
	else if(!strcasecmp(chtypename, "irreversible")){
		return IRREVERSIBLE_T;
	}
	else if(!strcasecmp(chtypename, "user")){
		return USERTYPE_T;
	}

	return MAX_CTYPE;
}
