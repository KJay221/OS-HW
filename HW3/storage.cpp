#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
    //count time
    clock_t start_t,finish_t;
    start_t = clock();

    //input filename
    string file_path=argv[1];
    
    //output filename
    string outpu_filename=file_path;
    size_t pos = 0;
    while((pos = outpu_filename.find("/")) != string::npos)
        outpu_filename.erase(0, pos + 1);
    pos = outpu_filename.find(".");
    outpu_filename.erase(pos,string::npos);
    outpu_filename+=".output";

    //open input file
    fstream file;
    file.open(file_path, ios::in);
    //file.open("./1.input", ios::in);
    if(!file){
        cout<<"can't open file."<<endl;
        return 0;
    }

    //read instruction
    map<unsigned long long int,string> database;
    string output_string="";
    FILE *output_pt=NULL;
    string input="";
    bool used_disk=false;
    while(getline(file, input)){
        //8GB: 50000000 map.size
        if(database.size()>2000){
            used_disk=true;
            string shell_command="./mkdir.sh storage";
            system(shell_command.c_str());
            //100MB: 650000
            for(int i=0;i<1000;i++){
                long long int index=0;
                string file_string="storage/";
                index=database.begin()->first;
                long long int compare_bit=-72057594037927936;
                for(int j=7;j>=1;j--){
                    file_string+=to_string((index&compare_bit)>>(j*8));
                    shell_command="./mkdir.sh "+file_string;
                    system(shell_command.c_str());
                    if(j!=1)
                        file_string+="/";
                    if(j != 7)
                        compare_bit>>=8;
                    else
                        compare_bit=71776119061217280;

                }
                database.erase(database.begin());
            }
        }

        //split instruction
        string instruction = "";
        string p1 = "";
        string p2 = "";
        pos = 0;
        for(int i=0;i<2;i++){
            if((pos = input.find(" ")) == string::npos)
                break;
            if(i==0)
                instruction = input.substr(0, pos);
            else if(i==1)
                p1 = input.substr(0, pos);
            input.erase(0, pos + 1);
        }
        (instruction=="PUT" || instruction=="SCAN")? p2=input : p1=input;
        
        //deal instruction
        long long int p1_int=stoll(p1);
        if(instruction == "PUT"){
            database[p1_int]=p2;
        }
        else if(instruction == "GET"){
            if(database.find(p1_int) != database.end())
                output_string+=(database[p1_int]+"\n");
            else{
                if(!used_disk)
                    output_string+="EMPTY\n";
                else{

                }
            }   
        }
        else if(instruction == "SCAN"){
            long long int p2_int=stoll(p2);
            for(int i=0;i <= p2_int-p1_int;i++){
                if(database.find(p1_int+i) != database.end())
                    output_string+=(database[p1_int+i]+"\n");
                else{
                    output_string+="EMPTY\n";
                }
            }
        }

        //if output_string is full write out
        if(output_string.length()>4000000000){
            output_pt=fopen(outpu_filename.c_str(),"w");
            if(!output_pt)
                puts("can't write file");
            else{
                fprintf(output_pt,"%s",output_string.c_str());
                output_string="";
                fclose(output_pt);
            }
        }
    }

    //end of input and write file
    output_pt=fopen(outpu_filename.c_str(),"w");
    if(!output_pt)
        puts("can't write file");
    else{
        fprintf(output_pt,"%s",output_string.c_str());
        output_string="";
        fclose(output_pt);
    }

    //count time
    finish_t = clock();
    float second=(float)(finish_t-start_t)/CLOCKS_PER_SEC;
    cout<<"second: "<<second<<endl;
}
