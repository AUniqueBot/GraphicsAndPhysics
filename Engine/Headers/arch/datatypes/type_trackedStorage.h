#pragma once
#include <queue>
#include <functional>
#include <arch/datatypes/type_sparseSet.h>

using TrackedID = uint64_t;
inline constexpr TrackedID C_TRACKEDID_INVALID = 0;


namespace {

	template<typename T>
	using ReleaseFunction = std::function<void(T&)>;

	template<typename T>
	static ReleaseFunction<T> CreateReleaseFunction() {
		if constexpr (requires(T & resource) {
			resource.Destroy();
		}) {
			return [](T& resource) {
				resource.Destroy();
				};
		}
		else {
			return nullptr;
		}
	}
}

template <typename T>
class TrackedStorage {
public:
	TrackedStorage(
		std::function<void(T&)> _releaseFunction = CreateReleaseFunction<T>()
	) : m_releaseFunction(_releaseFunction) {};

public:
	SparseSetView<T> GetResource(TrackedID _id) {
		return m_storage.At(_id);
	}
	SparseSetView<const T> GetResource(TrackedID _id) const {
		return m_storage.At(_id);
	}

	TrackedID AddResource(T&& _data) {
		TrackedID id = AllocateID();
		m_storage.Add(std::move(_data), id);
		return id;
	}

	bool RemoveResource(TrackedID _id) {
		SparseSetView<T> view = m_storage.At(_id);
		if (!view) return false;
		if (m_releaseFunction) {
			m_releaseFunction(*view);
		}
		bool result = m_storage.Remove(_id);
		if (result) ReclaimID(_id);
		return result;

	}

	void Clear() {
		if (m_releaseFunction) {
			for (T& res : m_storage) {
				m_releaseFunction(res);
			}
		}
		m_storage.clear();
		m_resIdCounter = 0;
		m_freeIds = {};
	}

	void SetDestroyFunction(std::function<void(T&)> _function) {
		m_releaseFunction = _function;
	}


private:
	TrackedID AllocateID() {
		TrackedID idToRet = C_TRACKEDID_INVALID;
		if (!m_freeIds.empty()) {
			idToRet = m_freeIds.front();
			m_freeIds.pop();
		}
		else {
			idToRet = ++m_resIdCounter;
		}
		return idToRet;

	}
	void ReclaimID(TrackedID _id) {
		if (_id == C_TRACKEDID_INVALID) return;
		m_freeIds.push(_id);
	}


private:
	// counter mechanism
	TrackedID m_resIdCounter{};
	std::queue<TrackedID> m_freeIds;

	// storage
	SparseSet<TrackedID, T> m_storage;
	std::function<void(T&)> m_releaseFunction;
};