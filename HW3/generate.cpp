#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <fstream>
using namespace std;

int main(){
    string file_name="";
    float data_size=0;
    int scan_range=1;

    cout<<"please enter file name: ";
    cin>>file_name;
    file_name+=".input";
    FILE *fp=fopen(file_name.c_str(),"w");
    if(!fp) { 
        puts("can't write file"); 
        return 1;
    }
    //cin data size
    cout<<"input data_size(MB):";
    cin>>data_size;

    //cin scan max range
    while(scan_range<2){
        cout<<"input scan max range(bigger than one):";
        cin>>scan_range;
    }

    default_random_engine generator(time(NULL));                            
    uniform_int_distribution<int> instruction_generator(0,2);
    uniform_int_distribution<unsigned long long> number_generator(0,9223372036854775807);
    uniform_int_distribution<int> string_generator(48,122);
    uniform_int_distribution<int> range_generator(2,scan_range);

    float size=0;
    while(size<data_size*1000000){
        for(int line=0;line<1000;line++){
            int random_instruction=instruction_generator(generator);
            string instruction="";
            string p1="";
            string p2="";
            string output_instruction="";
            if(random_instruction==0){
                instruction="PUT";
                p1=to_string(number_generator(generator));
                for(int i=0;i<128;i++){
                    char output_char=0;
                    do{
                        output_char=string_generator(generator);
                    }while((output_char>57 && output_char<65)||(output_char>90 && output_char<97));
                    p2+=output_char;
                }
                output_instruction=instruction+" "+p1+" "+p2+"\n";
                fprintf(fp,"%s",output_instruction.c_str());
            }   
            else if(random_instruction==1){
                instruction="GET";
                p1=to_string(number_generator(generator));
                output_instruction=instruction+" "+p1+"\n";
                fprintf(fp,"%s",output_instruction.c_str());
            }
            else if(random_instruction==2){
                instruction="SCAN";
                int range=range_generator(generator);
                long long int start_number=0;
                do{
                    start_number=number_generator(generator);
                }while((start_number+range)>9223372036854775807);
                p1=to_string(start_number);
                p2=to_string(start_number+range);
                output_instruction=instruction+" "+p1+" "+p2+"\n";
                fprintf(fp,"%s",output_instruction.c_str());
            }
        }
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
    }
    fclose(fp);
}