
#pragma once

#include <pch.h>

#include <arch/datatypes/type_sparseset.h>
#include <arch/components/componentList.h>
#include <arch/common/component.h>
#include <arch/common/sparseSetViewHandles.h>

#include <typeindex>
#include <functional>

#include <arch/common/entity.h>

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

	auto begin()		{ return m_compPool.begin(); }
	auto end()		{ return m_compPool.end(); }


	bool Add(EntityID _addTo);
	bool Remove(EntityID _removeFrom);

	bool ComponentExistsForEntity(const EntityID& _id) { return static_cast<bool>(m_compPool.At(_id)); }

	ComponentView<T> Get(EntityID _client);
	ComponentView<const T> Get(EntityID _client) const;
	size_t size() const override { return m_compPool.size(); };

	void clear() { m_compPool.clear(); };

};


// metadata component info class.
struct ComponentMetadata {
public:
	using CompType = std::type_index;
	using CompTypeID = unsigned;
public:
	ComponentMetadata(
		std::string _name,
		CompType _compId,
		CompTypeID _id,
		std::shared_ptr<IComponentPool> _pool,
		std::function<void()> _registerFunction
	) :
		m_componentName		{ _name },
		m_componentType		{ _compId },
		m_componentPool		{ _pool },
		m_registerFunction	{ _registerFunction },
		m_componentTypeID	{_id}
	{}

	const std::string& GetComponentName() const						{ return m_componentName; }
	const CompType& GetComponentType() const						{ return m_componentType; };
	const CompTypeID& GetComponentTypeID() const						{ return m_componentTypeID; };

	std::shared_ptr<IComponentPool> GetComponentPool()				{ return m_componentPool; }
	std::shared_ptr<const IComponentPool> GetComponentPool() const	{ return m_componentPool; }


	std::function<void()> GetComponentRegisterFunction()			{ return m_registerFunction; }

private:
	CompTypeID	m_componentTypeID;
	CompType	m_componentType;
	std::string m_componentName;
	std::shared_ptr<IComponentPool> m_componentPool;
	std::function<void()> m_registerFunction;
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
		

		CompTypeID componentId{ typeid(T) };
		std::string componentName = componentId.name();
		std::shared_ptr<IComponentPool> pool = std::make_shared<ComponentPool<T>>();
		std::function<void()> registerFunction = T::Register;

		// setup to lookup and metadata.
		ComponentMetadata::CompTypeID id = ++s_componentIdxId;
		m_componentIDLookup.Add(typeid(T), id);
		m_componentMetadataMap.insert({ 
			componentId, ComponentMetadata(
				componentName, 
				componentId, id, 
				pool, 
				registerFunction
			) }
		);




		ComponentMetadata& compInfo = m_componentMetadataMap.at(componentId);
		std::string log{"Registering Component: "};
		log += componentId.name();
		

		LOG_INFO(log);
	}

	void RegisterComponentsForSerialisation() {
		for (auto& [UNUSED, info] : m_componentMetadataMap) {
			std::function<void()> fn = info.GetComponentRegisterFunction();
			fn();
		}
	}

	void PrintDebugInfo() const;

	template <typename T>
	SparseSetView<ComponentPool<T>> GetComponentPool();

	template <typename T>
	SparseSetView<const ComponentPool<T>> GetComponentPool() const;

	std::deque<Entity>& GetEntityList()					{ return m_entityList.Data(); }
	const std::deque<Entity>& GetEntityList() const		{ return m_entityList.Data(); }
	

	EntityView Get(const EntityID& _id)					{ return m_entityList.At(_id); };
	EntityViewConst Get(const EntityID& _id)	const   { return m_entityList.At(_id); };

	
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
	EntityView Instantiate();


	// - existence checks -------------------------------------------------
	template <typename T>
	bool ComponentPoolExists();


	std::string name;
private:

	void Clear();
	// map would be a better idea
	SparseSet<EntityID, Entity> m_entityList; // 

private:
	using CompTypeID = std::type_index;
	using CompID = unsigned;

	// 1 lookup integer mapped to 1 
	SparseSet<CompID, CompTypeID> m_componentIDLookup;
	std::unordered_map<CompTypeID, ComponentMetadata> m_componentMetadataMap;

	inline static CompID s_componentIdxId{ 0 }; // 0 is invalid.

};




#include <arch/ecs/ecs_registryDEF.hpp>