#include "base32.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  // TODOs
  // [x] Implement base32 decoding
  // [ ] Implement get unix timestamp
  // [ ] Encode time
  // [ ] Generate HMAC digest
  char *secret = "NBSWU43LNJUGI2TTMRQXGZDEONQXGZA=";
  unsigned int bytes = 20;
  size_t limit = sizeof(uint8_t) * bytes;
  BYTE *output = malloc(limit);

  int res = base32_decode(secret, output, limit);

  if (res < 0)
    return res;

  printf("%s decoded is: 0x", secret);

  for (unsigned int i = 0; i < bytes; i++)
    printf("%02x", output[i]);

  printf("\n");

  free(output);
  return 0;
}