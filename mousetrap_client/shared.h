#define MAX_CVARS 4

struct mtCvars_t {
	char name[30];
	char defaultValue[60];
};

extern mtCvars_t mtCvars[MAX_CVARS];