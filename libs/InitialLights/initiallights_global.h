#pragma once

#include <QtCore/qglobal.h>

#if defined(INITIALLIGHTS_LIBRARY)
#  define INITIALLIGHTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define INITIALLIGHTSSHARED_EXPORT Q_DECL_IMPORT
#endif
