#ifndef _BATMON_CONFIGURATION_H
#define _BATMON_CONFIGURATION_H

#include <QString>

#include "batmon.h"

template <class T> class Configuration {
private:
	T value;
public:
	void loadValue(QString &s);
	T getValue();
};

template <class T> inline T Configuration<T>::getValue(void)
{
	return value;
}

template <> inline void Configuration<int>::loadValue(QString &s)
{
	value = s.toInt();
}

template <class T> inline void Configuration<T>::loadValue(QString &s) {
	value = s;
}

#endif /* _BATMON_CONFIGURATION_H */
