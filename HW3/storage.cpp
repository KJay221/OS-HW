#include <iostream>
#include <fstream>
#include<string>
using namespace std;

int main(){
    //open input instruction and store it
    /*cout<<"Please enter file path: ";
    string file_path="";
    cin>>file_path;*/

    //read 10000 instruction each cycle
    bool input_end=false;
    fstream file;
    //file.open(file_path, ios::in);
    file.open("./hw3example.input", ios::in);
    if(!file){
        cout<<"can't open file."<<endl;
        return 0;
    }
    for(int line=0;line<20000000;line++){
        string input="";
        if(!getline(file, input)){
            input_end=true;
            break;
        }
        //split instruction
        string instruction = "";
        string p1 = "";
        string p2 = "";
        string delimiter = " ";
        size_t pos = 0;
        string token;
        for(int i=0;i<2;i++){
            if((pos = input.find(delimiter)) == string::npos)
                break;
            if(i==0)
                instruction = input.substr(0, pos);
            else if(i==1)
                p1 = input.substr(0, pos);
            input.erase(0, pos + 1);
        }
        (instruction=="PUT" || instruction=="SCAN")?p2=input:p1=input;
        cout<<instruction<<endl;
        cout<<p1<<endl;
        cout<<p2<<endl;    
    }
}
