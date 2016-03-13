#pragma once
#include "Trituple.h"
#include<iostream>
#include<cstdlib>

using namespace std;

const int DafaultSize=100;

template<class T>
class SparseMatrix
{
public:
	SparseMatrix(int maxSz=DafaultSize);
	SparseMatrix(SparseMatrix<T> &x);
	~SparseMatrix();
	template<class T>
	friend ostream& operator << (ostream &out,SparseMatrix<T> &M);
	template<class T>
	friend istream& operator >> (istream &in,SparseMatrix<T> &M);
	SparseMatrix<T>& operator = (SparseMatrix<T> &x);
	SparseMatrix<T> Transpose();
	SparseMatrix<T> Add(SparseMatrix<T> &b);
	SparseMatrix<T> Multiple(SparseMatrix<T> &b);
private:
	int Rows,Cols,Terms;
	Trituple<T> *smArray;
	int maxTerms;
};





template<class T>
SparseMatrix<T>::SparseMatrix(int maxSz):maxTerms(maxSz)
{
	if(maxSz<1)
	{
		cerr<<"¾ØÕó³õÊ¼»¯Öµ´í£¡"<<endl;
		exit(1);
	}
	smArray=new Trituple<T>[maxSz];
	if(smArray==NULL)
	{
		cerr<<"´¢´æ·ÖÅä´íÎó£¡"<<endl;
		exit(1);
	}
	Terms=0;
	Rows=0;
	Cols=0;
}


template<class T>
SparseMatrix<T>::SparseMatrix(SparseMatrix<T> &x)
{
	Rows=x.Rows;
	Cols=x.Cols;
	Terms=x.Terms;
	maxTerms=x.maxTerms;
	smArray=new Trituple<T>[maxTerms];
	if(smArray==NULL)
	{
		cerr<<"´¢´æ·ÖÅä´íÎó£¡"<<endl;
		exit(1);
	}
	for(int i=0;i<Terms;i++)
		smArray[i]=x.smArray[i];
}


template<class T>
SparseMatrix<T>::~SparseMatrix()
{
	delete [] smArray;
}

template<class T>
ostream& operator << (ostream &out,SparseMatrix<T> &M)
{
	out<<"rows="<<M.Rows<<endl;
	out<<"cols="<<M.Cols<<endl;
	out<<"Nonzero terms="<<M.Terms<<endl;
	for(int i=0;i<M.Terms;i++)
	{
		out<<"M["<<M.smArray[i].row<<"]["<<M.smArray[i].col<<"]="<<M.smArray[i].value<<endl;
	}
	return out;
}


template<class T>
istream& operator >> (istream &in,SparseMatrix<T> &M)
{
	cout<<"Enter number of rows,columns,and terms"<<endl;
	in>>M.Rows>>M.Cols>>M.Terms;
	if(M.Terms>M.maxTerms)
	{
		cerr<<"Number of terms overflow!"<<endl;
		exit(1);
	}
	for(int i=0;i<M.Terms;i++)
	{
		cout<<"Enter row,column,and value of term:"<<i+1<<endl;
		in>>M.smArray[i].row>>M.smArray[i].col>>M.smArray[i].value;
	}
	return in;
}


template<class T>
SparseMatrix<T>& SparseMatrix<T>::operator = (SparseMatrix<T> &x)
{
	Rows=x.Rows;
	Cols=x.Cols;
	Terms=x.Terms;
	for(int i=0;i<Terms;i++)
	{
		smArray[i]=x.smArray[i];
	}
	return *this;
}

template<class T>
SparseMatrix<T> SparseMatrix<T>::Transpose()
{
	int *rowSize=new int[Cols];
	int *rowStart=new int[Cols];
	SparseMatrix<T> b(maxTerms);
	b.Rows=Cols;
	b.Cols=Rows;
	b.Terms=Terms;
	if(Terms>0)
	{
		int i,j;
		for(i=0;i<Cols;i++) rowSize[i]=0;
		for(i=0;i<Terms;i++) rowSize[smArray[i].col]++;
		rowStart[0]=0;
		for(i=1;i<Cols;i++)
		{
			rowStart[i]=rowSize[i-1]+rowStart[i-1];
		}
		for(i=0;i<Terms;i++)
		{
			j=rowStart[smArray[i].col];
			b.smArray[j].col=smArray[i].row;
			b.smArray[j].row=smArray[i].col;
			b.smArray[j].value=smArray[i].value;
			rowStart[smArray[i].col]++;
		}
	}
	delete []rowSize;
	delete []rowStart;
	return  b;
}


template<class T>
SparseMatrix<T> SparseMatrix<T>::Add(SparseMatrix<T> &b)
{
	SparseMatrix<T> result(maxTerms);
	if(Rows!=b.Rows || Cols!=b.Cols)
	{
		cerr<<"Incompatible matrices"<<endl;
		return result;
	}
	result.Rows=Rows;
	result.Cols=Cols;
	result.Terms=0;
	int i,j,index_a,index_b;
	while(i<Terms && j<b.Terms)
	{
		index_a=smArray[i].row*Cols+smArray[i].col;
		index_b=b.smArray[j].row*b.Cols+b.smArray[j].col;
		if(index_a<index_b)
		{
			result.smArray[result.Terms]=smArray[i];
			i++;
		}
		else if(index_a>index_b)
		{
			result.smArray[result.Terms]=b.smArray[j];
			j++;
		}
		else 
		{
			result.smArray[result.Terms]=smArray[i];
			result.smArray[result.Terms].value=smArray[i].value+b.smArray[j].value;
			i++;
			j++;
		}
		result.Terms++;
	}
	for(;i<Terms;i++)
	{
		result.smArray[result.Terms]=smArray[i];
		result.Terms++;
	}
	for(;j<b.Terms;j++)
	{
		result.smArray[result.Terms]=b.smArray[j];
		result.Terms++;
	}
	return result;
}


template<class T>
SparseMatrix<T> SparseMatrix<T>::Multiple(SparseMatrix<T> &b)
{
	SparseMatrix<T> result(maxTerms);
	if(Cols!=b.Rows)
	{
		cerr<<"Incompatible matrices!"<<endl;
		return result;
	}
	if(Terms==maxTerms || b.Terms==maxTerms)
	{
		cerr<<"One additional space in a or b needed!"<<endl;
		return result;
	}
	int *rowSize=new int[b.Rows];
	int *rowStart=new int[b.Rows+1];
	T *temp=new T[b.Cols];
	int i;
	for(i=0;i<b.Rows;i++)
		rowSize[i]=0;
	for(i=0;i<b.Terms;i++)
		rowSize[b.smArray[i].row]++;
	rowStart[0]=0;
	for(i=0;i<=b.Rows;i++)
		rowStart[i]=rowSize[i-1]+rowStart[i-1];
	int current=0,lastInresult=-1,rowA,colA,colB;
	while(current<Terms)
	{
		rowA=smArray[current].row;
		for(i=0;i<b.Cols;i++) temp[i]=0;
		while(current<Terms && smArray[current].row==rowA)
		{
			colA=smArray[current].col;
			for(i=rowStart[colA];i<rowStart[colA+1];i++)
			{
				colB=b.smArray[i].col;
				temp[colB]+=smArray[current].value*b.smArray[i].value;
			}
			current++;
		}
		for(i=0;i<b.Cols;i++)
		{
			if(temp[i]!=0)
			{
				lastInresult++;
				result.smArray[lastInresult].row=rowA;
				result.smArray[lastInresult].col=i;
				result.smArray[lastInresult].value=temp[i];
			}
		}
	}
	result.Rows=Rows;
	result.Cols=b.Cols;
	result.Terms=lastInresult+1;
	delete []rowSize;
	delete []rowStart;
	delete []temp;
	return result;
}