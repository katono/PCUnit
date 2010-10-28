#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "StrToken.h"

struct StrToken {
	char *buf;
	char **token;
	size_t token_count;
};

static int is_delim(char c, const char *delims)
{
	const char *p;
	for (p = delims; *p; p++) {
		if (*p == c) {
			return 1;
		}
	}
	return 0;
}

StrToken *StrToken_new(const char *str, const char *delims)
{
	StrToken *self;
	char *p;
	size_t len;
	size_t token_max;
	int prev_is_delim;
	assert(str);
	assert(delims);

	len = strlen(str) + 1;
#define ALIGN(size)	(((size) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))
	token_max = ALIGN(sizeof(char *) * len >> 1);
	self = (StrToken *) malloc(sizeof(StrToken) + token_max + len);
	if (!self) return 0;

	self->token = (char **) (self + 1);
	self->buf = ((char *) self->token) + token_max;
	strcpy(self->buf, str);

	self->token_count = 0;
	prev_is_delim = 1;
	for (p = self->buf; *p; p++) {
		if (is_delim(*p, delims)) {
			*p = '\0';
			prev_is_delim = 1;
		} else {
			if (prev_is_delim) {
				self->token[self->token_count++] = p;
			}
			prev_is_delim = 0;
		}
	}
	assert(self->token_count <= (len >> 1));

	return self;
}

void StrToken_delete(StrToken *self)
{
	if (!self) return;
	free(self);
}

size_t StrToken_count(StrToken *self)
{
	assert(self);
	return self->token_count;
}

const char *StrToken_get(StrToken *self, size_t idx)
{
	assert(self);
	assert(idx < self->token_count);
	return self->token[idx];
}

