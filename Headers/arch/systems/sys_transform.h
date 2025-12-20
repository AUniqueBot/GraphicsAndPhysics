#pragma once
#include <arch/common/system.h>
#include <glm/glm.hpp>


class TransformSystem final : public System, public Singleton<TransformSystem> {
public:
	void Init()		override { std::cout << typeid(TransformSystem).name() << ": Init" << std::endl; };


	void Start()	override;
	void Update()	override;
	void Stop()		override {std::cout << typeid(TransformSystem).name() << ":Stop" << std::endl; };
	void Cleanup()	override {std::cout << typeid(TransformSystem).name() << ":Cleanup" << std::endl; };

	
private:



};