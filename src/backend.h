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

#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QProcess>
#include "scriptprocess.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString shell READ shell CONSTANT)
    Q_PROPERTY(bool minimized READ minimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(QString configFile READ configFile WRITE setConfigFile)
    Q_PROPERTY(QString logDir READ logDir WRITE setLogDir)

public:
    explicit Backend(QObject *parent = nullptr);
    QString shell() const;
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
    void shellChanged(const QString &);
    void minimizedChanged(bool);
};

#endif // BACKEND_H
