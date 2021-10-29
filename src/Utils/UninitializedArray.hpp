#pragma once

template <typename T, size_t size>
class UninitializedArray
{
public:
	UninitializedArray() {};

	T& operator[] (size_t index);
	T* data();

private:
	alignas(alignof(T)) unsigned char m_data[sizeof(T) * size];
};

template<typename T, size_t size>
T& UninitializedArray<T, size>::operator[](size_t index)
{
	return reinterpret_cast<T*>(m_data)[index];
}

template<typename T, size_t size>
T* UninitializedArray<T, size>::data()
{
	return reinterpret_cast<T*>(m_data);
}
