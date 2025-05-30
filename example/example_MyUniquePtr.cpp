#include <iostream>
#include "MyUniquePtr.h"
#include <memory>
#include <cassert>

// Пример структуры для тестирования
struct Test {
	int x;

	Test(int val) : x(val) {};
	Test() :x(0){};

	void print() const {
		std::cout << "Test x: " << x << std::endl;
	}

	void set(int val) {
		x = val;
	}
};

int main()
{
	setlocale(LC_ALL, "rus");

	// Тестирование умного указателя для обычных значений

	{
		UniquePtr<int> num;

		UniquePtr<int> num1(new int(111));
		std::cout << "num1: " << *num1 << "\n";

		UniquePtr<int> num2(nullptr);

		UniquePtr<int>test1(new int(222));
		UniquePtr<int>num3(std::move(test1));
		std::cout << "num3: " << *num3 << "\n";

		{
			// Работа с объектами класса
			UniquePtr<Test> ptr1(new Test(5));

			ptr1->set(10);
			std::cout << "Test 2: После изменения ptr1->x = " << ptr1->x << std::endl;

			// Тест 3: Проверка оператора * для константного объекта
			const UniquePtr<Test> ptr2(new Test(20));
			std::cout << "Test 3: Разыменование *ptr2 -> x = " << (*ptr2).x << std::endl;

			// Тест 4: Проверка оператора -> для константного объекта
			ptr2->print();
		}

		const UniquePtr<int> num4(new int(444));
		std::cout << "num4 const ptr: " << *num4 << "\n";

		UniquePtr<int> num5(new int(555));
		UniquePtr<int> num6;
		num6 = std::move(num5);
		std::cout << "num6 relocated obj: " << *num6 << "\n";

		UniquePtr<int> num7(new int(888));
		UniquePtr<int> num8;
		num8.reset(num7.release());
		std::cout << "num8 reset value: " << *num8 << "\n";
		num8.reset(new int(43));
		std::cout << "num8 second reset value: " << *num8 << "\n";


		std::cout << "Get testing (num8): " << *(num8).get() << "\n";

		UniquePtr<int> num9(new int(99));
		UniquePtr<int> num10(new int(88));
		std::cout << "num9 before swap: " << *num9 << "\n";
		std::cout << "num10 before swap: " << *num10 << "\n";
		num9.swap(num10);
		std::cout << "num9 after swap: " << *num9 << "\n";
		std::cout << "num10 after swap: " << *num10 << "\n";
		swap(num9, num10);
		std::cout << "num9 after reswap: " << *num9 << "\n";
		std::cout << "num10 after reswap: " << *num10 << "\n";

		auto num11 = make_unique<int>(999);
		std::cout << "num11 maked by make_unique: " << *num11 << "\n";

	}


	// Тестирование умного указателя для массивов

	{
		UniquePtr<int[]> arr(new int[2] { 1, 2});
		std::cout << "arr: ";
		for (int i = 0; i < 2; i++)
		{
			std::cout << arr[i] << " ";
		}
		std::cout << "\n";

		UniquePtr<int[]> test1(new int[3] {6, 6, 6});
		UniquePtr<int[]> arr2(std::move(test1));
		std::cout << "arr2: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr2[i] << " ";
		}
		std::cout << "\n";


		int* test2{ new int[3] {7,7,7} };
		UniquePtr<int[]>arr3(test2);
		std::cout << "arr3: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr3[i] << " ";
		}
		std::cout << "\n";


		int* test3{ new int[3] {8,8,8} };
		UniquePtr<int[]> arr4(new int[3] {9, 9, 9});
		std::cout << "arr4 before reset: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr4[i] << " ";
		}
		std::cout << "\n";
		arr4.reset(test3);
		std::cout << "arr4 after reset: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr4[i] << " ";
		}
		std::cout << "\n";

		UniquePtr<int[]> arr5(new int[3] {10, 10, 10});
		UniquePtr<int[]> arr6(new int[3] {11, 11, 11});
		std::cout << "arr5 before swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr5[i] << " ";
		}
		std::cout << "\n";
		std::cout << "arr6 before swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr6[i] << " ";
		}
		std::cout << "\n";

		arr5.swap(arr6);
		std::cout << "arr5 after swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr5[i] << " ";
		}
		std::cout << "\n";
		std::cout << "arr6 after swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr6[i] << " ";
		}
		std::cout << "\n";

		swap(arr5, arr6);
		std::cout << "arr5 after swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr5[i] << " ";
		}
		std::cout << "\n";
		std::cout << "arr6 after swap: ";
		for (int i = 0; i < 3; i++)
		{
			std::cout << arr6[i] << " ";
		}
		std::cout << "\n";

		UniquePtr<int[]> arr7(nullptr);
		UniquePtr<int[]> arr8(new int[3]);

		auto arr9 = make_unique<int[]>(5);
		std::cout << "arr9 member maked by make_unique: " << arr9[1] << "\n";
	}

	return 0;
}