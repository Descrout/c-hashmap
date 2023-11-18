# C Hashmap

Hashmap implementation in C written for educational purposes.  
Can only use "char*" as key but anything as value "void*".

```c
Hashmap* map = new_hashmap(/* Capactiy */ 100, /* free value callback */ &free_value);

// Set
hm_set_value(map, "asd", alloc_str("asd_val"));
hm_set_value(map, "qwe", alloc_str("qwe_val"));
hm_set_value(map, "zxc", alloc_str("zxc_val"));

assert(map->length == 3);

// Override
// This also will call the free_value callback for the old value.
hm_set_value(map, "asd", alloc_str("PogChamp"));

assert(map->length == 3);

// Get
char* val = (char*)hm_get_value(map, "asd");
printf("Value of asd: %s\n", val);
assert(strcmp(val, "PogChamp") == 0);

// Remove
hm_remove_value(map, "qwe");
assert(map->length == 2);
assert(hm_get_value(map, "qwe") == NULL);

// Iterate
hm_iterate(map, &iterate_callback);

// Free
free_hashmap(map);
```

### Helpers
```c
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
```
