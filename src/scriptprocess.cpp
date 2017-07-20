/*
  BSD 3-Clause License

  Copyright (c) 2017, Star Brilliant
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include "scriptprocess.h"
#include <QCoreApplication>
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
    delete logFile;
    logFile = nullptr;
}

void ScriptProcess::start(const QString &script, const QString &log) {
    scriptFile = new QTemporaryFile;
#ifdef WIN32
    scriptFile->setFileTemplate(QDir::tempPath() + "\\Scriptvisor.XXXXXX.ps1");
#else
    scriptFile->setFileTemplate(QDir::tempPath() + "/Scriptvisor.XXXXXX");
#endif
    scriptFile->setAutoRemove(true);
    scriptFile->open();
#ifdef WIN32
    scriptFile->write("\xef\xbb\xbf", 3);
#endif
    scriptFile->write(script.toUtf8());
    scriptFile->close();
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
    logFile->write(tr(":: Script started at %1\n").arg(QDateTime::currentDateTime().toString()).toUtf8());
    logFile->close();
    process = new QProcess(parent());
    QStringList arguments;
#ifdef WIN32
    process->setProgram("powershell");
#else
    process->setProgram(qApp->applicationFilePath());
    arguments << "-exec";
#endif
    arguments << scriptFile->fileName();
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
            taskkill->setArguments(QStringList({"/t", "/pid", QString::number(process->processId())}));
#else
            taskkill->setProgram("kill");
            taskkill->setArguments(QStringList({"--", QString::number(process->processId()), QString::number(-process->processId())}));
#endif
            taskkill->setStandardInputFile(QProcess::nullDevice());
            taskkill->setStandardOutputFile(QProcess::nullDevice());
            taskkill->setStandardErrorFile(QProcess::nullDevice());
            connect(taskkill, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=](int, QProcess::ExitStatus) {
                delete taskkill;
            });
            taskkill->start();
        }
    }
}

void ScriptProcess::relayExecution(const QString &script) {
#ifndef WIN32
    pid_t pid = getpid();
    setpgid(pid, pid);
    execlp("bash", "bash", script.toUtf8().data(), nullptr);
#endif
}

void ScriptProcess::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if(scriptFile) {
        scriptFile->remove();
    }
    if(logFile) {
        logFile->open(QFile::Append | QFile::Text);
    }
    if(exitStatus == QProcess::NormalExit && exitCode == 0) {
        if(logFile) {
            logFile->write(tr(":: Script finished at %1\n").arg(QDateTime::currentDateTime().toString()).toUtf8());
        }
        emit finished();
    } else {
        if(logFile) {
            if(started) {
                logFile->write(tr(":: Script failed at %1, return code = %2\n").arg(QDateTime::currentDateTime().toString(), QString::number(exitCode)).toUtf8());
            } else {
                logFile->write(tr(":: Script terminated at %1\n").arg(QDateTime::currentDateTime().toString()).toUtf8());
            }
        }
        emit failed();
    }
    if(logFile) {
        logFile->close();
    }
}
