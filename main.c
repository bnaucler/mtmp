#include <stdio.h>

#include "mtmp.h"

int main(int argc, char **argv) {

	char loc[LOCLEN] = "";
	char ret[WBUFSZ];
	char *ip = NULL;

	if(argc > 1) strncpy(loc, argv[1], LOCLEN);
	else ip = creq(IPECHO);

	mtmp(loc, ip, ret, WBUFSZ);
	printf("%s", ret);

	return 0;
}
