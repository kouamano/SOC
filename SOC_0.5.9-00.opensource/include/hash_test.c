/* hash_test.c */
#include <stdio.h>
#include "hash.h"
#include "hash.c"

int main(void)
{
    /* 文字列->整数のハッシュテーブル */
    {
        HASH hash;
        int n, *np;

        hash = make_hash(64, sizeof(int));

        n = 1;
        hash_put(hash, "foo", &n);

        n = 2;
        hash_put(hash, "bar", &n);

        if ( (np = (int *)hash_get(hash, "foo")) != HASH_NOVAL)
            printf("foo = %d\n", *np);
        else
            printf("foo = NOVAL\n");
        
        if ( (np = (int *)hash_get(hash, "bar")) != HASH_NOVAL)
            printf("bar = %d\n", *np);
        else
            printf("bar = NOVAL\n");

        if ( (np = (int *)hash_get(hash, "baz")) != HASH_NOVAL)
            printf("baz = %d\n", *np);
        else
            printf("baz = NOVAL\n");

        free_hash(hash);
    }

    /* 文字列->文字列のハッシュテーブル */
    {
        HASH hash;
        const char *str;

        hash = make_hash(64, sizeof(char) * 64);

        hash_put(hash, "foo", "bar");
        hash_put(hash, "bar", "qux");

        if ( (str = (const char *)hash_get(hash, "foo")) != HASH_NOVAL)
            printf("foo = %s\n", str);
        else
            printf("foo = NOVAL\n");

        if ( (str = (const char *)hash_get(hash, "bar")) != HASH_NOVAL)
            printf("bar = %s\n", str);
        else
            printf("bar = NOVAL\n");

        if ( (str = (const char *)hash_get(hash, "baz")) != HASH_NOVAL)
            printf("baz = %s\n", str);
        else
            printf("baz = NOVAL\n");

        free_hash(hash);
    }

    return 0;
}
