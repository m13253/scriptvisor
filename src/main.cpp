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

#include <QApplication>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include "backend.h"
#include "scriptprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QTranslator appTranslator;
    appTranslator.load(QLocale(), "scriptvisor", "_", ":/i18n");
    app.installTranslator(&appTranslator);

    bool dashdash = false;
    for(int i = 1; i < app.arguments().length(); ++i) {
        QString option = app.arguments().at(i);
        if(!dashdash) {
            if(option == "--") {
                dashdash = true;
                continue;
            } else if(option == "-exec") {
                ++i;
                if(i < app.arguments().length()) {
                    ScriptProcess::relayExecution(app.arguments().at(i));
                }
                return 127;
            } else if(option == "-autostart") {
                Backend::setMinimized(true);
                continue;
            } else if(option == "-config") {
                ++i;
                if(i < app.arguments().length()) {
                    Backend::setConfigFile(app.arguments().at(i));
                }
            } else if(option == "-logdir") {
                ++i;
                if(i < app.arguments().length()) {
                    Backend::setLogDir(app.arguments().at(i));
                }
            }
        }
    }

    qmlRegisterType<Backend>("Scriptvisor.Backend", 1, 0, "Backend");
    qmlRegisterType<ScriptProcess>("Scriptvisor.ScriptProcess", 1, 0, "ScriptProcess");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
