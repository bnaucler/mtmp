#include <stdio.h>

#include "mtmp.h"

int main(int argc, char **argv) {

	char loc[LOCLEN] = "";
	char str[WSTRLEN];
	char *ip = NULL;
	weather wtr;

	if(argc > 1) strncpy(loc, argv[1], LOCLEN);
	else ip = creq(IPECHO);

	mtmp(loc, ip, &wtr);
	puts(mkwstr(&wtr, str, sizeof(str)));

	return 0;
}
