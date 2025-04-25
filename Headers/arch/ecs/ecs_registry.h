#pragma once
#include <pch.h>

#include <arch/datatypes/type_sparseset.h>

#include <arch/common/entity.h>
#include <arch/common/entityid.h>
#include <arch/components/componentList.h>
#include <arch/common/component.h>

#include <typeindex>
#include <functional>

#define TEMPLATE_BASECOMPONENT template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true> 




// component pool class
// ------------------------------------------------------------------------------------
// base class to utilise polymorphism, pure virtual class.
class IComponentPool {
public:
	IComponentPool() = default;
	virtual ~IComponentPool() = default;// pure virtual.
	virtual size_t size() const { return 3; }
};


template <typename T>
class ComponentPool : public IComponentPool {

	// data struct containing all components here.
	SparseSet<EntityID, T> m_compPool;

public:
	ComponentPool() {};
	~ComponentPool() override { };
	///! @brief get the data of the component pool.
	///! @return 
	SparseSet<EntityID, T>& Data();
	const SparseSet<EntityID, T>& Data() const;


	bool Add(EntityID _addTo);
	bool Remove(EntityID _removeFrom);

	size_t size() const override { return m_compPool.size(); };

};

// ------------------------------------------------------------------------------------
class EntityRegistry {

	friend class Entity;
	friend class EntityID;

public:

	EntityRegistry() = default;
	~EntityRegistry();
	
	///! @brief registers a component for use.
	///! @tparam T: Component
	///! @note since the registration of components happens here and it filters out non-components, only this function has a component check
	TEMPLATE_BASECOMPONENT
	void RegisterType() {
		// create a sparse set of type T
		CompType currentComponent{ typeid(T) };
		m_componentPool[currentComponent] = std::make_shared<ComponentPool<T>>();
		m_compRegisterFunctions[currentComponent] = T::Register;


		std::cout << __FUNCSIG__ << ": registering component: " << currentComponent.name() << std::endl;

	}

	void RegisterComponentsForSerialisation() {
		for (auto& [UNUSED, func] : m_compRegisterFunctions)
			func();
	}

	void PrintDebugInfo() const;

	template <typename T>
	std::optional<std::reference_wrapper<ComponentPool<T>>> GetComponentPool();

	template <typename T>
	std::optional<std::reference_wrapper<const ComponentPool<T>>> GetComponentPool() const;


	
	// component handling.

	///! @brief adds specified component to the entity given.
	///! @tparam T: Component Type
	///! @param _addTo: entity id to add a component to
	///! @return true if successful.
	template <typename T>
	bool AddComponent(EntityID _addTo);


	///! @brief removes specified component from the entity given.
	///! @tparam T: Component Type
	///! @param _removeFrom: entity id to remove component from 
	///! @return true if successful.
	template <typename T>
	bool RemoveComponent(EntityID _removeFrom);


	// creates an entity.
	std::optional<std::reference_wrapper<Entity>> Instantiate();



private:

	void Clear();

	
	SparseSet<EntityID, Entity> m_entityList;

private:
	using CompType = std::type_index;
	std::unordered_map<CompType, std::function<void()>> m_compRegisterFunctions;
	std::unordered_map<CompType, std::shared_ptr<IComponentPool>> m_componentPool;


};








#include <arch/ecs/ecs_registryDEF.hpp>