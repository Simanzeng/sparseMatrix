#pragma once

//
template<class T>
class Trituple
{
public:
	Trituple();
	~Trituple();
	int row,col;
	T value;
	Trituple<T> &operator = (Trituple<T> &);
};





template<class T>
Trituple<T>::Trituple(void)
{
}


template<class T>
Trituple<T>::~Trituple(void)
{
}

template<class T>
Trituple<T> & Trituple<T>::operator = (Trituple<T> & x)
{
	row=x.row;
	col=x.col;
	value=x.value;
	return *this;
}
