#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <jansson.h>
#include <curl/curl.h>

#define BUFSZ  8192
#define LOCLEN  128
#define VALLEN  128

#define LAPIURL "ip-api.com/json"
#define WAPIURL "api.openweathermap.org/data/2.5/weather?q="
#define WAPIKEY "e3153384df37ea0a3a3d51cc5a08d72d"

#define O_NOUINF 0
#define O_UINF 1

typedef struct weather {
	char loc[LOCLEN];
	double temp;
} weather;

typedef struct wresult {
    char *data;
    int pos;
} wresult;

static int die(char *err, int uinf, int ret) {

	if(err[0]) fprintf(stderr, "Error: %s\n", err);
	exit(ret);
}

static size_t wresp(void *ptr, size_t size, size_t nmemb, void *stream) {

    wresult *result = (wresult*)stream;

    if(result->pos + size * nmemb >= BUFSZ - 1)
		die("Buffer too small", O_NOUINF, 1);

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

static char *creq(const char *url) {

    CURL *curl = NULL;
    CURLcode status;
    struct curl_slist *headers = NULL;
    char *data = calloc(BUFSZ, sizeof(char));
    long code;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl) die("Could not start curl", O_NOUINF, 1);

    wresult wresult = {data, 0};
    headers = curl_slist_append(headers, "User-Agent: mtmp");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wresp);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wresult);

    status = curl_easy_perform(curl);
    if(status != 0) die("Could not read data", O_NOUINF, 2);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200) die("Returning error code from server", O_NOUINF, code);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    data[wresult.pos] = 0;

    return data;
}

static char *geoloc(char *ret, size_t len) {

	char *raw;
	raw = creq(LAPIURL);

	json_t *root;
	json_error_t err;

	root = (json_loads(raw, 0, &err));
	free(raw);

	if(!json_is_object(root)) return NULL;

	const char *key;
	json_t *val;
	json_object_foreach(root, key, val) { if(!strncmp(key, "city", len)) break; }
	strncpy(ret, json_string_value(val), len);

	return ret;
}

int main(int argc, char **argv) {

	char url[256];
	char *raw;

	weather wtr;

	json_t *root;
	json_error_t err;

	if(argc < 2) strncpy(wtr.loc, geoloc(wtr.loc, LOCLEN), LOCLEN);
	else strncpy(wtr.loc, argv[1], LOCLEN);
	if(!wtr.loc[0]) die("Could not retrieve geolocation", O_NOUINF, 1);
	wtr.loc[0] = toupper(wtr.loc[0]);

	snprintf(url, 256, "%s%s&appid=%s", WAPIURL, wtr.loc, WAPIKEY);
	raw = creq(url);
	if(!raw) die("Could not read data", O_NOUINF, 2);

	root = (json_loads(raw, 0, &err));
	free(raw);
	if(!root) die("JSON decoding failed", O_NOUINF, 3);

	const char *key;
	json_t *v1, *v2;
	json_object_foreach(root, key, v1) { if(!strcmp(key, "main")) break; }
	if(!json_is_object(v1)) die("Unexpected JSON format", O_NOUINF, 4);

	json_object_foreach(v1, key, v2) {
		if(!strncmp(key, "temp", VALLEN)) wtr.temp = json_real_value(v2) - 273.15;
	}

	printf("Current temperature in %s is %.2fC\n", wtr.loc, wtr.temp);

	return 0;
}
