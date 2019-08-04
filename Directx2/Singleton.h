#pragma once

template<class T>
class Singleton {

public:
	static T* instance = nullptr;

	T* Instance() {
		if (instance == nullptr)
		{
			instance = new T();
		}
		return instance;
	}

protected:
	Singleton() {}
	~Singleton() {}


private:
	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};
};