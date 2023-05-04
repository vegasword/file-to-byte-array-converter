#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    FILE *fileptr;
	char *name;
    char *buffer;
	int namelen;
    long filelen;
    size_t result;

    for (int i=1; i < argc; i++)
    {
        fileptr = fopen(argv[i], "rb");
        if (fileptr == NULL) {
            printf("Error: Could not open file.\n");
            return 1;
        }
		
        fseek(fileptr, 0, SEEK_END);
        filelen = ftell(fileptr);
        rewind(fileptr);
        buffer = (char *)malloc(filelen * sizeof(char));
        if (buffer == NULL) {
            printf("Error: Could not allocate memory.\n");
            return 1;
        }
		memset(buffer, '\0', filelen);
		
        result = fread(buffer, sizeof(char), filelen, fileptr);
        if (result != filelen) {
            printf("Error: Could not read file.\n");
            return 1;
        }
        fclose(fileptr);
		
		namelen = strcspn(argv[i], ".");
		char *name = (char *)malloc(namelen * sizeof(char));
		if (name == NULL) {
            printf("Error: Could not allocate memory.\n");
            return 1;
        }
		memset(name, '\0', namelen);
		strncpy(name, argv[i], namelen);
		
        printf("unsigned char %s[] = {\n", name);
        for (int i = 0; i < filelen; i++) {
            printf("0x%02X%c", (unsigned char)buffer[i], i == filelen - 1 ? '\0' : ',');
            if (i % 16 == 15) {
                printf("\n");
            }
        }
        printf("};\n");
		free(name);
        free(buffer);
    }

    return 0;
}