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
 * Check if an exe is already running
 */
#ifndef _EXE_CHECKER_H
#define _EXE_CHECKER_H

#include <QDir>
#include <QObject>

class ExeChecker : public QObject
{
	Q_OBJECT

	public:
		ExeChecker();
		int search();
	private:
		QDir proc;
		QString exe_path;
};

#endif /* _EXE_CHECKER_H */
