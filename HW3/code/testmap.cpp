#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <limits>
using namespace std;

int main(){
    // map<int,string> data;
    // data[0]="aa";
    // cout<<data.begin()->first<<" "<<data.end()->first;
    //cout<<data.begin()->second<<" "<<data.end()->second;

    // string folder;
    // cin>>folder;
    // string file="./mkdir.sh "+folder;
    // system(file.c_str());

    // long long int test=255;
    // for(int i=0;i<70;i++){
        
        
    //     test<<=8;
    //     cout<<test<<endl;
    // }

    //cout<<(9187343239835811840>>8)<<endl;

    //cout<<CLOCKS_PER_SEC<<endl;
    // string shell_command="./mkdir.sh";
    // string arg="storage/1";
    // execlp(shell_command.c_str(),shell_command.c_str(),arg.c_str(),NULL);

    // fstream write_record;
    // write_record.open("hi.txt",ios::in);
    // if(!write_record)
    //     cout<<"d"<<endl;
    // else{
    //     char a='q';
    //     while(write_record.get(a))
    //     cout<<a<<" ";
    // }
    // int a=1;
    // int b=5;
    // cout<<(a+=b)<<endl;

    // fstream file;
    // file.open("hi.txt",ios::in);
    // file.seekg(ios::beg);
    // for(int i=0; i < 0 - 1; ++i){
    //     file.ignore(numeric_limits<streamsize>::max(),'\n');
    // }
    // string line8;
    // file >> line8;
    // cout << line8;
    string memory_data="./1.input";
    FILE *fp_memory_data=fopen(memory_data.c_str(),"r");
    if(fp_memory_data)
        cout<<"11";
}