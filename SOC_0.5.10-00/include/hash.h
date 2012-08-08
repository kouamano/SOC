/* hash.h */
#ifndef ___HASH_H
#define ___HASH_H

#include <stddef.h>

#define HASH_NOVAL NULL

typedef void *HASH;

HASH make_hash(size_t table_size, size_t value_size);
void free_hash(HASH hash);
void hash_put(HASH hash, const char *key, const void *value);
void *hash_get(HASH hash, const char *key);

#endif  /* ___HASH_H */
