#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

void test(void)
{
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            printf("Error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Can't allocate redis context\n");
        }
        return;
    }
}