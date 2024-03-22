#include <iostream>

using namespace std;

int idx = 0;
int mydata[3] = {0};

int next(int current)
{
    mydata[idx++] = current;
    idx %= 3;

    int a = mydata[0];
    int b = mydata[1];
    int c = mydata[2];

    // source: https://www.geeksforgeeks.org/middle-of-three-using-minimum-comparisons/
    if ((a < b && b < c) || (c < b && b < a))
       return b;
 
    // Checking for a
    else if ((b < a && a < c) || (c < a && a < b))
       return a;
 
    else
       return c;    
}
int main()
{
    int n;
    for (int i = 0; i < 2; i++) {
        cin >> n;
        next(n);
    }
    
    while (cin >> n)
        cout << next(n) << endl;
}