#include<iostream>
#include<vector>
#include<stack>
#include<deque> 
#include<fstream>
using namespace std;



//saves the nfa given 
struct nfa
{
    bool has_epsilon=false;
    vector<vector<bool>> state_info;
    vector<bool> epsilon;
};

//saves the converted machine
struct dfa
{
    bool qacc=false;
    vector<bool> ID;
    vector<vector<bool>> info;
};

//converter class
class converter{
public:
    //gets initial data from user
    void get_data();
    //gets machine table from user 
    void get_table();
    //w8
    converter(){
        nfa_obj.resize(1);
    }
    //w8
    ~converter(){
        nfa_obj.empty();
    }
    //convert nfa start state to dfa start state
    void convert_qstart();
    //conver table of nfa machine to dfa table 
    void convert_machine();
    //printing the final dfa machine
    void print();
private:
//saves start state of nfa machine 
int qstart_nfa=0;
//saves accept state of final machine
vector<bool> qacc;
//saves number of alphabet member
int alphabet_count=0;
//saves number of nfa machine state 
int nfa_state_count=0;
//saves number of dfa machine state
int dfa_state_count=0;
//make object of nfa struct
vector<nfa> nfa_obj;
//saves start state of final machine
vector<bool> dfa_qstart;
//make objects of dfa struct
vector<dfa> dfa_obj;
//saves the state should be done 
deque <vector<bool>> dque;
};

///this function getting number of states and number of alphabet and some basics for initialing the convertion  
void converter::get_data(){
    string s_temp="";
    string s_temp_1="";
    int i_temp = 0;
    //this part gets number of state and alphabet member 
    cout << "Enter Number Of your stages :(stages will save like q0,q1,...)\n";
    cin >> s_temp;
    nfa_state_count=stoi(s_temp)  ;
    cout << "Enter your number of alphabet member epslion will not count  : (alphabet will save like a,b,c,...)\n";
    cin >> s_temp;
    alphabet_count = stoi(s_temp);
    //this part gets accept state of nfa machine
    cout<<"Enter accept states like 123... : it connot be greaater than nfa state count\n";
    cin>>s_temp;
    qacc.resize(nfa_state_count);
    for (int k = 0; k < s_temp.length(); k++)
    {
        s_temp_1=s_temp[k];
        i_temp=stoi(s_temp_1);
        if(i_temp!=0)
            qacc[i_temp-1]=1;
    }
    //this part gets start state of nfa 
    cout<<"Enter start state it should be 1 number :\n";
    cin>>s_temp;
    qstart_nfa=stoi(s_temp);
}

///this function getting table of nfa machine with 2 condition: 1.from file 2.on program 
void converter::get_table(){    
    string s_temp="";
    int i_temp=0;
    string s_temp_1="";
    nfa_obj.resize(nfa_state_count +1);
    nfa_obj[0].state_info.resize(alphabet_count);
    nfa_obj[0].epsilon.resize(nfa_state_count);

    for (int i = 1; i < nfa_state_count+1; i++)
    {
        nfa_obj[i].state_info.resize(alphabet_count);
        nfa_obj[i].epsilon.resize(nfa_state_count);
        for (int j = 0; j < (alphabet_count+1); j++)
        {
            if (j<alphabet_count)
            {
                nfa_obj[i].state_info[j].resize(nfa_state_count);
                cout<<"state "<<i<<" with alphabet number "<<j<<" goes to:\n";
                cin>>s_temp;
                for (int k = 0; k < s_temp.length(); k++)
                    {
                        s_temp_1=s_temp[k];
                        i_temp=stoi(s_temp_1);
                        if(i_temp!=0)
                            nfa_obj[i].state_info[j][i_temp-1]=1;
                    }
            }
            else{
                cout<<"if this state has epsilon enter 'yes' or just type no \n";
                cin>>s_temp;
                if (s_temp=="yes")
                {
                    nfa_obj[i].has_epsilon=true;
                    cout<<"q "<<i<<" with epsilon goes to :\n";
                    cin>>s_temp;
                    for (int k = 0; k < s_temp.length(); k++)
                    {
                        s_temp_1=s_temp[k];
                        i_temp=stoi(s_temp_1);
                        if(i_temp!=0)
                        nfa_obj[i].epsilon[i_temp-1]=1;
                    }
                }  
            }
        }  
    }    
}

// converts start sate of nfa machine to dfa 
void converter::convert_qstart(){
    dfa_qstart.resize(nfa_state_count);
    dfa_qstart[qstart_nfa-1]=1;
    if (nfa_obj[qstart_nfa].has_epsilon==true)
    {
        for (int i = 0; i < nfa_state_count; i++)
        {
            if (nfa_obj[qstart_nfa].epsilon[i]==1)
                dfa_qstart[i]=1;    
        }   
    }
}

///this functionn convert table of nfa machine to dfa bascily the main convert 
void converter::convert_machine(){
    vector<bool> temp;
    dfa_state_count=1;
    temp.resize(nfa_state_count);
    dque.push_back(dfa_qstart);
    while (dque.empty()!=true)
    {
        dfa_obj.resize(dfa_state_count);
        dfa_obj[dfa_state_count-1].info.resize(alphabet_count);
        dfa_obj[dfa_state_count-1].ID.resize(nfa_state_count);
        temp=dque.front();
        int i_temp=0;

        for (int i = 0; i < nfa_state_count; i++)
        {
            if (temp[i]==1)
            {
                i_temp=0;
                dfa_obj[dfa_state_count-1].ID[i]=1;
                for (int j = 0; j < alphabet_count; j++)
                {
                    dfa_obj[dfa_state_count-1].info[j].resize(nfa_state_count);
                    for ( int k = 0; k < nfa_state_count; k++)
                    {
                        dfa_obj[dfa_state_count-1].info[j][k]=nfa_obj[i+1].state_info[j][k]||dfa_obj[dfa_state_count-1].info[j][k];  
                    }    
                } 
                if (qacc[i]==1)
                {
                    dfa_obj[dfa_state_count-1].qacc=true;
                }       
            }
            else
            {
                i_temp++;
            }    
        }

        if (i_temp==nfa_state_count)
        {
            for (int j = 0; j < alphabet_count; j++)
            {
                dfa_obj[dfa_state_count-1].info[j].resize(nfa_state_count);
                    for ( int k = 0; k < nfa_state_count; k++)
                    {
                        dfa_obj[dfa_state_count-1].info[j][k]=0;  
                    }        
            }  
        }
        
        for (int j = 0; j < alphabet_count; j++)
        {
            for (int k = 0; k < nfa_state_count; k++)
            {
                if(dfa_obj[dfa_state_count-1].info[j][k]==true)
                {
                    if (nfa_obj[k+1].has_epsilon==true)
                    {
                        for (int l = 0; l < nfa_state_count; l++)
                        {
                            dfa_obj[dfa_state_count-1].info[j][l]=nfa_obj[k+1].epsilon[l]||dfa_obj[dfa_state_count-1].info[j][l];
                        }
                        
                    }
                                        
                }
            }    
        }
        

        for (int i = 0; i < alphabet_count; i++)
        {
            for (int j = 0; j < dfa_state_count; j++)
            {
                if (dfa_obj[dfa_state_count-1].info[i]!=dfa_obj[j].ID)
                {
                    if (j==dfa_state_count-1)
                    {
                        for ( int k = 0; k < int(dque.size()); k++)
                        {
                            temp=dque.at(k);
                            if (dfa_obj[dfa_state_count-1].info[i]!=temp){
                                if (k==dque.size()-1){
                                    dque.push_back(dfa_obj[dfa_state_count-1].info[i]);
                                    break;
                                }
                            }
                            else
                                break;
                        }
                        break;
                    }
                }
                else
                    break;
            }    
        }
        
        dque.pop_front();
        dfa_state_count++;    
    }    
}


void converter::print(){
    bool p_check=false;

    ofstream write_file("table.csv");;
	write_file << "state name,";
    for (int i = 0; i < alphabet_count; i++)
    {
        write_file << "alphabet number "<<i+1<<",";
    }
    write_file << "accept state \n";
    for (int i = 0; i < dfa_state_count-1; i++)
    {
        for (int j = 0; j < nfa_state_count; j++)
        {
            if (dfa_obj[i].ID[j]==true )
            {
                write_file<<j+1<<" ";
                p_check=true;
            }  
        }
        if (p_check==false)
            write_file<<"{}";
        p_check=false;

        write_file<<",";
        for (int j = 0; j < alphabet_count; j++)
        {
            for (int k = 0; k < nfa_state_count; k++)
            {
                if (dfa_obj[i].info[j][k]==true)
                {
                    write_file<<k+1<<" ";
                    p_check=true;
                }   
            }
            if(p_check==false)
                write_file<<"{}"; 
            p_check=false;
            write_file<<",";
        }
        if (dfa_obj[i].qacc==true)
        {
            write_file<<"yes";
        }
        else
            write_file<<"no";

        write_file<<endl;    
    }
        
	write_file.close();

    cout<<"ID"<<"\t";
    for (int i = 0; i < alphabet_count; i++)
    {
        cout<<"alphabet"<<i<<"  ";
    }
    cout<<"accept state ";
    cout<<endl;
    for (int i = 0; i < dfa_state_count-1; i++)
    {
        for (int j = 0; j < nfa_state_count; j++)
        {
            if (dfa_obj[i].ID[j]==true )
            {
                cout<<j+1<<",";
                p_check=true;
            }  
        }
        if (p_check==false)
            cout<<"{}";
        p_check=false;
        cout<<"\t";
    
        for (int j = 0; j < alphabet_count; j++)
        {
            for (int k = 0; k < nfa_state_count; k++)
            {
                if (dfa_obj[i].info[j][k]==true)
                {
                    cout<<k+1<<",";
                    p_check=true;
                }   
            }
            if(p_check==false)
                cout<<"{}"; 
            p_check=false;
            cout<<"\t        ";
        }
        if (dfa_obj[i].qacc==true)
        {
            cout<<"yes";
        }
        else
            cout<<"no";

        cout<<endl;    
    }    
}

int main(){
    converter obj;
    obj.get_data();
    obj.get_table();
    obj.convert_qstart();
    obj.convert_machine();
    obj.print();
    return 0;

}