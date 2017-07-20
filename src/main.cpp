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
