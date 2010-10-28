#ifndef RINGBUF_H_INCLUDED
#define RINGBUF_H_INCLUDED

typedef struct RingBuf {
	int read_ptr;
	int write_ptr;
	unsigned char *buf;
	int buf_size;
} RingBuf;

void RingBuf_init(RingBuf *self, unsigned char *buf, int buf_size);
int RingBuf_push(RingBuf *self, unsigned char data);
unsigned char RingBuf_pop(RingBuf *self);
int RingBuf_maxsize(RingBuf *self);
int RingBuf_size(RingBuf *self);
int RingBuf_empty(RingBuf *self);
int RingBuf_full(RingBuf *self);


#endif /* RINGBUF_H_INCLUDED */
