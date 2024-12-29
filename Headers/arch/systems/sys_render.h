#pragma once
#include <pch.h>
#include <arch/common/system.h>

class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	void Init()		override { std::cout << typeid(RenderSystem).name() <<"::Init" << std::endl; };


	void Start()	override { std::cout << typeid(RenderSystem).name() << "::Start" << std::endl;};
	void Update()	override { std::cout << typeid(RenderSystem).name() << "::Update" << std::endl;};
	void Stop()		override { std::cout << typeid(RenderSystem).name() << "::Stop" << std::endl; };
	void Cleanup()	override { std::cout << typeid(RenderSystem).name() << "::Cleanup" << std::endl; };
private:



};