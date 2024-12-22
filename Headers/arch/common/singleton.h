#pragma once

// simple singleton class.
// used for systems where having one 


template <typename T>
class Singleton {
public:
	Singleton()						= default;
	~Singleton()					= default;
	Singleton(const Singleton<T>&)	= delete;

	T& GetInstance() { return m_instance; };

private:
	static T m_instance;
};