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
 * Main EasyBatMon Header file.
 */
#ifndef _BATMON_H
#define _BATMON_H

#include <QObject>
#include <QSystemTrayIcon>

#include "exe_checker.h"

/* Position of the icon in the tray */
enum iconposition {
	POS_LEFT,
	POS_RIGHT
};

#include "batmon_configuration.h"

/* Type of icons, must be congruent to the icons in the directory */
enum icontype {
	ICON_NONE,
	ICON_DISCHARGE,
	ICON_CHARGE,
	ICON_FULL
};

/* Default place where the battery icons are placed. */
#define D_BATTERYICONPATH  "/usr/share/icons/Adwaita/16x16/status/"
/* Default battery path */
#define D_BATTERYPATH      "/sys/class/power_supply/BAT0/"
/* Default full battery reference file */
#define D_CHARGE_FULL_FILE D_BATTERYPATH "energy_full"
/* Default battery current value file */
#define D_CHARGE_NOW_FILE  D_BATTERYPATH "energy_now"
/* Default battery status file */
#define D_STATUS_FILE      D_BATTERYPATH "status"

/* Default update interval in mS */
#define D_UPDATE_INTERVAL 5000
/* If true, the tray will have the icon */
#define D_ICON true
/* Position of the icon */
#define D_ICON_POSITION POS_RIGHT
/* If true, the daemon will print more information when run */
#define D_VERBOSE false
/* If true, the daemon will check if another process with the same name
 * is running and then refuse to start */
#define D_CHECK_NOT_RUNNING false

class BatteryMonitor : public QObject
{
	Q_OBJECT

	public:
		BatteryMonitor();
	public slots:
		void update();

	private:
		int do_check_running();
		/* Load default configuration */
		void loadConfiguration();
		/* Load the configuration file in memory */
		bool loadConfigurationFile(const QString &path);
		/* Set the icon to the value specified */
		void setIcon(enum icontype icon, const unsigned int percent);
		/* Set the text to the value specified */
		void setTextIcon(enum icontype icon, const unsigned int percent);
		/* Select the correct icon from the percent */
		void setIcoIcon(enum icontype icon, const unsigned int percent);

		unsigned int update_interval;
		QString battery_charge_full_file;
		QString battery_charge_now_file;
		QString battery_status_file;

		QSystemTrayIcon trayiconnum;
		QSystemTrayIcon trayiconico;

		unsigned int currentPercent = 0;
		unsigned int currentIcon = ICON_NONE;

		QString batteryiconpath;
		unsigned int updateinterval;
		bool icon;
		enum iconposition iconpos;
		bool verbose;
		bool check_not_running;

		ExeChecker exe_checker;
};

#define KEYS                                                             \
	X("icon",                     int,     icon)                     \
	X("verbose",                  int,     verbose)                  \
	X("iconpos",                  int,     iconpos)                  \
	X("update_interval",          int,     update_interval)          \
	X("battery_iconpath",         QString, batteryiconpath)          \
	X("battery_charge_full_file", QString, battery_charge_full_file) \
	X("battery_charge_now_file",  QString, battery_charge_now_file)  \
	X("battery_status_file",      QString, battery_status_file)      \
	X("check_not_running",        int,     check_not_running)

#endif /* _BATMON_H */
