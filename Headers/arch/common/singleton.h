#pragma once

// simple singleton class.
// used for systems where having one 


template <typename T>
class Singleton {
public:
	Singleton()						= default;
	~Singleton()					= default;
	Singleton(const Singleton<T>&)	= delete;

	static T& GetInstance() { static T s_instance;  return s_instance; };

protected:

private:

};