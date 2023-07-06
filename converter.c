#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  for(int i = 1; i < argc; i++) {
    FILE *inFile = fopen(argv[i], "rb");
    if (!inFile) {
      sprintf(stderr, "Error: Could not open %s.\n", argv[i]);
      return 1;
    }
    fseek(inFile, 0, SEEK_END);
    size_t fileSize = ftell(inFile);
    rewind(inFile);
    char *buffer = (char *)malloc(fileSize);
    if (!buffer) {
      sprintf(stderr, "Error: Could not allocate memory for %s.\n", argv[i]);
      continue;
    }
    size_t result = fread(buffer, 1, fileSize, inFile);
    if (result != fileSize) {
      sprintf(stderr, "Error: Couldn't read entirely %s.\n", argv[i]);
      continue;
    }
		char inFileName[256];
		size_t inFileNameLen = strcspn(argv[i], ".");
		strncpy(inFileName, argv[i], inFileNameLen);
    inFileName[inFileNameLen] = '\0';
		char outFileName[260];
		strcpy(outFileName, inFileName);
		strcat(outFileName, ".txt");
    FILE *outFile = fopen(outFileName, "w");
    fprintf(outFile, "unsigned char %s[] = {\n", inFileName);
    for (int j = 0; j < fileSize; j++) {
      fprintf(outFile, "0x%02X%c", (unsigned char)buffer[j], j == fileSize - 1 ? '\0' : ',');
      if (j % 16 == 15) fprintf(outFile, "\n");
    }
    fprintf(outFile, "};\n");		
    fclose(inFile);
    fclose(outFile);
  }
  return 0;
}
