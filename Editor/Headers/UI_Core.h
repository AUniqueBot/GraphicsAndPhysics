#pragma once

struct GLFWwindow;

class UI_Core {


public:
	void Init(unsigned _major, unsigned _minor, GLFWwindow* _window);



	void Update();

	void Exit();


private:


};