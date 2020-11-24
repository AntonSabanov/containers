#include "pch.h"
#include "../Containers/DynamicArray.cpp"
#include "../Containers/List.cpp"

struct TestStruct
{
	std::string name;
	int id;
	float number;

	~TestStruct()
	{
		id = 0;
		number = 0;
	}
};

class TestClass
{
public:
	int id;
	float number;

	~TestClass()
	{
		id = 0;
		number = 0.0f;
	}
};

TEST(TestCaseName, TestName)
{
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(ConstuctorTests, WithoutParam) //проверка конструктора без параметров
{
	DArray<int> array;
	EXPECT_TRUE(&array[0] != nullptr);
	array[0] = 3;
	EXPECT_EQ(array[0], 3);
}

TEST(ConstuctorTests, WithParam) //проверка конструктора без параметров
{
	DArray<int> array(1);
	EXPECT_TRUE(&array[0] != nullptr);
	array[0] = 3;
	EXPECT_EQ(array[0], 3);
}

TEST(DestructorTest, WithoutParam) //проверка размещающего деструктора
{
	TestStruct testElement1 = { "test1", 4, 0.2f };
	TestStruct testElement2 = { "test2", 7, 3.567f };
	TestStruct testElement3 = { "test3", 2, 3.5f };
	DArray<TestStruct> array(3);
	array.Insert(testElement1);
	array.Insert(testElement2);
	array.Insert(testElement3);
	TestStruct *p = nullptr;
	p = &array[2];
	p->~TestStruct();
	EXPECT_TRUE(array[2].id != 2);
	array[0].~TestStruct();
	EXPECT_TRUE(array[0].id != 4);
}

TEST(InsertTest, WithoutAlloc) //проверка конструктора без параметров
{
	DArray<int> array;
	array.Insert(1);
	array.Insert(2);
	array.Insert(3);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array[1], 2);
	EXPECT_EQ(array[2], 3);
}

TEST(InsertTest, WithAlloc) //проверка конструктора без параметров
{
	DArray<int> array(3);
	array.Insert(1);
	array.Insert(2);
	array.Insert(3);
	array.Insert(4);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array[1], 2);
	EXPECT_EQ(array[2], 3);
	EXPECT_EQ(array[3], 4);
	EXPECT_EQ(array.Size(), 4);
}

TEST(InsertToEnd, OnStruct)
{
	TestStruct testElement1 = { "test1", 4, 0.2f };
	TestStruct testElement2 = { "test2", 7, 3.567f };
	TestStruct testElement3 = { "test3", 2, 3.5f };
	DArray<TestStruct> array(3);
	array.Insert(testElement1);
	array.Insert(testElement2);
	array.Insert(testElement3);
	EXPECT_EQ(array[0].id, 4);
	EXPECT_EQ(array[1].id, 7);
	EXPECT_EQ(array[2].id, 2);
}

TEST(InsertToIndex, OnDouble)
{
	DArray<double> array(3);
	array.Insert(2.5);
	array.Insert(7.15);
	array.Insert(5.5);
	array.Insert(1, 3.33);
	EXPECT_EQ(array[0], 2.5);
	EXPECT_EQ(array[1], 3.33);
	EXPECT_EQ(array[2], 7.15);
	EXPECT_EQ(array[3], 5.5);
	EXPECT_EQ(array.Size(), 4);
}

TEST(InsertToIndex, OnFloat)
{
	DArray<float> array(3);
	array.Insert(1.1f);
	array.Insert(5.3f);
	array.Insert(3.7f);
	array.Remove(0);
	EXPECT_EQ(array[0], 5.3f);
	EXPECT_EQ(array[1], 3.7f);
	EXPECT_EQ(array.Size(), 2);
}

TEST(Iterator, GetTest)
{
	DArray<int> array(3);
	array.Insert(1);
	array.Insert(5);
	array.Insert(3);
	auto it = array.GetIterator();
	EXPECT_EQ(it.Get(), 1);
}

TEST(Iterator, SetTest)
{
	DArray<int> array(3);
	array.Insert(1);
	array.Insert(5);
	array.Insert(3);
	auto it = array.GetIterator();
	it.Set(7);
	EXPECT_EQ(array[0], 7);
}

TEST(Iterator, InsertTest)
{
	DArray<int> array(2);
	array.Insert(2);
	array.Insert(3);
	auto it = array.GetIterator();
	it.Insert(1);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array.Size(), 3);
}

TEST(Iterator, RemoveTest)
{
	DArray<int> array(3);
	auto it = array.GetIterator();
	it.Insert(3);
	it.Insert(2);
	it.Insert(1);
	it.Remove();
	EXPECT_EQ(array[0], 2);
	EXPECT_EQ(array[1], 3);
	EXPECT_EQ(array.Size(), 2);
}

TEST(Iterator, Next_HasNext)
{
	DArray<int> array(3);
	auto it = array.GetIterator();
	it.Insert(3);
	it.Insert(2);
	it.Insert(1);
	it.Next();
	it.Insert(7);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array[1], 7);
	EXPECT_EQ(array[2], 2);
	EXPECT_EQ(array[3], 3);
	EXPECT_EQ(array.Size(), 4);
	it.Next();
	it.Next();
}

TEST(Iterator, Prev_HasPrev)
{
	DArray<int> array(3);
	auto it = array.GetIterator();
	it.Insert(3);
	it.Insert(2);
	it.Insert(1);
	it.Next();
	it.Next();
	it.Prev();
	it.Insert(7);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array[1], 7);
	EXPECT_EQ(array[2], 2);
	EXPECT_EQ(array[3], 3);
	EXPECT_EQ(array.Size(), 4);
}

TEST(Iterator, ToIndex)
{
	DArray<int> array(3);
	auto it = array.GetIterator();
	it.Insert(4);
	it.Insert(2);
	it.Insert(1);
	it.ToIndex(2);
	it.Insert(3);
	EXPECT_EQ(array[0], 1);
	EXPECT_EQ(array[1], 2);
	EXPECT_EQ(array[2], 3);
	EXPECT_EQ(array[3], 4);
	EXPECT_EQ(array.Size(), 4);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(List_ConstructorTest, Test)
{
	List<int> list;
	list.InsertHead(7);
	auto test = list.Head();
	EXPECT_EQ(test, 7);
	EXPECT_TRUE(true);
	//List<int> list; 
	//list.InsertHead(7);
}

TEST(List_HeadInsertTest, Test)
{
	List<int> list;
	list.InsertHead(3);
	list.InsertHead(2);
	list.InsertHead(1);	
	EXPECT_EQ(list.Head(), 1);
}

TEST(List_TailInsertTest, Test)
{
	List<int> list;
	list.InsertHead(3);
	list.InsertHead(2);
	list.InsertHead(1);
	EXPECT_EQ(list.Size(), 3);
	list.InsertTail(7);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 7);
	EXPECT_EQ(list.Size(), 4);
}

TEST(List_HeadInsertTest, ClassTest)
{
	List<TestClass> list;
	TestClass testElement1 = { 4, 0.2f };
	TestClass testElement2 = { 7, 3.567f };
	TestClass testElement3 = { 2, 3.5f };
	list.InsertHead(testElement1);
	list.InsertHead(testElement2);
	list.InsertHead(testElement3);
	auto head = list.Head().id;
	auto tail = list.Tail().id;
	EXPECT_EQ(head, 2);
	EXPECT_EQ(tail, 4);
}

TEST(List_HeadInsertTest, StructTest)
{
	List<TestStruct> list;
	TestStruct testElement1 = { "test1", 4, 0.2f };
	TestStruct testElement2 = { "test2", 7, 3.567f };
	TestStruct testElement3 = { "test3", 2, 3.5f };
	list.InsertHead(testElement1);
	list.InsertHead(testElement2);
	list.InsertHead(testElement3);
	auto head = list.Head().id;
	auto tail = list.Tail().id;
	EXPECT_EQ(head, 2);
	EXPECT_EQ(tail, 4);
}

TEST(List_TailInsertTest, StructTest)
{
	List<TestStruct> list;
	TestStruct testElement1 = { "test1", 4, 0.2f };
	TestStruct testElement2 = { "test2", 7, 3.567f };
	TestStruct testElement3 = { "test3", 2, 3.5f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	EXPECT_EQ(list.Tail().id, 2);
}


TEST(List_HeadInsertTest, AddHeadChunk)
{
	List<int> list;
	list.InsertHead(3);
	list.InsertHead(2);
	list.InsertHead(1);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 3);
	EXPECT_EQ(list.Size(), 3);
	list.InsertHead(7);
	list.InsertHead(0);
	EXPECT_EQ(list.Head(), 0);
	EXPECT_EQ(list.Tail(), 3);
	EXPECT_EQ(list.Size(), 5);
}

TEST(List_TailInsertTest, AddTailChunk_IntTest)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 3);
	EXPECT_EQ(list.Size(), 3);
	list.InsertTail(4);
	list.InsertTail(5);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 5);
	EXPECT_EQ(list.Size(), 5);
}

TEST(List_TailInsertTest, AddTailChunk_ClassTest)
{
	List<TestClass> list;
	TestClass testElement1 = { 1, 1.1f };
	TestClass testElement2 = { 2, 2.2f };
	TestClass testElement3 = { 3, 3.3f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 3);
	EXPECT_EQ(list.Size(), 3);
	TestClass testElement4 = { 4, 4.4f };
	TestClass testElement5 = { 5, 5.5f };
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 5);
	EXPECT_EQ(list.Size(), 5);
}

TEST(List_HeadRemove, Test_ClassTest)
{
	List<TestClass> list;
	TestClass testElement1 = { 1, 1.1f };
	TestClass testElement2 = { 2, 2.2f };
	TestClass testElement3 = { 3, 3.3f };
	TestClass testElement4 = { 4, 4.4f };
	TestClass testElement5 = { 5, 5.5f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	list.RemoveHead();
	list.RemoveHead();
	list.RemoveHead();
	EXPECT_EQ(list.Head().id, 4);
	EXPECT_EQ(list.Tail().id, 5);
	EXPECT_EQ(list.Size(), 2);
}

TEST(List_TailRemove, Test_ClassTest)
{
	List<TestClass> list;
	TestClass testElement1 = { 1, 1.1f };
	TestClass testElement2 = { 2, 2.2f };
	TestClass testElement3 = { 3, 3.3f };
	TestClass testElement4 = { 4, 4.4f };
	TestClass testElement5 = { 5, 5.5f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	list.RemoveTail();
	list.RemoveTail();
	list.RemoveTail();
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 2);
	EXPECT_EQ(list.Size(), 2);
}

TEST(ListIterator, Test_HasNext_Next_Get)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 5);
	EXPECT_EQ(list.Size(), 5);
	auto it = list.GetIterator();
	EXPECT_EQ(it.Get(), 1);
	it.Next();
	EXPECT_EQ(it.Get(), 2);
	it.Next();
	EXPECT_EQ(it.Get(), 3);
	it.Next();
	it.Next();
	EXPECT_EQ(it.Get(), 5);
}

TEST(ListIterator, Test_Set)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 5);
	EXPECT_EQ(list.Size(), 5);
	auto it = list.GetIterator();
	it.Next();
	it.Next();
	it.Set(7);
	EXPECT_EQ(it.Get(), 7);
}

TEST(ListIterator_Remove1, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	auto it = list.GetIterator();
	it.Next();
	it.Next();
	it.Next();
	it.Remove();
	EXPECT_EQ(it.Get(), 5);//5
	it.Remove();
	EXPECT_EQ(it.Get(), 3);//3
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 3);
	it.Remove();
	EXPECT_EQ(it.Get(), 2);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 2);
	EXPECT_EQ(list.Size(), 2);
}

TEST(ListIterator_Remove2, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	auto it = list.GetIterator();
	it.Remove();
	it.Remove();
	EXPECT_EQ(it.Get(), 3);
	it.Next();
	it.Next();
	it.Remove();
	EXPECT_EQ(list.Head(), 3);
	EXPECT_EQ(list.Tail(), 4);
	EXPECT_EQ(it.Get(), 4);
	EXPECT_EQ(list.Size(), 2);
}

TEST(ListIterator_Remove1, Test_TestClass)
{
	List<TestClass> list;
	TestClass testElement1 = { 1, 1.1f };
	TestClass testElement2 = { 2, 2.2f };
	TestClass testElement3 = { 3, 3.3f };
	TestClass testElement4 = { 4, 4.4f };
	TestClass testElement5 = { 5, 5.5f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	auto it = list.GetIterator();
	it.Remove();
	it.Remove();
	EXPECT_EQ(it.Get().id, 3);
	it.Next();
	it.Next();
	it.Remove();
	EXPECT_EQ(list.Head().id, 3);
	EXPECT_EQ(list.Tail().id, 4);
	EXPECT_EQ(it.Get().id, 4);
	EXPECT_EQ(list.Size(), 2);
}

TEST(ListIterator_Remove2, Test_TestClass)
{
	List<TestStruct> list;
	TestStruct testElement1 = { "test1", 1, 1.1f };
	TestStruct testElement2 = { "test2", 2, 2.2f };
	TestStruct testElement3 = { "test3", 3, 3.3f };
	TestStruct testElement4 = { "test4", 4, 4.4f };
	TestStruct testElement5 = { "test5", 5, 5.5f };
	list.InsertTail(testElement1);
	list.InsertTail(testElement2);
	list.InsertTail(testElement3);
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	auto it = list.GetIterator();
	it.Remove();
	it.Remove();
	EXPECT_EQ(it.Get().id, 3);
	it.Next();
	it.Next();
	it.Remove();
	EXPECT_EQ(list.Head().id, 3);
	EXPECT_EQ(list.Tail().id, 4);
	EXPECT_EQ(it.Get().id, 4);
	EXPECT_EQ(list.Size(), 2);
}

TEST(ListIterator_HasPrev_Prev_Get, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	auto it = list.GetIterator();
	it.Next();
	it.Next();
	it.Next();
	it.Prev();
	it.Prev();
	EXPECT_EQ(it.Get(), 2);
}

TEST(ListIterator_Insert1, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(6);
	auto it = list.GetIterator();
	it.Next();
	it.Next();
	it.Next();
	it.Next();
	it.Insert(5);
	EXPECT_EQ(it.Get(), 5);
	it.Prev();
	it.Insert(7);
	EXPECT_EQ(it.Get(), 7);
	it.Next();
	EXPECT_EQ(it.Get(), 4);
	it.Next();
	EXPECT_EQ(it.Get(), 5);
	EXPECT_EQ(list.Size(), 7);
}

TEST(ListIterator_Insert2, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(4);
	list.InsertTail(5);
	auto it = list.GetIterator();
	it.Insert(0);
	EXPECT_EQ(it.Get(), 0);
	it.Next();
	EXPECT_EQ(it.Get(), 1);
	EXPECT_EQ(list.Size(), 6);
}

TEST(ListIterator_Insert3, Test_Int)
{
	List<int> list;
	list.InsertTail(1);
	list.InsertTail(2);
	list.InsertTail(3);
	list.InsertTail(5);
	auto it = list.GetIterator();
	it.Next();
	it.Next();
	it.Next();
	it.Insert(4);
	it.Next();
	EXPECT_EQ(it.Get(), 5);
	EXPECT_EQ(list.Size(), 5);
	EXPECT_EQ(list.Head(), 1);
	EXPECT_EQ(list.Tail(), 5);
}

TEST(ListIterator_StressTest)
{
	List<TestStruct> list;
	TestStruct testElement1 = { "test1", 1, 1.1f };
	TestStruct testElement2 = { "test2", 2, 2.2f };
	TestStruct testElement3 = { "test3", 3, 3.3f };
	TestStruct testElement4 = { "test4", 4, 4.4f };
	TestStruct testElement5 = { "test5", 5, 5.5f };
	list.InsertTail(testElement4);
	list.InsertTail(testElement5);
	list.InsertHead(testElement3);
	list.InsertHead(testElement2);
	list.InsertHead(testElement1);
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 5);
	list.RemoveHead();
	list.RemoveTail();
	EXPECT_EQ(list.Head().id, 2);
	EXPECT_EQ(list.Tail().id, 4);
	EXPECT_EQ(list.Size(), 3);
	auto it = list.GetIterator();
	it.Insert(testElement1);
	it.Next();
	it.Next();
	it.Next();
	it.Set(testElement5);
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 5);
	EXPECT_EQ(list.Size(), 4);
	it.Insert(testElement4);
	EXPECT_EQ(list.Head().id, 1);
	EXPECT_EQ(list.Tail().id, 5);
	EXPECT_EQ(it.Get().id, 4);
	it.Prev();
	it.Prev();
	EXPECT_EQ(it.Get().id, 2);
	it.Prev();
	it.Remove();
	EXPECT_EQ(it.Get().id, 2);
	it.Remove();
	EXPECT_EQ(it.Get().id, 3);
	it.Next();
	EXPECT_EQ(it.Get().id, 4);
	it.Next();
	EXPECT_EQ(it.Get().id, 5);
	EXPECT_EQ(list.Head().id, 3);
	EXPECT_EQ(list.Tail().id, 5);
	EXPECT_EQ(list.Size(), 3);
}