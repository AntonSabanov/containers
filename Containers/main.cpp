#include "pch.h"
#include <iostream>
#include "../Containers/DynamicArray.cpp"
#include "../Containers/List.cpp"

int main()
{
	DArray<int> array;
	for (int i = 0; i < 10; ++i)
		array.Insert(i + 1);

	for (int i = 0; i < array.Size(); ++i)
		array[i] *= 2;

	for (auto it = array.GetIterator(); it.HasNext(); it.Next())
		std::cout << it.Get() << std::endl;
}

