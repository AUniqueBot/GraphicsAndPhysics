
#pragma once

#include <pch.h>

#include <arch/datatypes/type_sparseset.h>
#include <arch/components/componentList.h>
#include <arch/common/component.h>
#include <arch/datatypes/type_sparseSet_viewHandle.h>

#include <typeindex>
#include <functional>

#include <util/util_serialisation.h>
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

	virtual bool Add(EntityID _addTo) = 0;
	virtual bool Remove(EntityID _removeFrom) = 0;
};




template<std::derived_from<Component> T>
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
	auto end()			{ return m_compPool.end(); }


	bool Add(EntityID _addTo) override;
	bool Remove(EntityID _removeFrom) override;

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
		std::function<void()> _registerFunction
	) :
		m_componentName		{ _name },
		m_componentType		{ _compId },
		m_registerFunction	{ _registerFunction },
		m_componentTypeID	{_id}
	{}

	const std::string& GetComponentName() const						{ return m_componentName; }
	const CompType& GetComponentType() const						{ return m_componentType; };
	const CompTypeID& GetComponentTypeID() const					{ return m_componentTypeID; };



	std::function<void()> GetComponentRegisterFunction()			{ return m_registerFunction; }

private:
	CompTypeID	m_componentTypeID;
	CompType	m_componentType;
	std::string m_componentName;
	//std::shared_ptr<IComponentPool> m_componentPool;
	std::function<void()> m_registerFunction;
};

struct ComponentPackedData {
	ComponentMetadata m_componentMetadata;
	std::shared_ptr<IComponentPool> m_componentPool;
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
	template<std::derived_from<Component> T>
	void RegisterComponent() {
		// create a sparse set of type T
		

		CompTypeID componentId		{ typeid(T) };
		std::string componentName	{ Reflection::TypeName<T>() };

		ComponentMetadata::CompTypeID id = ++s_componentIdxId;
		std::shared_ptr<IComponentPool> pool = std::make_shared<ComponentPool<T>>();
		std::function<void()> registerFunction = T::Register;
		
		ComponentMetadata cmdata = ComponentMetadata(
			componentName,
			componentId, id,
			registerFunction
		);
			
		// setup to lookup and metadata.
		m_componentIDLookup.Add(typeid(T), id);
		m_componentData.insert({ componentId, ComponentPackedData{ cmdata, pool } });

		
		std::string log{"Registering Component: "};
		log += Reflection::TypeName<T>();

		LOG_INFO(log);
	}

	void RegisterComponentsForSerialisation() {
		for (auto& [_, packedData] : m_componentData) {
			
			std::function<void()> fn = packedData.m_componentMetadata.GetComponentRegisterFunction();
			fn();
		}
	}

	void PrintDebugInfo() const;

	template<std::derived_from<Component> T>
	SparseSetView<ComponentPool<T>> GetComponentPool();

	template<std::derived_from<Component> T>
	SparseSetView<const ComponentPool<T>> GetComponentPool() const;

	std::deque<Entity>& GetEntityList()					{ return m_entityList.Data(); }
	const std::deque<Entity>& GetEntityList() const		{ return m_entityList.Data(); }
	

	EntityView GetEntity(const EntityID& _id)				{ return m_entityList.At(_id); };
	EntityViewConst GetEntity(const EntityID& _id)	const   { return m_entityList.At(_id); };

	
	// component handling.

	///! @brief adds specified component to the entity given.
	///! @tparam T: Component Type
	///! @param _addTo: entity id to add a component to
	///! @return true if successful.
	template<std::derived_from<Component> T>
	bool AddComponent(EntityID _addTo);


	///! @brief removes specified component from the entity given.
	///! @tparam T: Component Type
	///! @param _removeFrom: entity id to remove component from 
	///! @return true if successful.
	template<std::derived_from<Component> T>
	bool RemoveComponent(EntityID _removeFrom);


	// creates an entity.
	EntityView Instantiate();

	///! @brief destroys the specified entity
	///! @param Entity: entity to remove
	///! @param EntityID: ALT entityID to remove
	///! @param
	void Destroy(Entity _remove, bool _recursiveDeleteChildren = true);
	void Destroy(EntityID _remove, bool _recursiveDeleteChildren = true);


	// - existence checks -------------------------------------------------

	///! @brief checks if a component pool of type T exists
	///! @tparam T: Component type
	///! @return true if exists
	template<std::derived_from<Component> T>
	bool ComponentPoolExists() const;


	void SelectEntity(EntityID _selectedObject, bool _additive = false);
	const std::vector<EntityID>& SelectedEntities() const;
	EntityID SelectedEntity() const;
	
	// remove 1 from selection
	void DeselectEntity(EntityID _selectedObject);
	// remove all from selection.
	void ClearSelection();
	bool EntityIsSelected(EntityID _id, bool _isCurrentSelection = true) const;


private:

	void Clear();
	// map would be a better idea
	SparseSet<EntityID, Entity> m_entityList;
	std::vector<EntityID> m_selectedEntitiesList;
private:
	using CompTypeID = std::type_index;
	using CompID = unsigned;

	// 1 lookup integer mapped to 1 
	SparseSet<CompID, CompTypeID> m_componentIDLookup;
	std::unordered_map<CompTypeID, ComponentPackedData> m_componentData;


	inline static CompID s_componentIdxId{ 0 }; // 0 is invalid.

};




#include <arch/ecs/ecs_registryDEF.hpp>

