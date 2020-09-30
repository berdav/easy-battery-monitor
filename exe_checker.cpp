/**
 * This file is part of EasyBatMon.
 *
 * Easy BatMon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Davide Berardi <berardi.dav@gmail.com> 2020
 *
 * Check if an executable is already running
 */

#include <unistd.h>
#include <dirent.h>
#include <QDebug>
#include "exe_checker.h"

static QString get_link(QString path)
{
	char *buf = (char *) malloc(PATH_MAX);
	int r = readlink(qPrintable(path + QString("/exe")), buf, PATH_MAX - 1);
	if (r < 0) {
		return QString("");
	}
	buf[r] = '\0';
	/* Also consider deleted executables */
	return QString(buf).split(" (deleted)")[0];
}

int ExeChecker::search()
{
	QStringList qsl = proc.entryList();
	for (QString entry : qsl) {
		entry = entry.replace(QChar('"'), QChar());

		if (!entry.contains(QRegExp("\\d+")))
			continue;

		/* Skip self executable */
		pid_t me = getpid();
		if (me == entry.toInt())
			continue;

		QString link_path = QString("/proc/") + entry;

		QString target_exe = get_link(link_path);
		if (!target_exe.compare(exe_path)) {
			return 1;
		}
	}
	return 0;
}

ExeChecker::ExeChecker()
{
	proc = QDir("/proc");
	exe_path = get_link(QString("/proc/self"));
}
