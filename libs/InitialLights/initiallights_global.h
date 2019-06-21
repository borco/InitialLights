#ifndef INITIALLIGHTS_GLOBAL_H
#define INITIALLIGHTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(INITIALLIGHTS_LIBRARY)
#  define INITIALLIGHTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define INITIALLIGHTSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define SCENE_COUNT 6

#endif // INITIALLIGHTS_GLOBAL_H
