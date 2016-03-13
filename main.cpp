#include "SparseMatrix.h"
#include "Trituple.h"
#include<iostream>

using namespace std;

int main()
{
	SparseMatrix<int> a(100),b(100);
	cin>>a;
	b=a.Transpose();
	cout<<b;
	return 0;
}