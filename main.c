#include <stdio.h>
#include <stdlib.h>
#define LEN 12

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Usage: ws-patch /path/to/file.\n");
    return 1;
  }
  char *file_name = argv[1];
  FILE *fp;
  long file_size = 0;
  char *data = NULL;
  fp = fopen(file_name, "r+");
  if (NULL == fp)
  {
    printf("File: %s not found.", file_name);
    return 2;
  }
  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  data = malloc(file_size);
  if (NULL == data)
  {
    printf("Failed to alloc space for file.\n");
    return 3;
  }
  fseek(fp, 0, SEEK_SET);
  fread(data, 1, file_size, fp);
  char sign[] = {'\x3D', '\xB7', '\x00', '\x00', '\x00', '\x74', '?', '\x83', '\xF8', '\x05', '\x74', '?'};
  char nop = '\x90';
  char bytes[] = {
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
      nop,
  };

  long start = 0;
  for (long i = 0; i < file_size; i++)
  {
    long curr_start = i;
    long j = 0;
    for (; j < LEN; j++)
    {
      if (sign[j] == '?')
      {
        continue;
      }
      char b = data[curr_start + j];
      if (sign[j] == b)
      {
        continue;
      }
      else
      {
        break;
      }
    }
    if (LEN == j)
    {
      start = curr_start;
      break;
    }
  }

  if (start > 0)
  {
    fseek(fp, start, SEEK_SET);
    fwrite(bytes, LEN, 1, fp);
    fclose(fp);
    free(data);
    printf("Patch success!\n");
    return 0;
  }
  else
  {
    fclose(fp);
    free(data);
    printf("Signature not found.\n");
    return 4;
  }
}