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
