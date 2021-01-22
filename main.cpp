#include <iostream>
#include <vector>
using namespace std;

void display(int p, const vector<int> a){
    cout << p+1 << ": ";
    for (int i : a){
        cout << i << " ";
    }
    cout << endl;
}

int main() {
    vector<int> a{1,0,1,0};
    size_t invalid_cnt = 0;
    for (size_t i = 0; i < a.size(); i++){
        display(i, a);
        while (a[i] != 1){
            size_t index = a.size() - 1 - invalid_cnt++;
            if (index  <= i) goto label;
            swap(a[i], a[index]);
            display(i, a);
        }

    }
    label:
    display(a.size(), a);
    printf("cnt:%d", invalid_cnt);
    return 0;
}

