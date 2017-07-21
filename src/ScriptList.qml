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
import "main.js" as JS

Page {
    id: root
    header: ToolBar {
        Label {
            text: qsTr("Script List")
            anchors.centerIn: parent
        }
        ToolButton {
            text: qsTr("+")
            font.pixelSize: 25
            anchors.right: parent.right
            onClicked: {
                var config = JS.newScript(root)
                root.StackView.view.push("qrc:/ScriptInfo.qml", {
                    config: config,
                    scriptList: root
                })
                JS.addScript(root, config)
                JS.sortScript(root)
            }
        }
    }
    property list<ScriptConfig> listModel
    ListView {
        id: listView
        anchors.fill: parent
        model: listModel
        delegate: ItemDelegate {
            text: modelData.name || qsTr("(Untitled)")
            width: listView.width - listView.leftMargin - listView.rightMargin
            Switch {
                anchors.right: parent.right
                anchors.rightMargin: parent.rightPadding
                anchors.verticalCenter: parent.verticalCenter
                checked: modelData.started && !modelData.failed
                onClicked: checked ? JS.startScript(modelData) : JS.stopScript(modelData)
            }
            onClicked: root.StackView.view.push("qrc:/ScriptInfo.qml", {
                config: modelData,
                scriptList: root
            })
        }
        ScrollIndicator.vertical: ScrollIndicator {}
    }

    Component.onCompleted: {
        JS.loadScript(root)
        JS.sortScript(root)
        JS.autoStartScript(listModel)
    }
}
