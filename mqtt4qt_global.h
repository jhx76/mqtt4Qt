#ifndef MOSQUITTO4QT_GLOBAL_H
#define MOSQUITTO4QT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MOSQUITTO4QT_LIBRARY)
#  define MOSQUITTO4QTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MOSQUITTO4QTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MOSQUITTO4QT_GLOBAL_H
