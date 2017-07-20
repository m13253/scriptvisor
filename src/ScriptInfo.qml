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
import "main.js" as JS

Page {
    id: root
    property ScriptConfig config
    property ScriptList scriptList
    header: ToolBar {
        ToolButton {
            text: qsTr("‹")
            anchors.left: parent.left
            onClicked: {
                JS.saveScript(scriptList)
                JS.sortScript(scriptList)
                root.StackView.view.pop()
            }
        }
        Label {
            text: config.name || qsTr("Edit Script")
            anchors.centerIn: parent
        }
        ToolButton {
            text: qsTr("⋮")
            anchors.right: parent.right
            onClicked: optionsMenu.visible ? optionsMenu.close() : optionsMenu.open()
            Menu {
                id: optionsMenu
                transformOrigin: Menu.TopRight
                x: parent.width - width
                y: parent.height
                MenuItem {
                    text: qsTr("Remove")
                    onClicked: {
                        JS.removeScript(scriptList, config)
                        root.StackView.view.pop()
                        JS.stopScript(config)
                    }
                }
                enter: Transition {
                    NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
                    NumberAnimation { property: "scale"; from: 0.9; to: 1.0; duration: 100 }
                }
                exit: Transition {
                    NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 250 }
                }
            }
        }
    }
    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true
        RowLayout {
            width: scrollView.width
            GridLayout {
                columns: 4
                rowSpacing: 25
                columnSpacing: 25
                Layout.margins: 25
                Layout.fillWidth: true
                Label {
                    text: qsTr("Name")
                }
                TextField {
                    text: config.name
                    placeholderText: qsTr("(Untitled)")
                    Layout.columnSpan: 3
                    Layout.fillWidth: true
                    onTextChanged: config.name = text
                }
                Label {
                    text: qsTr("Auto start")
                }
                Switch {
                    checked: config.autoStart
                    onCheckedChanged: config.autoStart = checked
                }
                Label {
                    text: qsTr("Restart delay")
                }
                SpinBox {
                    id: restartDelay
                    from: 0
                    to: 300000
                    stepSize: 1000
                    value: config.restartDelay * 1000
                    editable: true
                    Layout.fillWidth: true
                    validator: DoubleValidator {
                        bottom: restartDelay.from
                        top: restartDelay.to
                    }
                    textFromValue: function(value, locale) {
                        return Number(value / 1000).toLocaleString(locale, 'f', 1)
                    }
                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text) * 1000
                    }
                    onValueChanged: config.restartDelay = value / 1000
                }
                Label {
                    text: qsTr("Startup script")
                    anchors.top: startupScript.top
                }
                TextArea {
                    id: startupScript
                    text: config.startupScript
                    font.family: "Monospace"
                    wrapMode: TextEdit.WrapAnywhere
                    Layout.columnSpan: 3
                    Layout.fillWidth: true
                    Layout.minimumHeight: 50
                    onTextChanged: config.startupScript = text
                }
                Label {
                    text: qsTr("Shutdown script")
                    anchors.top: shutdownScript.top
                }
                TextArea {
                    id: shutdownScript
                    text: config.shutdownScript
                    font.family: "Monospace"
                    wrapMode: TextEdit.WrapAnywhere
                    Layout.columnSpan: 3
                    Layout.fillWidth: true
                    Layout.minimumHeight: 50
                    onTextChanged: config.shutdownScript = text
                }
                Label {
                    text: qsTr("Scripts will be executed using %1.").arg(JS.shellName())
                    Layout.columnSpan: 4
                }
            }
        }
    }
}
