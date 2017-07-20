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

#ifndef SCRIPTPROCESS_H
#define SCRIPTPROCESS_H

#include <QObject>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>

class ScriptProcess : public QObject
{
    Q_OBJECT
public:
    explicit ScriptProcess(QObject *parent = nullptr);
    ~ScriptProcess();
    Q_INVOKABLE void start(const QString &script, const QString &log);
    Q_INVOKABLE void stop();
    static void relayExecution(const QString &script);

private:
    QProcess* process = nullptr;
    QTemporaryFile *scriptFile = nullptr;
    QFile *logFile = nullptr;
    bool started = false;

signals:
    void finished();
    void failed();

public slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // SCRIPTPROCESS_H
