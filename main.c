#include <stdio.h>

#include "mtmp.h"

int main(int argc, char **argv) {

	char loc[LOCLEN];
	char ret[WBUFSZ];

	if(argc > 1) strncpy(loc, argv[1], LOCLEN);
	mtmp(loc, "1", ret, WBUFSZ);
	printf("%s", ret);

	return 0;
}
