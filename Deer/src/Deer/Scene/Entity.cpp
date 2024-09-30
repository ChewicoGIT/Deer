#include "Entity.h"
#include "Deer/Core/Core.h"

namespace Deer {
	Entity::Entity(entt::entity handle, Environment* scene)
		: m_entityHandle(handle), m_environment(scene) {
	}

	bool Entity::removeChild(Entity& child) {
		DEER_CORE_ASSERT(child.m_environment == m_environment, "Can not remove childrens from diferent enviroments");

		std::vector<uid>& children = getChildren();
		auto it = std::find(children.begin(), children.end(), child.m_entityUID);
		if (it != children.end())
		{
			children.erase(it);
			return true;
		}

		return false;
	}

	void Entity::setParent(Entity& parent) {
		DEER_CORE_ASSERT(parent.m_environment == m_environment , "Can not set parent from diferent enviroments");
		DEER_CORE_ASSERT(!isRoot(), "Can not set parent to root");
		DEER_CORE_ASSERT(parent.isValid(), "Parent is not valid");

		if (m_parentUID == parent.m_entityUID)
			return;

		if (m_parentUID != 0){
			Entity& current_parent = getParent();

			if (parent.isDescendant(*this)) {
				return;
			}

			current_parent.removeChild(*this);
		}

		m_parentUID = parent.m_entityUID;
		getComponent<RelationshipComponent>().parent_UID = parent.m_entityUID;
		parent.getChildren().push_back(m_entityUID);
	}

	bool Entity::isDescendant(Entity& parent)
	{
		if (m_entityUID == parent.m_entityUID)
			return true;
		
		if (isRoot())
			return false;

		return getParent().isDescendant(parent);
	}

	Entity& Entity::duplicate() {
		Entity& creation = m_environment->createEntity(getComponent<TagComponent>().tag + " (duplicated)");

		creation.getComponent<TransformComponent>() = getComponent<TransformComponent>();
		Entity& parent = m_environment->getEntity(m_parentUID);
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
			m_environment->getEntity(entt).destroy();
		}

		m_environment->m_registry.destroy(m_entityHandle);
		m_entityHandle = entt::null;
		m_environment = nullptr;
		m_entityUID = 0;
	}

	Entity& Entity::getParent() {
		return m_environment->getEntity(m_parentUID);
	}

	std::vector<uid>& Entity::getChildren() {
		return getComponent<RelationshipComponent>().children;
	}

	glm::mat4 Entity::getWorldMatrix() {
		if (isRoot())
			return glm::mat4(1.0f);

		return getParent().getWorldMatrix() * getRelativeMatrix();
	}

	glm::mat4 Entity::getRelativeMatrix() {
		return getComponent<TransformComponent>().getMatrix();
	}

	void Entity::update() {
		TagComponent& tag = getComponent<TagComponent>();
		RelationshipComponent& relation = getComponent<RelationshipComponent>();

		m_entityUID = tag.entityUID;
		m_parentUID = relation.parent_UID;
		m_isRoot = relation.parent_UID == 0;

		if (m_isRoot) {
			m_environment->m_registry.destroy(m_environment->m_rootEntity->m_entityHandle);
			m_environment->m_rootEntity = &m_environment->m_entities[tag.entityUID];
		}
		m_environment->m_entities[tag.entityUID] = *this;
	}
}

