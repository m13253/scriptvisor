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
                text: "&Restore"
                onTriggered: {
                    appWindow.show()
                    appWindow.raise()
                    appWindow.requestActivate()
                }
            }
            MenuItem {
                text: "&Quit"
                onTriggered: {
                    JS.stopAllScripts(scriptList.listModel, function() {
                        Qt.quit()
                    })
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

    Component.onCompleted: {
        JS.init(backend)
    }
}