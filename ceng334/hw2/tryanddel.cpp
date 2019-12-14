/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

template <class A>
class B{
  A c;
  int x;
  public:
    B(int s): x(s) { }
    void foo() {cout << "FOO CALLED with: " << this->x << endl;}
};

class A;
typedef B<A> BA;

class A {
    public:
    static BA *b; 
    
};

class C :public A{
    public:
    void boo(){
        this->b->foo();
    }
};
BA* A::b = new BA(13);

int main()
{
    C c;
    c.boo();
    cout<<"Hello World";

    return 0;
}
