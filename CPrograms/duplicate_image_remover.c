#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <dirent.h>
#include <string.h>
#include <uthash.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

struct Duplicate{
    const char *name;
    unsigned int originalindex;
};

struct Original{
    const char *name;
    unsigned char *hash;
};

void printHash(unsigned char* digest){
    for(int i=0;i<MD5_DIGEST_LENGTH;i++){
        printf("%02X", digest[i]);
    }
    printf("\n");
}

unsigned char* calculateHash(const char* file_name){
    FILE *file = fopen(file_name, "rb");
    
    if(!file){
        perror("Error Opening file");
        return (unsigned char*)file;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    // Reset to beginning for MD5 use
    fseek(file, 0, 0);


    unsigned char *digest_buffer = malloc(MD5_DIGEST_LENGTH);
    unsigned char chunk[BUFFER_SIZE];

    MD5_CTX ctx;
    MD5_Init(&ctx);

    size_t read_bytes; 
    while((read_bytes = fread(chunk, 1, BUFFER_SIZE, file)) > 0){
        MD5_Update(&ctx, chunk, read_bytes);
    }
    MD5_Final(digest_buffer, &ctx); 

    fclose(file);

    return digest_buffer;
}



_Bool checkIfImage(const char* filename){
    unsigned char buf[8];
    FILE *file = fopen(filename, "rb");
    
    // printf("Evaluating file: %s", filename);
    if(fread(buf, 1, sizeof(buf), file) < sizeof(buf)){
        // printf("\t Error reading entry\n");
        fclose(file);
        return false;
    }
    if(memcmp(buf, "\xFF\xD8\xFF", 3) == 0){
        // printf(" [ Valid JPEG image ]\n");
        return true;
    }
    // printf("\n");
    return false;
}


int containsHash(unsigned char* hash, struct Original *digests, int size_of_digest){
    while(size_of_digest--){
        if(memcmp(hash, digests[size_of_digest].hash, MD5_DIGEST_LENGTH) == 0)
            return size_of_digest;
    }
    return -1;
}

int main(int argc, char** argv){
    struct dirent **namelist;
    unsigned int MAX_FILES_TO_DIGEST = scandir(".", &namelist, NULL, alphasort);
    DIR *dir = opendir(".");
    if(!dir || MAX_FILES_TO_DIGEST < 0){
        perror("Cannot open directory");
        return 1;
    }

    struct Duplicate duplicates[MAX_FILES_TO_DIGEST];
    struct Original originals[MAX_FILES_TO_DIGEST];

    struct dirent *entry;
    int i=0, dupptr=0,origptr=0;
    
    while((entry = readdir(dir)) != NULL){
        if(checkIfImage(entry->d_name) == 1){
            // printf("Digesting: %s\n", entry->d_name);

            unsigned char *hash = calculateHash(entry->d_name);
            // printf("Hash calculated: ");
            // printHash(hash);

            int index = containsHash(hash, originals, origptr);
            if(index != -1){
               duplicates[dupptr].name = entry->d_name;
               duplicates[dupptr].originalindex = index;//Index of original;
               dupptr++;
            }else{
            // If there is not, store it in original array
               originals[origptr].name = entry->d_name;
               originals[origptr].hash = hash;
               origptr++;
            }
            i++;
        }
    }

    for(int i=1;i<argc;i++){
            if(strcmp(argv[i],"--help") == 0){
                printf("\
                        Remove duplicate JPEG images\n\
                        Usage: checkDuplicates [directory] [flags]\n\
                        Flags:\n\
                        --help          : Show this help\n\
                        --remove        : Remove all duplicates\n\
                        --show          : Show all duplicates and corresponding originals\n");
            }else{
                if(strcmp(argv[i],"--show") == 0){
                    printf("Duplicates\n");
                    for(int i=0;i<dupptr;i++){
                        printf("%s ----> %s\n", duplicates[i].name, originals[duplicates[i].originalindex].name);
                    }
                }
                if(strcmp(argv[i],"--remove") == 0){
                    for(int i=0;i<dupptr;i++){
                        remove(duplicates[i].name);
                    }
                    printf("Removed %d files\n", i);
                }
            }
    } 

    return 0;
}
