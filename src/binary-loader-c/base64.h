/* Licensed under BSD-MIT - see LICENSE file for details */
#ifndef CCAN_BASE64_H
#define CCAN_BASE64_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

/**
 * base64_maps_t - structure to hold maps for encode/decode
 */
typedef struct {
	char encode_map[64];
	signed char decode_map[256];
} base64_maps_t;

/**
 * base64_encoded_length - Calculate encode buffer length
 * @param srclen the size of the data to be encoded
 * @note add 1 to this to get null-termination
 * @return Buffer length required for encode
 */
size_t base64_encoded_length(size_t srclen);

/**
 * base64_decoded_length - Calculate decode buffer length
 * @param srclen Length of the data to be decoded
 * @note This does not return the size of the decoded data!  see base64_decode
 * @return Minimum buffer length for safe decode
 */
size_t base64_decoded_length(size_t srclen);

/**
 * base64_init_maps - populate a base64_maps_t based on a supplied alphabet
 * @param dest A base64 maps object
 * @param src Alphabet to populate the maps from (e.g. base64_alphabet_rfc4648)
 */
void base64_init_maps(base64_maps_t *dest, const char src[64]);


/**
 * base64_encode_triplet_using_maps - encode 3 bytes into base64 using a specific alphabet
 * @param maps Maps to use for encoding (see base64_init_maps)
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 characters
 */
void base64_encode_triplet_using_maps(const base64_maps_t *maps,
				      char dest[4], const char src[3]);

/**
 * base64_encode_tail_using_maps - encode the final bytes of a source using a specific alphabet
 * @param maps Maps to use for encoding (see base64_init_maps)
 * @param dest Buffer containing 4 bytes
 * @param src Buffer containing srclen bytes
 * @param srclen Number of bytes (<= 3) to encode in src
 */
void base64_encode_tail_using_maps(const base64_maps_t *maps, char dest[4],
				   const char *src, size_t srclen);

/**
 * base64_encode_using_maps - encode a buffer into base64 using a specific alphabet
 * @param maps Maps to use for encoding (see base64_init_maps)
 * @param dest Buffer to encode into
 * @param destlen Length of dest
 * @param src Buffer to encode
 * @param srclen Length of the data to encode
 * @return Number of encoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen (past any required padding)
 * @note sets errno = EOVERFLOW if destlen is too small
 */
ssize_t base64_encode_using_maps(const base64_maps_t *maps,
				 char *dest, size_t destlen,
				 const char *src, size_t srclen);

/*
 * base64_char_in_alphabet - returns true if character can be part of an encoded string
 * @param maps A base64 maps object (see base64_init_maps)
 * @param b64char Character to check
 */
bool base64_char_in_alphabet(const base64_maps_t *maps, char b64char);

/**
 * base64_decode_using_maps - decode a base64-encoded string using a specific alphabet
 * @param maps A base64 maps object (see base64_init_maps)
 * @param dest Buffer to decode into
 * @param destlen length of dest
 * @param src the buffer to decode
 * @param srclen the length of the data to decode
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 * @note sets errno = EOVERFLOW if destlen is too small
 * @note sets errno = EDOM if src contains invalid characters
 */
ssize_t base64_decode_using_maps(const base64_maps_t *maps,
				 char *dest, size_t destlen,
				 const char *src, size_t srclen);

/**
 * base64_decode_quartet_using_maps - decode 4 bytes from base64 using a specific alphabet
 * @param maps A base64 maps object (see base64_init_maps)
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 bytes
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note sets errno = EDOM if src contains invalid characters
 */
int base64_decode_quartet_using_maps(const base64_maps_t *maps,
				     char dest[3], const char src[4]);

/**
 * base64_decode_tail_using_maps - decode the final bytes of a base64 string using a specific alphabet
 * @param maps A base64 maps object (see base64_init_maps)
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 bytes - padded with '=' as required
 * @param srclen Number of bytes to decode in src
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note sets errno = EDOM if src contains invalid characters
 * @note sets errno = EINVAL if src is an invalid base64 tail
 */
int base64_decode_tail_using_maps(const base64_maps_t *maps, char *dest,
				  const char *src, size_t srclen);


/* the rfc4648 functions: */

static const base64_maps_t base64_maps_rfc4648;

/**
 * base64_encode - Encode a buffer into base64 according to rfc4648
 * @param dest Buffer to encode into
 * @param destlen Length of the destination buffer
 * @param src Buffer to encode
 * @param srclen Length of the data to encode
 * @return Number of encoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen (past any required padding)
 * @note sets errno = EOVERFLOW if destlen is too small
 *
 * This function encodes src according to http://tools.ietf.org/html/rfc4648
 *
 * Example:
 *	size_t encoded_length;
 *	char dest[100];
 *	const char *src = "This string gets encoded";
 *	encoded_length = base64_encode(dest, sizeof(dest), src, strlen(src));
 *	printf("Returned data of length %zd @%p\n", encoded_length, &dest);
 */
static inline
ssize_t base64_encode(char *dest, size_t destlen,
		      const char *src, size_t srclen)
{
	return base64_encode_using_maps(&base64_maps_rfc4648,
					dest, destlen, src, srclen);
}

/**
 * base64_encode_triplet - encode 3 bytes into base64 according to rfc4648
 * @param dest Buffer containing 4 bytes
 * @param src Buffer containing 3 bytes
 */
static inline
void base64_encode_triplet(char dest[4], const char src[3])
{
	base64_encode_triplet_using_maps(&base64_maps_rfc4648, dest, src);
}

/**
 * base64_encode_tail - encode the final bytes of a source according to rfc4648
 * @param dest Buffer containing 4 bytes
 * @param src Buffer containing srclen bytes
 * @param srclen Number of bytes (<= 3) to encode in src
 */
static inline
void base64_encode_tail(char dest[4], const char *src, size_t srclen)
{
	base64_encode_tail_using_maps(&base64_maps_rfc4648, dest, src, srclen);
}


/**
 * base64_decode - decode An rfc4648 base64-encoded string
 * @param dest Buffer to decode into
 * @param destlen Length of the destination buffer
 * @param src Buffer to decode
 * @param srclen Length of the data to decode
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 * @note sets errno = EOVERFLOW if destlen is too small
 * @note sets errno = EDOM if src contains invalid characters
 *
 * This function decodes the buffer according to
 * http://tools.ietf.org/html/rfc4648
 *
 * Example:
 *	size_t decoded_length;
 *	char ret[100];
 *	const char *src = "Zm9vYmFyYmF6";
 *	decoded_length = base64_decode(ret, sizeof(ret), src, strlen(src));
 *	printf("Returned data of length %zd @%p\n", decoded_length, &ret);
 */
static inline
ssize_t base64_decode(char *dest, size_t destlen,
		      const char *src, size_t srclen)
{
    return base64_decode_using_maps(&base64_maps_rfc4648,
					dest, destlen, src, srclen);
}

/**
 * base64_decode_quartet - decode the first 4 characters in src into dest
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 characters
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note sets errno = EDOM if src contains invalid characters
 */
static inline
int base64_decode_quartet(char dest[3], const char src[4])
{
	return base64_decode_quartet_using_maps(&base64_maps_rfc4648,
						dest, src);
}

/**
 * @brief decode the final bytes of a base64 string from src into dest
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 bytes - padded with '=' as required
 * @param srclen Number of bytes to decode in src
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note sets errno = EDOM if src contains invalid characters
 * @note sets errno = EINVAL if src is an invalid base64 tail
 */
static inline
ssize_t base64_decode_tail(char dest[3], const char *src, size_t srclen)
{
	return base64_decode_tail_using_maps(&base64_maps_rfc4648,
					     dest, src, srclen);
}

/* end rfc4648 functions */

/**
 * base64_maps_rfc4648 - pregenerated maps struct for rfc4648
 */
static const base64_maps_t base64_maps_rfc4648 = {
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",

  "\xff\xff\xff\xff\xff" /* 0 */                    \
  "\xff\xff\xff\xff\xff" /* 5 */                    \
  "\xff\xff\xff\xff\xff" /* 10 */                   \
  "\xff\xff\xff\xff\xff" /* 15 */                   \
  "\xff\xff\xff\xff\xff" /* 20 */                   \
  "\xff\xff\xff\xff\xff" /* 25 */                   \
  "\xff\xff\xff\xff\xff" /* 30 */                   \
  "\xff\xff\xff\xff\xff" /* 35 */                   \
  "\xff\xff\xff\x3e\xff" /* 40 */                   \
  "\xff\xff\x3f\x34\x35" /* 45 */                   \
  "\x36\x37\x38\x39\x3a" /* 50 */                   \
  "\x3b\x3c\x3d\xff\xff" /* 55 */                   \
  "\xff\xff\xff\xff\xff" /* 60 */                   \
  "\x00\x01\x02\x03\x04" /* 65 A */                 \
  "\x05\x06\x07\x08\x09" /* 70 */                   \
  "\x0a\x0b\x0c\x0d\x0e" /* 75 */                   \
  "\x0f\x10\x11\x12\x13" /* 80 */                   \
  "\x14\x15\x16\x17\x18" /* 85 */                   \
  "\x19\xff\xff\xff\xff" /* 90 */                   \
  "\xff\xff\x1a\x1b\x1c" /* 95 */                   \
  "\x1d\x1e\x1f\x20\x21" /* 100 */                  \
  "\x22\x23\x24\x25\x26" /* 105 */                  \
  "\x27\x28\x29\x2a\x2b" /* 110 */                  \
  "\x2c\x2d\x2e\x2f\x30" /* 115 */                  \
  "\x31\x32\x33\xff\xff" /* 120 */                  \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" /* 125 */          \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" /* 155 */          \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" /* 185 */          \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" /* 215 */          \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"                \
  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" /* 245 */
};

#endif /* CCAN_BASE64_H */
