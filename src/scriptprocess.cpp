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

#include "scriptprocess.h"
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include "backend.h"
#ifndef WIN32
#include "unistd.h"
#endif

ScriptProcess::ScriptProcess(QObject *parent) : QObject(parent) {
}

ScriptProcess::~ScriptProcess() {
    delete scriptFile;
    scriptFile = nullptr;
    if(!scriptFilename.isEmpty()) {
        QFile(scriptFilename).remove();
        scriptFilename.clear();
    }
    delete logFile;
    logFile = nullptr;
}

void ScriptProcess::start(const QString &script, const QString &log) {
    scriptFile = new QTemporaryFile;
    scriptFile->setAutoRemove(false);
#ifdef WIN32
    scriptFile->setFileTemplate(QDir::tempPath() + "\\Scriptvisor.XXXXXX.ps1");
#else
    scriptFile->setFileTemplate(QDir::tempPath() + "/Scriptvisor.XXXXXX");
#endif
    scriptFile->open();
#ifdef WIN32
    scriptFile->write("\xef\xbb\xbf", 3);
#endif
    scriptFile->write(script.toUtf8());
#ifdef WIN32
    scriptFile->write("\nexit $LASTEXITCODE\n", 20);
#endif
    scriptFile->flush();
    scriptFilename = scriptFile->fileName();
    delete scriptFile;
    scriptFile = nullptr;
    QDir().mkdir(Backend::logDir());
    QByteArray logFilenameBuffer(Backend::logDir().toUtf8());
    logFilenameBuffer += "/log-";
    for(char c : log.toUtf8()) {
        if(
            (c >= 0 && c <= 31) ||
            (c == '<') || (c == '>') || (c == '|') ||
            (c == '"') || (c == '%') ||
            (c == '/') || (c == '\\') || (c == ':') ||
            (c == '?') || (c == '*')
        ) {
            logFilenameBuffer += '%';
            if(c >= 0 && c <= 9) {
                logFilenameBuffer += '0';
            }
            logFilenameBuffer += QString::number(c, 16).toUtf8();
        } else {
            logFilenameBuffer += c;
        }
    }
    logFilenameBuffer += ".log";
    QString logFilename = QString::fromUtf8(logFilenameBuffer);
    logFile = new QFile(logFilename);
    logFile->open(QFile::Append | QFile::Text);
    logFile->write(tr(":: Script started at %1\n").arg(QDateTime::currentDateTime().toString()).toLocal8Bit());
    logFile->close();
    process = new QProcess(parent());
    QStringList arguments;
#ifdef WIN32
    process->setProgram("powershell");
    arguments << "-ExecutionPolicy" << "Bypass";
#else
    process->setProgram(qApp->applicationFilePath());
    arguments << "-exec";
#endif
    arguments << scriptFilename;
    process->setArguments(arguments);
    process->setStandardInputFile(QProcess::nullDevice());
    process->setStandardOutputFile(logFilename, QProcess::Append);
    process->setStandardErrorFile(logFilename, QProcess::Append);
    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &ScriptProcess::onProcessFinished);
    started = true;
    process->start();
}

void ScriptProcess::stop() {
    started = false;
    if(process) {
        qint64 pid = process->processId();
        if(pid > 1) {
            QProcess *taskkill = new QProcess(parent());
#ifdef WIN32
            taskkill->setProgram("taskkill");
            taskkill->setArguments(QStringList({"/t", "/pid", QString::number(pid)}));
#else
            taskkill->setProgram("kill");
            taskkill->setArguments(QStringList({"--", QString::number(pid), QString::number(-pid)}));
#endif
            taskkill->setStandardInputFile(QProcess::nullDevice());
            taskkill->setStandardOutputFile(QProcess::nullDevice());
            taskkill->setStandardErrorFile(QProcess::nullDevice());
            connect(taskkill, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int, QProcess::ExitStatus) {
                delete taskkill;
            });
            taskkill->start();
        }
    }
}

void ScriptProcess::relayExecution(const QString &script) {
#ifdef WIN32
    (void) script;
#else
    pid_t pid = getpid();
    setpgid(pid, pid);
    execlp("bash", "bash", script.toUtf8().data(), nullptr);
#endif
}

void ScriptProcess::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if(!scriptFilename.isEmpty()) {
        QFile(scriptFilename).remove();
        scriptFilename.clear();
    }
    if(logFile) {
        logFile->open(QFile::Append | QFile::Text);
    }
    if(exitStatus == QProcess::NormalExit && exitCode == 0) {
        if(logFile) {
            logFile->write(tr(":: Script finished at %1\n").arg(QDateTime::currentDateTime().toString()).toLocal8Bit());
        }
        emit finished();
    } else {
        if(logFile) {
            if(started) {
                logFile->write(tr(":: Script failed at %1, return code = %2\n").arg(QDateTime::currentDateTime().toString(), QString::number(exitCode)).toLocal8Bit());
            } else {
                logFile->write(tr(":: Script terminated at %1\n").arg(QDateTime::currentDateTime().toString()).toLocal8Bit());
            }
        }
        emit failed();
    }
    if(logFile) {
        logFile->close();
    }
}
