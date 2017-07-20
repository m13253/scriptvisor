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

#include "backend.h"
#include <QFile>

Backend::Backend(QObject *parent) : QObject(parent) {
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
QString Backend::m_configFile = "Scriptvisor.json";
QString Backend::m_logDir = "Scriptvisor-logs";
