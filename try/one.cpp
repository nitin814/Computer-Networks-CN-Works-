#include <bits/stdc++.h>
using namespace std;

int main ()
{
	string str;
	getline(cin,str);
		cout<<str<<endl;
	cin>>str;
	
	int t = 0;
	while (t!=100)
	{
		if (str[t]=='\0')
			break;

		t++;
	}
	cout<<t<<endl;
}
