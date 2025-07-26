
#pragma once

#include <pch.h>

#include <arch/datatypes/type_sparseset.h>
#include <arch/components/componentList.h>
#include <arch/common/component.h>

#include <typeindex>
#include <functional>



class Entity;
class EntityID;



// component pool class
// ------------------------------------------------------------------------------------
// base class to utilise polymorphism
class IComponentPool {
public:
	IComponentPool() = default;
	virtual ~IComponentPool() = default;
	virtual size_t size() const { return 0; } // test value 3.
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

	auto begin()	{ return m_compPool.begin(); }
	auto end()		{ return m_compPool.end(); }


	bool Add(EntityID _addTo);
	bool Remove(EntityID _removeFrom);

	std::optional<std::reference_wrapper<T>> Get(EntityID _client);
	std::optional<std::reference_wrapper<const T>> Get(EntityID _client) const;

	size_t size() const override { return m_compPool.size(); };

	void clear() { m_compPool.clear(); };

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
	template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
	void RegisterType() {
		// create a sparse set of type T
		CompType currentComponent{ typeid(T) };
		m_componentPool.insert({ currentComponent,   std::make_shared<ComponentPool<T>>() });
		m_compRegisterFunctions.insert({ currentComponent, T::Register });

		std::string log{"Registering Component: "};
		log += currentComponent.name();
		LOG_INFO(log);
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

	std::deque<Entity>& GetEntityList()					{ return m_entityList.Data(); }
	const std::deque<Entity>& GetEntityList() const		{ return m_entityList.Data(); }
	

	std::optional<std::reference_wrapper<Entity>> Get(const EntityID& _id)			   { return m_entityList.At(_id); };
	std::optional<std::reference_wrapper<const Entity>> Get(const EntityID& _id) const { return m_entityList.At(_id); };

	
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

	std::string name;
private:

	void Clear();


	// map would be a better idea
	SparseSet<EntityID, Entity> m_entityList; // 

private:
	using CompType = std::type_index;
	std::unordered_map<CompType, std::function<void()>> m_compRegisterFunctions;
	std::unordered_map<CompType, std::shared_ptr<IComponentPool>> m_componentPool;


};




#include <arch/ecs/ecs_registryDEF.hpp>