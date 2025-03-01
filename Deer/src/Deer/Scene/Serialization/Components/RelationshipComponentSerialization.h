#pragma once
#include "Deer/Scene/Components.h"

namespace Deer {
    // RELATIONSHIP COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        RelationshipComponent& relationship) {

        archive(cereal::make_nvp("parentUID", relationship.parent_UID));
        archive(cereal::make_nvp("childrensUIDs", relationship.children));

    }
}