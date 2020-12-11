#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main(int argc, char *argv[]){
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
    while(getline(file, input)){
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
            else
                output_string+="EMPTY\n";
        }
        else if(instruction == "SCAN"){
            long long int p2_int=stoll(p2);
            for(int i=0;i <= p2_int-p1_int;i++){
                if(database.find(p1_int+i) != database.end())
                    output_string+=(database[p1_int+i]+"\n");
                else
                    output_string+="EMPTY\n";
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
}