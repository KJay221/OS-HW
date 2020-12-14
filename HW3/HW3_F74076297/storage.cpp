#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <limits>
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

    //lord last memory data
    map<long long int,string> database;
    string memory_data_path="./storage/memory.data";
    fstream file_memory;
    file_memory.open(memory_data_path, ios::in);
    if(file_memory){
        string memory_data="";
        while(getline(file_memory,memory_data)){
            pos = memory_data.find(" ");
            database[stoll(memory_data.substr(0, pos))]=memory_data.substr(pos+1, string::npos);
        }
        file_memory.close();
    }
    
    
    //open input file
    fstream file;
    file.open(file_path, ios::in);
    if(!file){
        cout<<"can't open file."<<endl;
        return 0;
    }
    
    //use disk
    bool used_disk=false;
    string use_disk_path="./storage/use_disk.data";
    fstream last_disk;
    last_disk.open(use_disk_path, ios::in);
    if(last_disk){
        used_disk=true;
        last_disk.close();
    }

    //read instruction
    string output_string="";
    FILE *output_pt=NULL;
    string input="";
    while(getline(file, input)){
        //--------------------change--------------------
        //8GB: 50000000 map.size
        if(database.size()>200){
            used_disk=true;
            char const *shell_command="./mkdir.sh";
            string file_string="./storage";
            int status;
            if(!fork()){/*child process*/
                execlp(shell_command,shell_command,file_string.c_str(),NULL);
            }    
            else{/*parent process*/
                wait(&status);
            }
            //--------------------change--------------------
            //need smaller than size*256
            for(int i=0;i<5;i++){
                long long int index=0;
                file_string="./storage/";
                index=database.begin()->first;
                long long int index_bit=0;
                long long int compare_bit=-72057594037927936;
                for(int j=7;j>=1;j--){
                    index_bit+=(index&compare_bit);
                    file_string+=to_string((index&compare_bit)>>(j*8));
                    if(!fork()){/*child process*/
                        execlp(shell_command,shell_command,file_string.c_str(),NULL);
                    }    
                    else{/*parent process*/
                        wait(&status);
                    }        
                    if(j!=1)
                        file_string+="/";
                    if(j != 7)
                        compare_bit>>=8;
                    else
                        compare_bit=71776119061217280;
                }
                //get 256 data
                string put_string_data="";
                string put_string_index="";
                bool index_exist[256]={false};
                for (map<long long int,string>::iterator nowone=database.begin();(nowone->first-index)<256;){
                    index_exist[(nowone->first)-index_bit]=true;
                    put_string_data+=nowone->second+"\n";
                    database.erase(database.begin());
                    if(!database.empty())
                        nowone=database.begin();
                }
                //write record file
                fstream record;
                int record_number=0;
                record.open(file_string+"/record.txt", ios::in);
                if(!record){
                    record.open(file_string+"/record.txt",ios::out);
                    record<<"0\n";
                    record.close();
                }
                else{
                    string file_number="0";
                    if(getline(record,file_number)){
                        record.close();
                        record_number=stoi(file_number)+1;
                        record.open(file_string+"/record.txt",ios::out);
                        record<<record_number;
                        record.close();
                    }
                    else{
                        cout<<"no number in record"<<endl;
                        record.close();
                    }
                }
                //write data file
                record.open(file_string+"/"+to_string(record_number)+".data",ios::out);
                record<<put_string_data;
                record.close();
                record.open(file_string+"/"+to_string(record_number)+".index",ios::out);
                for(int j=0;j<256;j++){
                    record<<index_exist[j];
                }
                record.close();
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
                    string file_string="./storage/";
                    long long int compare_bit=-72057594037927936;
                    for(int i=7;i>=1;i--){
                        file_string+=to_string((p1_int&compare_bit)>>(i*8));     
                        if(i != 1)
                            file_string+="/";
                        if(i != 7)
                            compare_bit>>=8;
                        else
                            compare_bit=71776119061217280;
                    }
                    //read record file
                    fstream record;
                    record.open(file_string+"/record.txt", ios::in);
                    if(!record)
                        output_string+="EMPTY\n";
                    else{
                        string in_file_number="0";
                        int record_number=0;
                        if(getline(record,in_file_number)){
                            record.close();
                            record_number=stoi(in_file_number);
                            for(int i=record_number;i>=0;i--){
                                record.open(file_string+"/"+to_string(i)+".index", ios::in);
                                char read_bit=' ';
                                int data_position=-1;
                                long long int final_position=p1_int&255;
                                for(int j=0; j <= final_position;j++){
                                    record.get(read_bit);
                                    if(read_bit=='1')
                                        data_position++;
                                    if(j==final_position && read_bit!='1')
                                        data_position=-1;
                                }
                                record.close();
                                if(data_position==-1){
                                    if(i==0)
                                        output_string+="EMPTY\n";
                                    else
                                        continue;
                                }
                                else{
                                    record.open(file_string+"/"+to_string(i)+".data", ios::in);
                                    record.seekg(ios::beg);
                                    for(int j=0; j < data_position-1;j++){
                                        record.ignore(numeric_limits<streamsize>::max(),'\n');
                                    }
                                    string data_number;
                                    getline(record,data_number);
                                    output_string+=(data_number+"\n");
                                    record.close();
                                    break;
                                }
                            }
                        }
                    }
                }
            }   
        }
        else if(instruction == "SCAN"){
            long long int p2_int=stoll(p2);
            if(!used_disk){
                for(int i=0;i <= p2_int-p1_int;i++){
                    if(database.find(p1_int+i) != database.end())
                        output_string+=(database[p1_int+i]+"\n");
                    else{
                        output_string+="EMPTY\n";
                    }
                }
            }
            else{
                for(long long int get_index=p1_int; get_index<=p2_int ;get_index++){
                    if(database.find(get_index) != database.end())
                        output_string+=(database[get_index]+"\n");   
                    else{
                        string file_string="./storage/";
                        long long int compare_bit=-72057594037927936;
                        for(int i=7;i>=1;i--){
                            file_string+=to_string((get_index&compare_bit)>>(i*8));     
                            if(i != 1)
                                file_string+="/";
                            if(i != 7)
                                compare_bit>>=8;
                            else
                                compare_bit=71776119061217280;
                        }
                        //read record file
                        fstream record;
                        record.open(file_string+"/record.txt", ios::in);
                        if(!record)
                            output_string+="EMPTY\n";
                        else{
                            string in_file_number="0";
                            int record_number=0;
                            if(getline(record,in_file_number)){
                                record.close();
                                record_number=stoi(in_file_number);
                                for(int i=record_number;i>=0;i--){
                                    record.open(file_string+"/"+to_string(i)+".index", ios::in);
                                    char read_bit=' ';
                                    int data_position=-1;
                                    long long int final_position=get_index&255;
                                    for(int j=0; j <= final_position;j++){
                                        record.get(read_bit);
                                        if(read_bit=='1')
                                            data_position++;
                                        if(j==final_position && read_bit!='1')
                                            data_position=-1;
                                    }
                                    record.close();
                                    if(data_position==-1){
                                        if(i==0)
                                            output_string+="EMPTY\n";
                                        else
                                            continue;
                                    }
                                    else{
                                        record.open(file_string+"/"+to_string(i)+".data", ios::in);
                                        record.seekg(ios::beg);
                                        for(int j=0; j < data_position;j++){
                                            record.ignore(200,'\n');
                                        }
                                        string data_number;
                                        getline(record,data_number);
                                        output_string+=(data_number+"\n");
                                        record.close();
                                        break;
                                    }
                                }
                            }
                        }
                    }
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

    //put map content in disk
    char const *shell_command="./mkdir.sh";
    string file_string="./storage";
    int status;
    if(!fork()){/*child process*/
        execlp(shell_command,shell_command,file_string.c_str(),NULL);
    }    
    else{/*parent process*/
        wait(&status);
    }
    memory_data_path="./storage/memory.data";
    FILE* fp_memory_data=fopen(memory_data_path.c_str(),"w");
    if(!fp_memory_data)
        cout<<"can't write file";
    else{
        output_string="";
        while(1){
            if(database.size()>1)
                database.erase(database.begin());
            else
                break;
            output_string=to_string(database.begin()->first)+" "+(database.begin()->second)+"\n";
            fprintf(fp_memory_data,"%s",output_string.c_str());
        }
        fclose(fp_memory_data);
    }

    //record use disk
    if(used_disk){
        use_disk_path="./storage/use_disk.data";
        FILE* fp_use_disk=fopen(use_disk_path.c_str(),"w");
        if(!fp_use_disk)
            cout<<"can't write file";
        else{
            fprintf(fp_use_disk,"%c",'1');
            fclose(fp_use_disk);
        }
    }
}