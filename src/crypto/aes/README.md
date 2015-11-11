Example of AES encrypt/decrypt
==============================

	#include "aes.h"

	/* 256 bits AES Key */
	uint8_t key[] = "\xcf\x99\x42\x29\x25\xeb\xe6\xd6\xde\x41\x51\xd6\x39\x39\x70\x4c\xac\xab\x6b\x0c\x43\x31\xf0\x49\x9c\xef\x4b\x8e\xc0\x91\xaa\xdf";

	/* Key Expanded Structure */
	aes256_ctx_t ctx;

	/* 16 bytes buffer with the message to encrypt/decrypt */
	char buffer[] = "1234567890123456";

	int main(void)
	{	
		/* Initialize the Board */
		/* ... */

		/* Initialize AES with Key */
		aes_init(key,  256, &ctx);

		/* Encrypt 16 bytes buffer */
		aes256_enc(buffer, &ctx);

		/* Decrypt 16 bytes buffer */
		aes256_dec(buffer, &ctx);
	}

