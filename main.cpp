#include <iostream>
#include <vector>
using namespace std;


int main() {
//    auto p =
    auto v = vector<decltype(p)>();
    v.push_back(make_shared<char *>(new char[64]));
    cout << sizeof(p.get())<< endl;
    *(v[0]) = "helloxxxxxxxxxxxxx";
    cout << sizeof(p.get())<< endl;
    cout << sizeof(*(v[0]))<< endl;
    cout << *(v[0]) << endl;

}

