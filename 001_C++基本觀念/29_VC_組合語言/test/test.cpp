#include <iostream>
using namespace std;
//C語言不額外增加變數將2個變數值互換!
void main()
{	
    float a=9.004, b=-28.5;
	//作弊法... for int or float
    _asm {
     push a
     push b
     pop a
     pop b}
    cout<< a <<"  "<<b<<"\n";
} 