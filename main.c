#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

int filesize(char *filePath) {
    struct stat details;

    const int status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    assert(status!=0);
    return -1;
}
 void readInputFile(char *filePath)
{
    const fileLength=filesize(filePath);
    const FILE* file=fopen(filePath,"r");

    fclose(file);
}

int getNetworkStats(FILE* file){

}

int main() {
    int s = filesize("/home/eran/CLionProjects/tau_swp_pca/m2ain.c");
    printf("Hello, World!\n");
    return 0;
}
