#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

typedef struct {
    int is_end;
    int str_number;
    FILE* fp_in;
    int one_size;
} read_p;

typedef struct {
    char **str;
    int str_number;
    pthread_t last_thread;
    int is_first;
    int is_end;
} write_p;

void* read(void* input) {
    read_p *r_input=(read_p *)input;
    int data_size=r_input->one_size; //each thread handle number

    //read file
    char **str = (char **) calloc(data_size, sizeof(char*));
    for(int i=0;i<data_size;i++)
        str[i] = (char *) calloc(300, sizeof(char));
    for(int i=0;i<data_size;i++){
        if(fgets(str[i],300,r_input->fp_in)==NULL){
            r_input->is_end=1;
            break;
        }
        else
            r_input->str_number++;
    }
    return (void *)str;
};

void* write(void* input) {
    write_p *w_input=(write_p *)input;
    if(w_input->is_first==0)
        pthread_join(w_input->last_thread, NULL);
    FILE *fp=fopen("output.json","a");
    if(!fp) { 
        puts("can't write file"); 
        return NULL; 
    }
    if(w_input->is_first==1)
        fprintf(fp,"[\n");
    for(int i = 0; i < w_input->str_number ; i++){
        char *p = strtok (w_input->str[i], "|");
        char *array[20];
        for(int i=0;i<20;i++){
            array[i]=p;
            p=strtok(NULL, "|");
        }
        fprintf(fp,"  {\n");
        for(int i=0;i<20;i++){
            if(i<19)
                fprintf(fp,"    \"col_%d\":%s,\n",i+1,array[i]);
            else
                fprintf(fp,"    \"col_%d\":%s",i+1,array[i]);//last number have a \n
        }
        if(w_input->is_end==1 && i+1==w_input->str_number)
            fprintf(fp,"  }\n]");
        else
            fprintf(fp,"  },\n");
    }
    fclose(fp);
    return NULL;
};

int main(int argc, char *argv[]) {
    //count time
    clock_t start_t,finish_t;
    start_t = clock();

    int thread_number=0;
    thread_number=atoi(argv[1]);
    FILE *fp_size=fopen("input.csv","r");
    if(!fp_size) { 
        puts("can't write file"); 
        return 1; 
    }
    long long int file_size=0;
    fseek(fp_size, 0L, SEEK_END);
    file_size = (long long int)ftell(fp_size)/1000000.0;
    fclose(fp_size);
    file_size*=4600;
    file_size/=thread_number;
    file_size+=100;
    int stop_number=0; //which thread is stop(array index)
    
    //thread
    pthread_t* read_thread=calloc(thread_number,sizeof(pthread_t));
    pthread_t* write_thread=calloc(thread_number,sizeof(pthread_t));
    void **output=calloc(thread_number,sizeof(void*));
    read_p *r_input=calloc(thread_number,sizeof(read_p));
    for(int i=0;i<thread_number;i++){
        r_input[i].fp_in=NULL;
        r_input[i].is_end=0;
        r_input[i].str_number=0;
        r_input[i].one_size=file_size;
    }
    write_p *w_input=calloc(thread_number,sizeof(write_p));
    for(int i=0;i<thread_number;i++){
        w_input[i].str=NULL;
        w_input[i].str_number=0;
        if(i==0)
            w_input[i].is_first=1;
        else  
            w_input[i].is_first=0;      
    }

    r_input[0].fp_in=fopen("input.csv","r");
    if(!r_input[0].fp_in) { 
        puts("can't write file"); 
        return 1; 
    }

    for(int i=0;i<=thread_number+1;i++){
        if(i==0)
            pthread_create(&read_thread[i], NULL, read, &r_input[i]);
        else{
            pthread_join(read_thread[i-1], &output[i-1]);
            w_input[i-1].str=(char **)output[i-1];
            w_input[i-1].str_number=r_input[i-1].str_number;
            w_input[i-1].is_end=r_input[i-1].is_end;
            if(i>1)
                w_input[i-1].last_thread=write_thread[i-2];
            pthread_create(&write_thread[i-1], NULL, write, &w_input[i-1]);
            if(r_input[i-1].is_end==0){
                r_input[i].fp_in=r_input[i-1].fp_in;
                pthread_create(&read_thread[i], NULL, read, &r_input[i]);  
            }
            else if(r_input[i-1].is_end==1){
                fclose(r_input[i-1].fp_in);
                stop_number=i-1;
                break;
            }    
        }
    }
    pthread_join(write_thread[stop_number], NULL);
    //count time
    finish_t = clock();
    double time=(double)(finish_t-start_t)/CLOCKS_PER_SEC;//count time
    double second=time;
    printf("second:%f\n",second);
    for(int i=0;i<=stop_number;i++){
        for(int j=0;j<w_input[i].str_number;j++)
            free(w_input[i].str[j]);
        free(w_input[i].str);
    }
    free(read_thread);
    free(write_thread);
    free(r_input);
    free(w_input);
}