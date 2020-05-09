/**
 * This program is released under ...
 *
 * Berardi Davide <berardi.dav@gmail.com>
 */
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QApplication>

#include "batmon.h"

BatteryMonitor::BatteryMonitor() {
	QTimer *updateTimer = new QTimer(this);
	if (!updateTimer) {
		exit(1);
	}

	loadConfiguration();

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

	updateTimer->start(update_interval);
	/* Signal an update at start time */
	update();
}

void BatteryMonitor::loadConfiguration()
{
	/* Load defaults */
	icon = D_ICON;
	verbose = D_VERBOSE;
	iconpos = D_ICON_POSITION;
	battery_charge_full_file = D_CHARGE_FULL_FILE;
	battery_charge_now_file = D_CHARGE_NOW_FILE;
	battery_status_file = D_STATUS_FILE;
	update_interval = D_UPDATE_INTERVAL;
	batteryiconpath = D_BATTERYICONPATH;
	
	const QList<QString> loadpaths = QList<QString>() <<
		QString::fromStdString(qgetenv("HOME").toStdString()) + ".config/batmon/batmon.conf" <<
		QString::fromStdString(qgetenv("HOME").toStdString()) + ".batmonrc" <<
		QString("/etc/batmon.conf")
		;

	for (auto path: loadpaths) {
		if (loadConfigurationFile(path))
			break;
	}
}

bool BatteryMonitor::loadConfigurationFile(const QString &filepath)
{
	QFile f(filepath);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!f.atEnd()) {
		QString s = QString::fromStdString(f.readLine().toStdString()).trimmed();

		if (s.contains(QRegExp("^\\s+#")))
			continue;
		
		qWarning() << s;
	}

	f.close();
	return true;
}

static QString readFileString(const QString &filepath)
{
	QFile f(filepath);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return QString();
	QByteArray content = f.readLine();
	f.close();

	return QString::fromStdString(content.toStdString()).trimmed();
}

static unsigned int readFileInt(const QString &filepath)
{
	return readFileString(filepath).toInt();
}

void BatteryMonitor::setTextIcon(enum icontype icon, const unsigned int percent)
{
	if (icon == currentIcon && percent == currentPercent)
		return;

	if (icon == ICON_NONE)
		trayiconnum.setVisible(false);

	QPixmap pixmap(16, 16);
	pixmap.fill(QColor(0,0,0,0));

	QPainter painter(&pixmap);

	if (icon == ICON_CHARGE)
		painter.setPen(Qt::white);
	if (icon == ICON_DISCHARGE) {
		if (percent < 30)
			painter.setPen(Qt::red);
		else
			painter.setPen(Qt::white);
	}

	painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::number(percent));

	trayiconnum.setIcon(pixmap);
	trayiconnum.setToolTip("Battery status");
	trayiconnum.setVisible(true);
}

void BatteryMonitor::setIcoIcon(enum icontype icon, const unsigned int percent)
{
	int rounded_percent = 0;
	if (icon == currentIcon && percent == currentPercent)
		return;

	QString batteryicon=QString(batteryiconpath)+QString("battery-");
	if (icon == ICON_NONE)
		batteryicon += QString("missing");
	else {
		batteryicon += QString("level-");
		/* Round to get the correct icon */
		rounded_percent = percent - (percent % 10);
		batteryicon += QString::number(rounded_percent);
	}

	if (icon == ICON_CHARGE) {
		if (rounded_percent == 100)
			batteryicon += QString("-charged");
		else
			batteryicon += QString("-charging");
	}

	batteryicon += QString("-symbolic.symbolic.png");

	if (verbose)
		qWarning() << batteryicon;

	QImage iconimage(batteryicon, "png");
	iconimage.invertPixels(QImage::InvertRgb);

	QPixmap pixmap(16, 16);
	pixmap.fill(QColor(0,0,0,0));
	pixmap.convertFromImage(iconimage);

	trayiconico.setIcon(pixmap);
	trayiconico.setToolTip("Battery status");
	trayiconico.setVisible(true);
}

void BatteryMonitor::setIcon(enum icontype icon, unsigned int percent)
{
	qWarning() << "setting icon";
	if (icon && iconpos == POS_LEFT)
		setTextIcon(icon, percent);
	setIcoIcon(icon, percent);
	if (icon && iconpos == POS_RIGHT)
		setTextIcon(icon, percent);

	currentIcon = icon;
	currentPercent = percent;
}

void BatteryMonitor::update(void) {
	unsigned int charge_pct = 0;
	QString status = readFileString(battery_status_file);
	unsigned int charge_now = readFileInt(battery_charge_now_file);
	unsigned int charge_full = readFileInt(battery_charge_full_file);

	if (charge_full != 0)
		charge_pct = (((double)charge_now / (double)charge_full) * 100 + 0.5);

	if (!status.compare("Charging"))
		setIcon(ICON_CHARGE, charge_pct);
	else if (!status.compare("Discharging") || !status.compare(""))
		setIcon(ICON_DISCHARGE, charge_pct);
	else {
		qWarning() << "Warn: Unknown status: " << status;
		setIcon(ICON_NONE, 0);
	}
}

int main(int argc, char **argv)
{
	QApplication qBatMon(argc, argv);
	new BatteryMonitor();

	return qBatMon.exec();
}
