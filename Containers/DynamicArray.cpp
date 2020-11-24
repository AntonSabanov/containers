#include "pch.h"
#include <iostream>
#include <cassert>
#include <utility>

#define STANDART_CAPACITY 8

template < typename T>
class DArray final
{
private:
	T* items = nullptr;
	int size;
	int capacity;

	DArray(const DArray&) = delete; // ��������� ����������� ����������� ��� ������� ����������� �������� ������
	void operator=(const DArray&) = delete;

	class Iterator
	{
	private:
		int curIndex;
		DArray<T>* curObj;
	public:
		Iterator(DArray<T>* iterated)
		{
			this->curObj = iterated;
			curIndex = 0;
		}

		const T& Get() const
		{
			return (*curObj)[curIndex];
		}

		void Set(const T& value)
		{
			(*curObj)[curIndex] = value;
		}
		
		void Insert(const T& value)
		{
			curObj->Insert(curIndex, value);		
		}

		void Remove()
		{
			curObj->Remove(curIndex);
		}

		void Next()
		{
			if (HasNext())
			{
				curIndex++;
			}
		}

		void Prev()
		{
			if (HasPrev())
			{
				curIndex--;
			}
		}

		void ToIndex(int index)
		{
			assert(index >= 0 && index < curObj->size);
			curIndex = index;
		}

		bool HasNext() const
		{
			return curIndex < curObj->size - 1;
		}

		bool HasPrev() const
		{
			return curIndex > 0;
		}
	};

	void Resize() // ����������������� ������ ��� ������ 1.00.00
	{
		// 1) �������� ������ ��� ��������� ��������� ��� �������� size*2 ���������
		capacity *= 2;
		T *temp = (T*)malloc(capacity * sizeof(T));
		// 2) ��������� � � ������ ������ � �����
		for (int i = 0; i < size; i++)
			*(temp + i) = std::move(items[i]);
		// 3) ����������� ������ �����
		free(items);
		// 4) �������������� ������ ��������� items �� � (items = p)
		items = temp;
		// 5) ��������� ����� �������
	}

public:
	DArray() // ��������� ������ ��� ������ (������:2.20.00)
	{
		items = (T*)malloc(STANDART_CAPACITY * sizeof(T));
		this->capacity = STANDART_CAPACITY;
		this->size = 0;
	}

	DArray(int capacity)
	{
		assert(capacity > 0);
		items = (T*)malloc(capacity * sizeof(T));
		this->capacity = capacity;
		this->size = 0;
	}

	~DArray()
	{
		for (int i = 0; i < size; i++)
		{
			items[i].~T();
		}
			
		free(items);
		size = 0;
		capacity = 0;
	}

	void Insert(const T& value) // ������� � ����� � �������������� placement new
	{
		if (size + 1 <= capacity)
		{
			new (&items[size])T(value); //placement new			
			size++;
		}
		else
		{
			Resize();
			new (&items[size])T(value); //placement new	
			size++;
		}
	}

	void Insert(int index, const T& value) //������� �� ������� (�������: 55.00)
	{
		assert(index < capacity);

		if (size + 1 > capacity)
		{
			Resize();
		}
			
		for (int i = size - 1; i >= index; i--)		
		{				
			items[i + 1] = std::move(items[i]);			
		}
	
		new (&items[index])T(value);
		size++;
	}

	void Remove(int index) // �������� �������� (�������: 58.00)
	{
		assert(index < size);
		assert(size > 0);
		//1) ����� ����������� ��� ���������� ��������
		items[index].~T();
		//2) �������� ��� �������� �����
		for (int i = index + 1; i < size; i++)
		{
			items[i - 1] = std::move(items[i]);
		}
		//3) ���������� size �� 1
		size--;
	}

	const T& operator[](int index) const
	{
		assert(index <= capacity);
		return items[index];
	}

	T& operator[](int index)
	{
		assert(index <= capacity);
		return items[index];
	}

	int Size()
	{
		return size;
	}
	
	Iterator GetIterator()
	{
		Iterator iterator(this);
		return iterator;
	}
};