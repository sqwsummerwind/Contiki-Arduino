#include "stm32def.h"

/*---------------------------------------------------------------------------*/
void *w_memcpy(void *out, const void *in, size_t n)
{
	uint8_t *src, *dest;
	src = (uint8_t *)in;
	dest = (uint8_t *)out;
	while (n-- > 0) {
		*dest++ = *src++;
	}
	return out;
}
/*---------------------------------------------------------------------------*/
void *w_memset(void *out, int value, size_t n)
{
	uint8_t *dest;
	dest = (uint8_t *)out;
	while (n-- > 0) {
		*dest++ = value & 0xff;
	}
	return out;
}
/*---------------------------------------------------------------------------*/