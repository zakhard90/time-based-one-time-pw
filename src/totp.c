#include "base32.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#define L_BYTES 20
#define S_BYTES 8
#define T_DURATION 30
#define ENV_PATH ".env"
#define ENV_KEY "SECRET"

static time_t unix_time();
static void encode_time_step(uint64_t step, BYTE *destination);
static int env_value(const char *env_path, const char *env_key, char *buffer);

int main(void)
{
  size_t limit = sizeof(BYTE) * L_BYTES;
  BYTE output_buffer[limit];
  char *secret_buffer = malloc(sizeof(char) * 40);
  char *env_path = ENV_PATH;
  char *env_key = ENV_KEY;

  int read = env_value(env_path, env_key, secret_buffer);

  if (read < 0)
    return read;

  int res = base32_decode(secret_buffer, output_buffer, limit);

  if (res < 0)
    return res;

  uint64_t time_step = unix_time() / T_DURATION;

  BYTE time_buffer[S_BYTES];
  encode_time_step(time_step, time_buffer);

  BYTE digest_buffer[L_BYTES];
  unsigned int length;
  HMAC(EVP_sha1(), output_buffer, L_BYTES, time_buffer, S_BYTES, digest_buffer, &length);

  int offset = digest_buffer[L_BYTES - 1] & 0xF;
  uint32_t binary = (digest_buffer[offset + 0] & 0x7f) << 24 |
                    digest_buffer[offset + 1] << 16 |
                    digest_buffer[offset + 2] << 8 |
                    digest_buffer[offset + 3];

  int otp = binary % 1000000;

  printf("Your OTP is: %06d\n", otp);
  free(secret_buffer);
  return 0;
}

static time_t unix_time()
{
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  return time.tv_sec;
}

static void encode_time_step(uint64_t step, BYTE *destination)
{
  for (int i = 0; i < S_BYTES; i++)
  {
    destination[i] = (step >> (7 - i) * S_BYTES) & 0xFF;
  }
}

static int env_value(const char *env_path, const char *env_key, char *buffer)
{
  FILE *file = fopen(env_path, "r");
  if (file == NULL)
  {
    return -1;
  }

  char line[512];

  while (fgets(line, sizeof(line), file))
  {
    int p = 0;
    int v = 0;
    while (line[p] != '=')
    {
      if (line[p] != env_key[p])
      {
        p = 0;
        break;
      }
      p++;
    }
    p++;

    while (line[p] != '\n')
    {
      // Allow CRLF
      if (line[p] == '\r')
      {
        p++;
        continue;
      }

      buffer[v++] = line[p++];
    }

    break;
  }

  fclose(file);
  return 0;
}