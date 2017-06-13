#include <stdio.h>

#include "mtmp.h"

int main(int argc, char **argv) {

	char loc[LOCLEN] = "";
	char *ip = NULL;
	weather wtr;

	if(argc > 1) strncpy(loc, argv[1], LOCLEN);
	else ip = creq(IPECHO);

	mtmp(loc, ip, &wtr);

	printf("%s, %s: %s %.1fC, humidity: %d%%, %d hPa, %.1f m/s %s\n",
			wtr.loc, wtr.cc, wtr.desc, wtr.temp, wtr.hum, wtr.pres,
			wtr.ws, wtr.wdir);

	return 0;
}
