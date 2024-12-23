#pragma once
#include <arch/common/system.h>
#include <glm/glm.hpp>


class TransformSystem final : public System, public Singleton<TransformSystem> {
public:
	void Init()		override { std::cout << "TRS::Init" << std::endl; };


	void Start()	override;
	void Update()	override;
	void Stop()		override {std::cout << "TRS::Stop" << std::endl; };
	void Cleanup()	override {std::cout << "TRS::Cleanup" << std::endl; };
private:



};