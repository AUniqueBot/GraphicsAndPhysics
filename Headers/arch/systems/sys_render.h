#pragma once
#include <pch.h>
#include <arch/common/system.h>

class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	void Init()		override;


	void Start()	override { std::cout << typeid(RenderSystem).name() << "::Start" << std::endl;};


	void PreUpdate() override;
	void Update()	override;
	void Stop()		override { std::cout << typeid(RenderSystem).name() << "::Stop" << std::endl; };
	void Cleanup()	override { std::cout << typeid(RenderSystem).name() << "::Cleanup" << std::endl; };
private:



};



