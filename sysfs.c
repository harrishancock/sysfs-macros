#include "sysfs.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
  //FILE* fp = popen(FROM("/sys/devices") SELECT SUBSYSTEM("tty"), "r");
  FILE* fp = popen(FROM("/sys/devices") SELECT SUBSYSTEM("usb") SELECTUP SUBSYSTEM("pci"), "r");

  if (!fp) {
    printf("popen() failed\n");
    return 1;
  }

  char* device = NULL;
  size_t n = 0;
  while (-1 != getdelim(&device, &n, '\0', fp)) {
    printf("%s\n", device);
  }

  free(device);

  if (ferror(fp)) {
    printf("error while reading\n");
    return 1;
  }

  if (-1 == pclose(fp)) {
    printf("pclose() failed\n");
    return 1;
  }
}
