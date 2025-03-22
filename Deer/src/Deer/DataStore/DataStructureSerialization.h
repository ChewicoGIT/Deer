#pragma once
#include "Deer/DataStore/DataStructure.h"
#include "Deer/Path.h"

#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"

#include <string>

namespace cereal {
    template<class Archive>
    void save(Archive& archive,
        std::filesystem::path const& path)
    {
        archive(path.generic_string());
    }

    template<class Archive>
    void load(Archive& archive,
        std::filesystem::path& path)
    {
        std::string _path;
        archive(_path);
        path = std::filesystem::path(_path);
    }
}

namespace Deer {

    template<class Archive>
    void serialize(Archive& archive,
        DataStructure& ds)
    {
        archive(ds.dataPath, ds.dataStart, ds.dataSize);
    }
}