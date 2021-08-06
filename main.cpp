#include <iostream>
#include <ctime>
using namespace std;

class StringMove
{
    char* data;
    int len;
public:
    friend ostream& operator<<(ostream& os,const StringMove& s);
    StringMove(const char* str){
        len=0;
        while (*str!='\0'){
            ++len;
            ++str;
        }
        data=new char[len+1];
        for (int i=len;i>=0;--i,--str)
            data[i]=*str;
    }
    StringMove(const StringMove& other){
        len=other.len;
        data=new char[len+1];
        for (int i=0;i<=len;++i)
            data[i]=other.data[i];
    }
    StringMove& operator=(const StringMove& other){
        if (data)
            delete []data;
        len=other.len;
        data=new char[len+1];
        for (int i=0;i<=len;++i)
            data[i]=other.data[i];
        return *this;
    }
    StringMove(StringMove&& other){
        data=other.data;
        len=other.len;
        other.data=nullptr;
        other.len=0;
    }
    StringMove& operator=(StringMove&& other){
        data=other.data;
        len=other.len;
        other.data=nullptr;
        other.len=0;
        return *this;
    }
    ~StringMove(){
        delete []data;
    }
};
ostream& operator<<(ostream& os,const StringMove& s)
{
    if (s.data)
        os<<s.data;
    return os;
}
void testfunction()
{
    clock_t stime=clock();
    int N=1000000;
    StringMove s("abc");
    for (int i=0;i<N;++i){
        StringMove tmp("asdljfladjflsadfasdfasdfadsgfdhfgjghjgfadfgsfafasdfajsfl");
        s=std::move(tmp);
    }
    cout<<"Time elapsed "<<clock()-stime<<" ms"<<endl;
}
int main() 
{
    testfunction();
    system("pause");
    return 0;
}
