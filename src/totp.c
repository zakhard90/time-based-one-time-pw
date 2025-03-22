#include "base32.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

static time_t unix_time()
{
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  return time.tv_sec;
}

static void encode_time_step(uint64_t step, BYTE *destination)
{
  for (int i = 0; i < 8; i++)
  {
    destination[i] = (step >> (7 - i) * 8) & 0xFF;
  }
}

int main(void)
{
  // TODOs
  // [x] Implement base32 decoding
  // [x] Implement get unix timestamp
  // [x] Encode time
  // [x] Generate HMAC digest
  // [x] Calculate TOTP
  // [ ] Read secret from an .env file
  char *secret = "J5XW6Y3LORUGS5DPNVZHK3TF";
  unsigned int bytes = 20;
  size_t limit = sizeof(BYTE) * bytes;
  BYTE output_buffer[limit];

  int res = base32_decode(secret, output_buffer, limit);

  if (res < 0)
    return res;

  printf("%s decoded is: 0x", secret);

  for (unsigned int i = 0; i < bytes; i++)
    printf("%02x ", output_buffer[i]);

  printf("\n");

  uint64_t time_step = unix_time() / 30;

  BYTE time_buffer[8];
  encode_time_step(time_step, time_buffer);
  printf("%ld\n", time_step);

  for (unsigned int i = 0; i < 8; i++)
    printf("%02x ", time_buffer[i]);
  printf("\n");

  BYTE digest_buffer[20];
  unsigned int length;
  HMAC(EVP_sha1(), output_buffer, 20, time_buffer, 8, digest_buffer, &length);

  for (unsigned int i = 0; i < 20; i++)
    printf("%02x ", digest_buffer[i]);
  printf("\n");

  int offset = digest_buffer[19] & 0xF;
  uint32_t binary = (digest_buffer[offset + 0] & 0x7f) << 24 |
                    digest_buffer[offset + 1] << 16 |
                    digest_buffer[offset + 2] << 8 |
                    digest_buffer[offset + 3];

  int otp = binary % 1000000;

  printf("%06d\n", otp);
  return 0;
}