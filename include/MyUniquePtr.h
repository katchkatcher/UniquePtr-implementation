#include <iostream>
#include <cassert>

// deleter для обычных значений
template <typename T>
struct Default_deleter
{
	void operator()(T* ptr)
	{
		std::cout << "Delete T object. . .\n";
		delete ptr;
	};
};

// deleter для массива 
template <typename T>
struct Default_deleter<T[]>
{
	void operator()(T* ptr)
	{
		std::cout << "Delete T[] object. . .\n";
		delete[] ptr;
	};
};

template <typename T, typename Deleter = Default_deleter<T>>
class UniquePtr
{
private:
	// Сырой указатель
	T* ptr;
	// Объект удалителя
	Deleter deleter;

	// Запрет копирования
	UniquePtr(const UniquePtr&) = delete;
	void operator=(const UniquePtr&) = delete;

public:

	// Конструкторы

	// По умолчанию
	explicit UniquePtr() noexcept : ptr(nullptr) {};

	// Принимает объект
	explicit UniquePtr(T* object_ptr) noexcept : ptr(object_ptr) {};

	// Конструктор перемещения
	UniquePtr(UniquePtr<T, Deleter>&& relocatable_obj) noexcept
		: ptr(relocatable_obj.ptr), deleter(std::move(relocatable_obj.deleter))
	{
		relocatable_obj.ptr = nullptr;
	};

	// Деструктор
	~UniquePtr()
	{
		if (ptr) deleter(ptr);
	};

	// Операторы

	// Оператор разыменования
	T& operator*()
	{
		assert(ptr != nullptr);
		return *ptr;
	};

	// Константный оператор разыменования
	const T& operator*() const
	{
		assert(ptr != nullptr);
		return *ptr;
	};

	// Оператор доступа к объекту
	T* operator->()
	{
		assert(ptr != nullptr);
		return ptr;
	};

	// Константный оператор доступа к объекту
	const T* operator->() const
	{
		assert(ptr != nullptr);
		return ptr;
	};

	// Оператор перемещающего присваивания
	UniquePtr<T, Deleter>& operator = (UniquePtr<T, Deleter>&& rhs) noexcept
	{
		if (this != &rhs)
		{
			reset(rhs.release());
			deleter = std::move(rhs.deleter);
		}
		return *this;
	}

	// Передаёт владение указателем вручную, обнуляя внутренний
	T* release() noexcept
	{
		T* getptr = this->ptr;
		this->ptr = nullptr;
		return getptr;
	};

	// Получение указателя
	T* get() const { return ptr; };

	// Меняет указатель
	void reset(T* new_ptr = nullptr) noexcept
	{
		T* old_ptr = std::exchange(ptr, new_ptr);
		if (old_ptr) deleter(old_ptr);
	}

	// Меняет указатели местами
	void swap(UniquePtr<T, Deleter>& second_ptr) noexcept
	{
		std::swap(ptr, second_ptr.ptr);
		std::swap(deleter, second_ptr.deleter);
	};
};

// Для одиночных объектов
template <typename T, typename... Args>
std::enable_if_t<!std::is_array_v<T>, UniquePtr<T>> make_unique(Args&&... args) {
	return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// Независимая функция swap
template <typename T, typename Deleter>
void swap(UniquePtr<T, Deleter>& lhs, UniquePtr<T, Deleter>& rhs) noexcept
{
	lhs.swap(rhs);
}

// UniquePtr для динамических массивов
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter>
{
private:
	T* arr_ptr;

	// Запреты
	UniquePtr(const UniquePtr<T[]>&) = delete;
	void operator=(const UniquePtr<T[]>&) = delete;
	T* operator->() = delete;
	const T* operator->() const = delete;
	T& operator*() = delete;
	const T& operator*() const = delete;

protected:

	Deleter deleter;

public:

	// Конструкторы

	// По умолчанию
	explicit UniquePtr(T* ptr = nullptr) noexcept : arr_ptr(ptr) {};

	// Перемещающий конструктор
	UniquePtr(UniquePtr<T[], Deleter>&& relocatable_arr) noexcept
		: arr_ptr(relocatable_arr.arr_ptr), deleter(std::move(relocatable_arr.deleter))
	{
		relocatable_arr.arr_ptr = nullptr;
	};

	// Деструктор
	~UniquePtr() { if (arr_ptr) deleter(arr_ptr); };

	// Оператор доступа по индексу
	T& operator [](size_t index) const
	{
		assert(arr_ptr != nullptr) ;
		return arr_ptr[index];
	};

	void reset(T* new_arr_ptr = nullptr) noexcept
	{
		T* old_arr_ptr = std::exchange(arr_ptr, new_arr_ptr);
		if (old_arr_ptr) deleter(old_arr_ptr);
	}

	T* release() noexcept
	{
		T* get_arr_ptr = arr_ptr;
		arr_ptr = nullptr;
		return get_arr_ptr;
	};

	void swap(UniquePtr<T[], Deleter>& second_ptr) noexcept
	{
		std::swap(arr_ptr, second_ptr.arr_ptr);
		std::swap(deleter, second_ptr.deleter);
	};
	
};

// make_unique для массивов
template <typename T>
std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> == 0, UniquePtr<T>> make_unique(std::size_t size) {
	using ElementType = std::remove_extent_t<T>;
	return UniquePtr<T>(new ElementType[size]());
}