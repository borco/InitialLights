#include "ibluetoothexplorer.h"

namespace il {
namespace bluetooth {

IBluetoothExplorer::IBluetoothExplorer(QObject *parent)
    : QObject(parent)
    , m_isDiscovering { false }
    , m_discoverTimeout { 10000 } // in msec
{
}

IBluetoothExplorer::~IBluetoothExplorer()
{
}

} // namespace bluetooth
} // namespace il
