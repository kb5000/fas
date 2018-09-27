#include<iostream>
#include"faster.hpp"
#include"textpron.hpp"
#include"table.hpp"
#include<vector>
using namespace std;
void testt();
//this file should not be saved
//it is just a template
//enter your code
//#include"fastt.h"



template<typename T>
inline void print(T vec)
{
	for (auto& i:vec)
	  cout<<i<<endl;
}
class testc
{
public:
  
  
private:
  
};

void maintest()
{
	
	
	
	
	
}

int main()
{
	do
	{
    testt();
    maintest();
	}while (cin.get()!='e');
	cin.get();
	return 0;
}

#define initT fas::timer times
#define beginT times.start()
#define endT times.stop()
#define outT(units) times.gettimes(fas::timer::unit::units)
//units:nano,micro,milli,sec,min,hour
void testt()
{
initT;
beginT;
  
endT;

}
//eg:cout<<outT(micro)<<endl;
 
