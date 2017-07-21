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

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QProcess>
#include "scriptprocess.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool minimized READ minimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(QString configFile READ configFile WRITE setConfigFile)
    Q_PROPERTY(QString logDir READ logDir WRITE setLogDir)

public:
    explicit Backend(QObject *parent = nullptr);
    static bool minimized();
    static void setMinimized(bool newValue);
    static QString configFile();
    static void setConfigFile(const QString &newValue);
    static QString logDir();
    static void setLogDir(const QString &newValue);
    Q_INVOKABLE QString loadConfig() const;
    Q_INVOKABLE void saveConfig(const QString &config) const;
    Q_INVOKABLE ScriptProcess *createScriptProcess(QObject *parent = nullptr) const;

private:
    static bool m_minimized;
    static QString m_configFile;
    static QString m_logDir;

signals:
    void minimizedChanged(bool);
};

#endif // BACKEND_H
