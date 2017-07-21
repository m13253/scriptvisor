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
import "main.js" as JS

Page {
    id: root
    property ScriptConfig config
    property ScriptList scriptList
    header: ToolBar {
        ToolButton {
            text: qsTr("‹")
            font.pixelSize: 25
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
            font.pixelSize: 25
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
