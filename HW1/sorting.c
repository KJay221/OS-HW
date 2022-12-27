#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmp(const void* a, const void* b){
  int va = *(const int*) a;
  int vb = *(const int*) b;
  return (va > vb) - (va < vb);
}

void merge(int start_number,int file_number);

int main(){
    int input_size=0;
    int page_size=0;
    printf("input page_size(one unit is 100MB):");
    if(!(scanf("%d", &input_size)==1))
        printf("can't input integer");
    page_size=input_size*9105500; //100MB

    //count time
    clock_t start_t,finish_t;
    start_t = clock();

    //open file
    FILE *fp=fopen("input.txt","r");
    if(!fp) { 
        puts("can't write file"); 
        return 1; 
    }

    //split file to small one
    int sub_file_number=0;
    int jumpout=0;
    int is_zero=0;
    
    while(1){
        int *data=calloc(page_size,sizeof(int));//each file integer numbers
        int size=0;
        for(int i=0;i<page_size;i++){
            char str[20];
            if(fgets (str, 20, fp)==NULL){
                jumpout=1;
                if(i==0)
                    is_zero=1;
                break;
            } 
            data[size]=atoi(str);
            size++;
        }
        if(is_zero)
            break;
        qsort(data, size, sizeof(int), cmp);
        //write into sub_file
        char sub_file[15]={'0'};
        snprintf(sub_file, sizeof(char)*15,"%d.txt",sub_file_number);
        sub_file_number++;
        FILE *sub_file_pt=fopen(sub_file,"w");
        if(!sub_file_pt) { 
            puts("can't write file"); 
            return 1; 
        }
        for(int i=0;i<size;i++)
            fprintf(sub_file_pt,"%d\n",data[i]);
        fclose(sub_file_pt);
        free(data);
        if(jumpout)
            break;
    }
    fclose(fp);
    if(sub_file_number!=1)
        merge(0,sub_file_number);
    else
        system("mv 0.txt output.txt");

    //count time
    finish_t = clock();
    double time=(double)(finish_t-start_t)/CLOCKS_PER_SEC;//count time
    int hour=time/3600;
    int min=((int)time%3600)/60;
    int second=(int)time%60;
    printf("hour:%d\nmin:%d\nsecond:%d\n",hour,min,second);
}

void merge(int start_number,int file_number){
    int odd=0;
    if(file_number%2==1){
        odd=1;
        file_number--;
    }
    for(int i=start_number;i<start_number+file_number;i=i+2){
        //open sub_file
        char sub_file_1[12]={'0'};
        char sub_file_2[12]={'0'};
        char sub_file_data[12]={'0'};
        snprintf(sub_file_1, sizeof(char)*12,"%d.txt",i);
        snprintf(sub_file_2, sizeof(char)*12,"%d.txt",i+1);
        if(odd)
            snprintf(sub_file_data, sizeof(char)*12,"%d.txt",start_number+file_number+(i-start_number)/2+1);
        else{
            if(file_number/2==1)
                snprintf(sub_file_data, sizeof(char)*12,"%s.txt","output");
            else
                snprintf(sub_file_data, sizeof(char)*12,"%d.txt",start_number+file_number+(i-start_number)/2);
        }    
        FILE *file1_pt=fopen(sub_file_1,"r");
        FILE *file2_pt=fopen(sub_file_2,"r");
        FILE *file_data_pt=fopen(sub_file_data,"w");
        int file1_read=1;
        int file2_read=1;
        char file1_str[20];
        char file2_str[20];
        int file1_number;
        int file2_number;
        int count=0;
        
        //read sub_file and sort
        while(1){
            count++;
            if(file1_read){
                if(fgets (file1_str, 20, file1_pt)==NULL){
                    fprintf(file_data_pt,"%d\n",file2_number);
                    while(fgets (file2_str, 20, file2_pt)!=NULL){
                        file2_number=atoi(file2_str);
                        fprintf(file_data_pt,"%d\n",file2_number);
                    }
                    break;
                }
            } 
            if(file2_read){
                if(fgets (file2_str, 20, file2_pt)==NULL){
                    fprintf(file_data_pt,"%d\n",file1_number);
                    while(fgets (file1_str, 20, file1_pt)!=NULL){
                        file1_number=atoi(file1_str);
                        fprintf(file_data_pt,"%d\n",file1_number);
                    }
                    break;
                }
            } 
            file1_number=atoi(file1_str);
            file2_number=atoi(file2_str);
            if(file1_number>file2_number){
                fprintf(file_data_pt,"%d\n",file2_number);
                file2_read=1;
                file1_read=0;
            }
            else if(file1_number<=file2_number){
                fprintf(file_data_pt,"%d\n",file1_number);
                file1_read=1;
                file2_read=0;
            }
        }
        fclose(file1_pt);
        fclose(file2_pt);
        fclose(file_data_pt);
    }
    for(int i=start_number;i<start_number+file_number;i++){
        char file_delete_number[20]={'0'};
        snprintf(file_delete_number, sizeof(char)*12,"rm %d.txt",i);
        system(file_delete_number);
    }
    if(odd)
        file_number++;
    if(file_number/2>1 || (odd==1&&file_number/2>0))  //might have problem
        merge(start_number+file_number-odd,file_number/2+odd);
}