#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Hashmap.h"

void free_value(char* val) {
	printf("Called free for value: %s\n", val);
	//Do some advance freeing, if your value is a complex struct.
	free(val);
}

void iterate_callback(const char *const key, char *const val) {
	printf("Called loop for: key[%s] value[%s]\n", key, val);
}

char* alloc_str(char* val) {
	size_t len = strlen(val) + 1;
	char* str = (char*)malloc(len);
	if (str == NULL) return NULL;
	memcpy(str, val, len);
	return str;
}

int main(void) {
	Hashmap* map = new_hashmap(/* Capactiy */ 100, /* free value callback */ &free_value);

	// Set
	hm_put(map, "asd", alloc_str("asd_val"));
	hm_put(map, "qwe", alloc_str("qwe_val"));
	hm_put(map, "zxc", alloc_str("zxc_val"));

	assert(map->length == 3);

	// Override
	// This also will call the free_value callback for the overriden value.
	hm_put(map, "asd", alloc_str("PogChamp"));

	assert(map->length == 3);

	// Get
	char* val = (char*)hm_get(map, "asd");
	printf("Value of asd: %s\n", val);
	assert(strcmp(val, "PogChamp") == 0);
	
	// Remove
	hm_remove(map, "qwe");
	assert(map->length == 2);
	assert(hm_get(map, "qwe") == NULL);

	// Iterate
	hm_iterate(map, &iterate_callback);

	// Free
	free_hashmap(map);

	return 0;
}