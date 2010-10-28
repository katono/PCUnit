#ifndef STRTOKEN_H_INCLUDED
#define STRTOKEN_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StrToken StrToken;
StrToken *StrToken_new(const char *str, const char *delims);
void StrToken_delete(StrToken *self);
size_t StrToken_count(StrToken *self);
const char *StrToken_get(StrToken *self, size_t idx);

#ifdef __cplusplus
}
#endif

#endif /* STRTOKEN_H_INCLUDED */
