#include "base32.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE;

static BYTE map_char(char c)
{
  if (c >= 'A' && c <= 'Z')
    return c - 'A';

  if (c >= '2' && c <= '7')
    return c - '2' + 26;

  return 0xFF;
}

int base32_decode(const char *input, char *output, size_t limit)
{
  if (input == NULL)
    return BASE32_INCORRECT_INPUT;

  if (output == NULL)
    return BASE32_INCORRECT_OUTPUT;

  int n = 0;
  int pos = 0;
  bool end = false;

  memset(output, 0, limit);

  while (*input != '\0' && !end)
  {
    BYTE buffer[8];
    int dlen = 8;

    for (int i = 0; i < 8;)
    {
      char c = *input;

      if (c == '\0')
        return BASE32_INCORRECT_PADDING;

      input++;

      if (isspace(c))
        continue;

      size_t len = strlen(input);
      if (c == '=' && i >= 2 && len < 8)
      {
        if ((len) + i < 8 - 1)
          return BASE32_INCORRECT_PADDING;

        for (int j = 0; j < 8 - 1 - i; j++)
        {
          if (len > j && input[j] != '=')
            return BASE32_INCORRECT_PADDING;
        }

        dlen = i;
        end = true;

        if (dlen == 1 || dlen == 3 || dlen == 6)
          return BASE32_INCORRECT_PADDING;

        break;
      }

      buffer[i] = map_char(c);
      if (buffer[i] == 0xFF)
        return BASE32_INCORRECT_SYMBOL;

      i++;
    }

    switch (dlen)
    {
    case 8:
      output[pos + 4] = (buffer[6] << 5) | buffer[7];
      n++;

    case 7:
      output[pos + 3] = (buffer[4] << 7) | (buffer[5] << 2) | (buffer[6] >> 3);
      n++;

    case 5:
      output[pos + 2] = (buffer[3] << 4) | (buffer[4] >> 1);
      n++;

    case 4:
      output[pos + 1] = (buffer[1] << 6) | (buffer[2] << 1) | (buffer[3] >> 4);
      n++;

    case 2:
      output[pos + 0] = (buffer[0] << 3) | (buffer[1] >> 2);
      n++;
    }

    pos += 5;
  }

  return n;
}
