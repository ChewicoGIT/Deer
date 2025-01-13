#include "CompressedDataAccess.h"

namespace Deer {
    CompressedDataAccess::~CompressedDataAccess() {
        delete[] m_data;
    }

    uint8_t* Deer::CompressedDataAccess::loadData(const Path& path, uint32_t* size) {
        DataStructure structure = m_dataStructure[path];
        *size = structure.dataSize;

        return &m_data[structure.dataStart];
    }
}
