#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *fp=fopen("data.csv","w");
    if(!fp) { 
        puts("can't write file"); 
        return 1; 
    }
    float data_size=0;
    printf("input data_size(MB):");
    if(!(scanf("%f", &data_size)==1))
        printf("can't input integer");

    float size=0;
    srand(time(NULL));
    while(size<data_size*1000000){
        int *data=calloc(200,sizeof(int));
        for(int t=0;t<10;t++){
            for(int i=0;i<20;i++){
                int PN_seed=(rand() % 10) +1;
                int data_number=(rand() % 2147483647)+1;
                if(PN_seed>5)
                    PN_seed=-1;
                else
                    PN_seed=1;
                data[i]=data_number*PN_seed;
                if(i!=19)
                    fprintf(fp,"%d|",data[i]);
                else if(i==19)
                    fprintf(fp,"%d\n",data[i]);
            }
        }
        free(data);
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
    }
    fclose(fp);
    system("iconv -f ascii -t UTF8 -o input.csv data.csv");
    system("rm data.csv");
}