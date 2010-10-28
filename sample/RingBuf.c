#include "RingBuf.h"


void RingBuf_init(RingBuf *self, unsigned char *buf, int buf_size)
{
	self->read_ptr = self->write_ptr = 0;
	self->buf = buf;
	self->buf_size = buf_size;
}

int RingBuf_push(RingBuf *self, unsigned char data)
{
	if (RingBuf_full(self)) {
		return 0;
	}
	self->buf[self->write_ptr] = data;
	self->write_ptr = ((self->write_ptr + 1) & (self->buf_size - 1));
	return 1;
}

unsigned char RingBuf_pop(RingBuf *self)
{
	unsigned char ret;
	ret = self->buf[self->read_ptr];
	self->read_ptr = ((self->read_ptr + 1) & (self->buf_size - 1));
	return ret;
}

int RingBuf_maxsize(RingBuf *self)
{
	return self->buf_size - 1;
}

int RingBuf_size(RingBuf *self)
{
	return (self->read_ptr <= self->write_ptr) ? 
		self->write_ptr - self->read_ptr : 
		self->buf_size - self->read_ptr + self->write_ptr;
}

int RingBuf_empty(RingBuf *self)
{
	return self->read_ptr == self->write_ptr;
}

int RingBuf_full(RingBuf *self)
{
	return self->read_ptr == ((self->write_ptr + 1) & (self->buf_size - 1));
}

