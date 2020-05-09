#ifndef _BATMON_H
#define _BATMON_H

#include <QObject>
#include <QSystemTrayIcon>

/* Default values */
#define D_BATTERYICONPATH  "/usr/share/icons/Adwaita/16x16/status/"
#define D_BATTERYPATH      "/sys/class/power_supply/BAT0/"
#define D_CHARGE_FULL_FILE D_BATTERYPATH "energy_full"
#define D_CHARGE_NOW_FILE  D_BATTERYPATH "energy_now"
#define D_STATUS_FILE      D_BATTERYPATH "status"

#define D_UPDATE_INTERVAL 5000
#define D_ICON true
#define D_ICON_POSITION POS_RIGHT
//#define D_VERBOSE false
#define D_VERBOSE true

enum icontype {
	ICON_NONE,
	ICON_DISCHARGE,
	ICON_CHARGE
};

enum iconposition {
	POS_LEFT,
	POS_RIGHT
};

class BatteryMonitor : public QObject
{
	Q_OBJECT

	public:
		BatteryMonitor();
	public slots:
		void update();

	private:
		void loadConfiguration();
		bool loadConfigurationFile(const QString &path);
		void setIcon(enum icontype icon, const unsigned int percent);
		void setTextIcon(enum icontype icon, const unsigned int percent);
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
};

#endif /* _BATMON_H */
