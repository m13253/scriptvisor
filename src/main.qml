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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import Scriptvisor.Backend 1.0
import "main.js" as JS

ApplicationWindow {
    id: appWindow
    visible: !backend.minimized
    width: 480
    height: 480
    title: qsTr("Scriptvisor")

    property ScriptList scriptList: ScriptList {}

    StackView {
        anchors.fill: parent
        initialItem: scriptList
    }
    SystemTrayIcon {
        iconSource: "qrc:/icon.png"
        visible: true
        menu: Menu {
            MenuItem {
                text: qsTr("&Restore")
                onTriggered: {
                    appWindow.show()
                    appWindow.raise()
                    appWindow.requestActivate()
                }
            }
            MenuItem {
                text: quitting ? qsTr("Force &Quit") : qsTr("&Quit")
                property bool quitting: false
                onTriggered: {
                    if(quitting) {
                        Qt.quit()
                    } else {
                        quitting = true
                        JS.stopAllScripts(scriptList.listModel, function() {
                            Qt.quit()
                        })
                    }
                }
            }
        }
        onActivated: {
            appWindow.show()
            appWindow.raise()
            appWindow.requestActivate()
        }
    }

    Backend {
        id: backend
    }

    onClosing: function(event) {
        event.accept = false;
        appWindow.hide();
    }
    Component.onCompleted: {
        JS.init(backend)
    }
}
