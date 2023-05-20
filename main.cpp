#include <iostream>
#include "BooleanbasedBlind.h"
#include "errorbasedsqlInjection.h"
#include "Timebasedsqlinjection.h"

using namespace std;

int main()
{

    cout << "what type of vulnerability do you want to detect?\n";
    cout << "1.Booleanbased\n2.errorbased\n3.timebased\n";
    int n;
    cin >> n;
    if(n==1) Booleanbased();
    if(n==2) errorbased();
    if(n==3) timebased();

}
