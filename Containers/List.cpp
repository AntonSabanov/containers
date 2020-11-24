#include "pch.h"
#include <iostream>
#include <cassert>
#include <utility>

#define CHANK_CAPACITY 2 // размер 2 выбран для удобства тестирования и отладки (выбранный размер чанка 16)

template < typename T>
class List final
{
private:
	List(const List&) = delete; // приватный конструктор копирования для запрета копирования объектов класса
	void operator=(const List&) = delete;
/////////////////////////////////////////////////////////////////////////////////
	class Chunk
	{
	private:
		T *items = nullptr; //хранимые значения
		int chunkSize; //количество элементов в чанке//
	public:
		Chunk* next = nullptr; //ссылка на следующий чанк
		Chunk* prev = nullptr; //ссылка на предыдущий чанк

		Chunk(T* itemsPtr)
		{
			items = itemsPtr; // адрес в выделенной памяти, с которой начинаяютя значения
			chunkSize = 0;
		}

		~Chunk()
		{
			for (int i = 0; i < chunkSize; i++) //сделать проход до размера
			{
				items[i].~T();
			}
		}

		int Size()
		{
			return chunkSize;
		}

		void SetSize(int newSize)
		{
			chunkSize = newSize;
		}

		void SetValue(int index, const T& value)
		{
			new (&items[index])T(value);
			chunkSize++;
		}

		void RemoveValue(int index)
		{
			items[index].~T();
			chunkSize--;
		}

		const T& GetValue(int index) const
		{
			return items[index];
		}

		T& GetItem(int index)
		{
			return items[index];
		}
	};
/////////////////////////////////////////////////////////////////////////////////
	Chunk *head;
	Chunk *tail;
	int listSize; 

	class Iterator
	{
	private:
		int curIndex; // текущее положение итератора в списке
		int curChunkIndex; // текущее положение итератора в чанке
		List<T>* curList;
		Chunk* curChunk;
	public:
		Iterator(List<T>* iterated)
		{
			this->curList = iterated;
			curIndex = 0;
			curChunk = curList->head;
			curChunkIndex = 0;
		}

		const T& Get() const
		{
			return curChunk->GetValue(curChunkIndex);
		}

		void Set(const T& value)
		{
			curChunk->RemoveValue(curChunkIndex);
			curChunk->SetValue(curChunkIndex, value);
		}

		void Insert(const T& value)
		{
			if (curChunk->Size() == CHANK_CAPACITY) // если текущий чанк заполнен
			{
				// 1) добавить новый чанк после текущего
				// 2) перенести все элементы начиная с теущей позиции в новый чанк
				// 3) вставить элемент в текущий чанк на текущую позицию
				Chunk* newChunk;
				if (curChunk == curList->tail || curList->tail == curList->head) // если текущий чанк хвостовой то чанк добавляется в хвост
				{
					curList->AddNewTailChunk();
					newChunk = curList->tail;
				}
				else
				{
					newChunk = curList->AddNewMiddleChunk();
					auto temp = curChunk->next;
					newChunk->prev = curChunk;
					newChunk->next = curChunk->next;
					curChunk->next = newChunk;
					temp->prev = newChunk;
				}	

				int j = (curChunk->Size() - 1) - curChunkIndex; //позиция в новом чанке
				newChunk->SetSize(j + 1);

				for (int i = curChunk->Size() - 1; i >= curChunkIndex; i--)
				{					
					new(&(newChunk->GetItem(j)))T(std::move(curChunk->GetItem(i)));
					j--;
					curChunk->GetItem(i).~T();
				}				
				curChunk->SetValue(curChunkIndex, value);
				curChunk->SetSize(curChunkIndex + 1); 
			}
			else
			{
				for (int i = curChunk->Size() - 1; i >= curChunkIndex; i--)
				{
					new(&(curChunk->GetItem(i + 1)))T(std::move(curChunk->GetItem(i)));
					curChunk->GetItem(i).~T();
				}
				curChunk->SetValue(curChunkIndex, value);
			}
			curList->listSize++;		
		}

		void Remove()
		{
			if (curChunk->Size() == 1) // в чанке остался 1 элемент и после его удаления чанк надо тоже удалить
			{
				//если текущий чанк - головной
				if (curChunk == curList->head)
				{
					curList->RemoveHead();
					curChunk = curList->head;
					curChunkIndex = 0;
				}
				//если текущий чанк - хвостовой
				else if (curChunk == curList->tail)
				{
					curList->RemoveTail();
					curChunk = curList->tail;
					curChunkIndex = curChunk->Size() - 1;
				}
				//если текущий чанк - в середине
				else if (curChunk->next != nullptr && curChunk->prev != nullptr)
				{
					auto nextTemp = curChunk->next;
					auto prevTemp = curChunk->prev;
					curChunk->RemoveValue(0);
					free(curChunk);
					nextTemp->prev = prevTemp; //связываем чанки
					prevTemp->next = nextTemp;
					curChunk = nextTemp;
					curChunkIndex = 0;
					curList->listSize--;//
				}
			}
			else
			{
				curChunk->RemoveValue(curChunkIndex);

				for (int i = curChunkIndex + 1; i < curChunk->Size() + 1; i++) //+1 потому что в remove умненьшается размер на 1
				{
					curChunk->GetItem(i - 1) = std::move(curChunk->GetItem(i));
				}

				if (curChunkIndex > curChunk->Size() - 1) // если итератор указывает на пустой элемент после удаления последнего элемента в чанке
				{
					if (curChunk->next != nullptr) //если есть следующий чанк, то итератор переводится на его нулевой элемент
					{
						curChunk = curChunk->next;
						curChunkIndex = 0;
					}
					else	// иначе сдвигаю итератор на 1 влево
						curChunkIndex--;
				}
				curList->listSize--;//
			}			
			curIndex--;		
		}

		void Next()
		{		
			if (HasNext())
			{
				if (curChunkIndex == (curChunk->Size() - 1))
				{
					curChunk = curChunk->next;
					curChunkIndex = 0;
				}
				else
				{
					curChunkIndex++;
				}
				curIndex++;
			}
		}

		void Prev()
		{
			if (HasPrev())
			{
				if (curChunkIndex == 0)
				{
					curChunk = curChunk->prev;
					curChunkIndex = curChunk->Size() - 1;
				}
				else
				{
					curChunkIndex--;
				}
				curIndex--;
			}
		}

		bool HasNext() const
		{
			if (curChunkIndex < (curChunk->Size() - 1))
			{
				return true;
			}
			else if (curChunk->next != nullptr)
			{
				return true;
			}
			else return false;
		}

		bool HasPrev() const
		{
			if (curChunkIndex > 0)
			{
				return true;
			}
			else if (curChunk->prev != nullptr)
			{
				return true;
			}
			else return false;
		}
	};

	Chunk* MemoryAllocation()
	{
		auto chunk = (Chunk*)malloc(sizeof(Chunk) + (CHANK_CAPACITY * sizeof(T))); // выделение памяти под чанк и хранимые данные		
		new (chunk)Chunk((T*)(chunk + 1)); // размещение объекта чанка начиная с первой ячейки памяти
		return chunk;
	}

	void AddNewHeadChunk()
	{
		auto newChunk = MemoryAllocation();
		head->prev = newChunk;
		newChunk->next = head;
		newChunk->prev = nullptr;
		head = newChunk;
	}

	void AddNewTailChunk()
	{
		auto newChunk = MemoryAllocation();
		tail->next = newChunk;
		newChunk->next = nullptr;
		newChunk->prev = tail;
		tail = newChunk;
	}

	Chunk* AddNewMiddleChunk() /////////////////////
	{
		return MemoryAllocation();;
	}

public:
	List()
	{
		head = MemoryAllocation();
		tail = head;
		listSize = 0;
	}

	~List()
	{
		Chunk* curChunk = head;
		while (curChunk->next != nullptr)
		{
			auto temp = curChunk->next;
			curChunk->~Chunk();
			free(curChunk);
			curChunk = temp;
		}
		curChunk->~Chunk();
		free(curChunk);	
	}

	void InsertHead(const T& value)
	{
		auto headChunkSize = head->Size();

		if (headChunkSize + 1 > CHANK_CAPACITY)
		{
			AddNewHeadChunk();
			headChunkSize = 0;
		}

		for (int i = headChunkSize - 1; i >= 0; i--)
		{
			auto test1 = head->GetItem(i);
			new(&(head->GetItem(i + 1)))T(std::move(head->GetItem(i)));
			head->GetItem(i).~T();
		}

		head->SetValue(0, value);
		listSize++;
	}

	void InsertTail(const T& value)
	{
		auto tailChunkSize = tail->Size();

		if (tailChunkSize + 1 > CHANK_CAPACITY)
		{
			AddNewTailChunk();
			tailChunkSize = 0;
		}
		tail->SetValue(tailChunkSize, value);
		listSize++;
	}

	void RemoveHead()
	{		
		assert(listSize > 0);
		auto headChunkSize = head->Size();

		if (headChunkSize == 1 && head->next != nullptr)
		{
			head->RemoveValue(0);
			auto temp = head->next;
			free(head);
			head = temp;
			head->prev = nullptr;
			headChunkSize = head->Size();
		}
		else
		{
			head->RemoveValue(0);

			for (int i = 1; i < headChunkSize; i++)
			{
				head->GetItem(i - 1) = std::move(head->GetItem(i));
			}
		}			
		listSize--;
	}

	void RemoveTail()
	{
		assert(listSize > 0);
		auto tailChunkSize = tail->Size();

		if (tailChunkSize == 1 && tail->prev != nullptr)
		{
			tail->RemoveValue(tailChunkSize - 1);
			auto temp = tail->prev;
			free(tail);
			tail = temp;
			tail->next = nullptr;
			tailChunkSize = tail->Size();
		}
		else
		{
			tail->RemoveValue(tailChunkSize - 1);
		}
		listSize--;
	}

	const T& Head() const
	{
		return head->GetValue(0);
	}

	const T& Tail() const
	{
		auto tailChunkSize = tail->Size();
		return tail->GetValue(tailChunkSize - 1);
	}

	int Size() const
	{
		return listSize;
	}

	Iterator GetIterator()
	{
		Iterator iterator(this);
		return iterator;
	}
};
