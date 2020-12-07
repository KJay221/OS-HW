#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char filename[20];
    printf("which file you want to check:");
    fgets(filename,20,stdin);
    strtok(filename,"\n");
    
    //open file
    FILE *fp=fopen(filename,"r");
    if(!fp) { 
        puts("can't write file"); 
        return 1; 
    }
    int size=0;
    int first=0;
    int front=0;
    int end=0;
    int is_correct=1;
    
    while(1){
        char str[20];
        if(fgets (str, 20, fp)==NULL)
            break;
        end=atoi(str);
        if(first==0){
            first=1;
            front=end;
        }
        else
            if(end<front){
                printf("wrong:%d\n",size);
                is_correct=0;
            }
        size++;
        front=end;
    }
    fclose(fp);
    if(is_correct)
        printf("It is correct.\n");
    printf("number of integer:%d\n",size);
}