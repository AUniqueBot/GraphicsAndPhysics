#pragma once
#include <arch/common/system.h>
#include <glm/glm.hpp>


class TransformSystem final : public System, public Singleton<TransformSystem> {
public:
	void Init()		override { LOG_INFO("Init"); };


	void Start()	override;
	void Update()	override;
	void Stop()		override { LOG_INFO("Stop"); };
	void Cleanup()	override { LOG_INFO("Cleanup"); };

	
private:



};