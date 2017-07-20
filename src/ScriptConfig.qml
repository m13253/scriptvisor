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
import Scriptvisor.ScriptProcess 1.0
import "main.js" as JS

Item {
    id: config
    property real uniq
    property string name
    property bool started: false
    property bool failed: false
    property bool autoStart: false
    property real restartDelay: 1
    property string startupScript
    property string startupScriptBash
    property string startupScriptPS
    property string shutdownScript
    property string shutdownScriptBash
    property string shutdownScriptPS
    property ScriptProcess process: null
    property Timer timer: Timer {
        interval: restartDelay * 1000
        onTriggered: JS.restartScript(config)
    }

    Component.onCompleted: {
        uniq = Math.random()
    }
}
