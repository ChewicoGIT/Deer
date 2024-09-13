#include "Entity.h"
#include "Deer/Core/Core.h"

namespace Deer {
	Entity::Entity(entt::entity handle, Environment* scene)
		: m_entityHandle(handle), m_environment(scene) {
	}

	bool Entity::removeChild(Entity& child) {
		DEER_CORE_ASSERT(child.m_environment == m_environment, "Can not remove childrens from diferent enviroments");

		std::vector<entt::entity>& children = getChildren();
		auto it = std::find(children.begin(), children.end(), child.m_entityHandle);
		if (it != children.end())
		{
			children.erase(it);
			return true;
		}

		return false;
	}

	void Entity::setParent(Entity& parent) {
		DEER_CORE_ASSERT(parent.m_environment == m_environment , "Can not set parent from diferent enviroments");
		Entity current_parent = getParent();

		DEER_CORE_ASSERT(!isRoot(), "Can not set parent to root");
		DEER_CORE_ASSERT(parent.isValid(), "Parent is not valid");

		if (current_parent == parent)
			return;

		if (current_parent.isValid())
			current_parent.removeChild(*this);

		getComponent<RelationshipComponent>().parent_handle = parent.m_entityHandle;
		parent.getChildren().push_back(m_entityHandle);
	}

	bool Entity::isDescendant(Entity& parent)
	{
		if (*this == parent)
			return true;
		
		if (isRoot())
			return false;

		return getParent().isDescendant(parent);
	}

	Entity Entity::duplicate() {
		Entity creation = m_environment->createEntity(getComponent<TagComponent>().tag + " (duplicated)");

		creation.getComponent<TransformComponent>() = getComponent<TransformComponent>();
		Entity parent = m_environment->tryGetEntity((uid)getComponent<RelationshipComponent>().parent_handle);
		creation.setParent(parent);

		if (m_environment->m_registry.any_of<MeshRenderComponent>(m_entityHandle))
			creation.addComponent<MeshRenderComponent>(getComponent<MeshRenderComponent>());

		if (m_environment->m_registry.any_of<CameraComponent>(m_entityHandle))
			creation.addComponent<CameraComponent>(getComponent<CameraComponent>());

		return creation;
	}

	void Entity::destroy() {
		DEER_CORE_ASSERT(!isRoot(), "Can not destroy the root");
		getParent().removeChild(*this);

		for (auto entt : getChildren()) {
			m_environment->tryGetEntity((uid)entt).destroy();
		}

		m_environment->m_registry.destroy(m_entityHandle);
		m_entityHandle = entt::null;
		m_environment = nullptr;
	}

	Entity Entity::getParent() {
		auto& rc = getComponent<RelationshipComponent>();
		if (rc.parent_handle == entt::null)
			return Entity();
		return Entity(rc.parent_handle, m_environment);
	}

	entt::entity Entity::getParentUID() {
		return getComponent<RelationshipComponent>().parent_handle;
	}

	void Entity::setParentUID(entt::entity parentUid) {
		getComponent<RelationshipComponent>().parent_handle = parentUid;
	}
	
	std::vector<entt::entity>& Entity::getChildren() {
		return getComponent<RelationshipComponent>().children;
	}

	glm::mat4 Entity::getWorldMatrix()
	{
		if (isRoot())
			return glm::mat4(1.0f);
		// To do, apply recursive matrix
		return getParent().getWorldMatrix() * getRelativeMatrix();
	}

	glm::mat4 Entity::getRelativeMatrix()
	{
		return getComponent<TransformComponent>().getMatrix();
	}
}

