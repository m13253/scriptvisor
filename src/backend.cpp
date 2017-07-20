/*
  Copyright (C) 2017  StarBrilliant <m13253@hotmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not,
  see <http://www.gnu.org/licenses/>.
*/

#include "backend.h"
#include <QApplication>
#include <QFile>
#include <QIcon>

Backend::Backend(QObject *parent) : QObject(parent) {
    qApp->setApplicationDisplayName(tr("Scriptvisor"));
    qApp->setDesktopFileName("scriptvisor-autostart.desktop");
    qApp->setWindowIcon(QIcon("qrc:/icon.png"));
}

QString Backend::shell() const {
#ifdef WIN32
    return "PowerShell";
#else
    return "Bash";
#endif
}

bool Backend::minimized() {
    return m_minimized;
}

void Backend::setMinimized(bool newValue) {
    m_minimized = newValue;
}

QString Backend::configFile() {
    return m_configFile;
}

void Backend::setConfigFile(const QString &newValue) {
    m_configFile = newValue;
}

QString Backend::logDir() {
    return m_logDir;
}

void Backend::setLogDir(const QString &newValue) {
    m_logDir = newValue;
}

QString Backend::loadConfig() const {
    QFile file(configFile());
    if(!file.exists()) {
        return "[]";
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();
    return QString::fromUtf8(data);
}

void Backend::saveConfig(const QString &config) const {
    QFile file(configFile());
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(config.toUtf8());
    file.close();
}

ScriptProcess *Backend::createScriptProcess(QObject *parent) const {
    return new ScriptProcess(parent);
}

bool Backend::m_minimized = false;
QString Backend::m_configFile = "scriptvisor.json";
QString Backend::m_logDir = "scriptvisor-logs";
