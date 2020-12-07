#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp=fopen("input.txt","w");
    if(!fp) { 
        puts("can't write file"); 
        return 1; 
    }
    long long int data_size=0;
    printf("input data_size(byte):");
    if(!(scanf("%lld", &data_size)==1))
        printf("can't input integer");
    //1073741824 1GB
    //this size is data size(number of bytes)
    long long int count=0;
    long long int size=0;
    srand(time(NULL));
    while(size<data_size){
        int *data=calloc(1000,sizeof(int));
        for(int i=0;i<1000;i++){
            int PN_seed=(rand() % 10) +1;
            int data_number=(rand() % 2147483647)+1;
            if(PN_seed>5)
                PN_seed=-1;
            else
                PN_seed=1;
            data[i]=data_number*PN_seed;
            fprintf(fp,"%d\n",data[i]);
            count++;
        }
        free(data);
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
    }
    printf("number of integer:%lld\n",count);
    fclose(fp);
}