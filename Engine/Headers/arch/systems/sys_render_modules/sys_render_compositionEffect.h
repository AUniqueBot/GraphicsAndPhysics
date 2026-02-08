#pragma once



class CompositionEffect {
public: 
	virtual void Render() = 0;
	void SetInputTexture();
private:
	// input texture.


};