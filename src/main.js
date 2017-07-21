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

function init(backend) {
    Qt.backend = backend
}

function shellName() {
    switch(Qt.platform.os) {
    case "windows":
    case "winrt":
        return "PowerShell"
    default:
        return "Bash"
    }
}

function monospaceFont() {
    switch(Qt.platform.os) {
    case "windows":
    case "winrt":
        return "Consolas"
    case "osx":
        return "Menlo"
    default:
        return "Monospace"
    }
}

function dumpScripts(listModel) {
    var result = []
    for(var i = 0; i < listModel.length; i++) {
        if(shellName() === "Bash") {
            listModel[i].startupScriptBash = listModel[i].startupScript
            listModel[i].shutdownScriptBash = listModel[i].shutdownScript
        }
        if(shellName() === "PowerShell") {
            listModel[i].startupScriptPS = listModel[i].startupScript
            listModel[i].shutdownScriptPS = listModel[i].shutdownScript
        }
        var item = {
            name: listModel[i].name,
            autoStart: listModel[i].autoStart,
            restartDelay: listModel[i].restartDelay,
            startupScriptBash: listModel[i].startupScriptBash.split("\n"),
            startupScriptPS: listModel[i].startupScriptPS.split("\n"),
            shutdownScriptBash: listModel[i].shutdownScriptBash.split("\n"),
            shutdownScriptPS: listModel[i].shutdownScriptPS.split("\n")
        }
        result.push(item)
    }
    result = JSON.stringify(result, null, "    ")
    return result
}

function parseScripts(scriptList, str) {
    var obj = JSON.parse(str)
    var result = []
    for(var i = 0; i < obj.length; i++) {
        var item = Qt.createQmlObject("ScriptConfig {}", scriptList, null)
        item.name = obj[i].name
        item.autoStart = obj[i].autoStart
        item.restartDelay = obj[i].restartDelay
        item.startupScriptBash = obj[i].startupScriptBash.join("\n")
        item.startupScriptPS = obj[i].startupScriptPS.join("\n")
        item.shutdownScriptBash = obj[i].shutdownScriptBash.join("\n")
        item.shutdownScriptPS = obj[i].shutdownScriptPS.join("\n")
        if(shellName() === "Bash") {
            item.startupScript = item.startupScriptBash
            item.shutdownScript = item.shutdownScriptBash
        }
        if(shellName() === "PowerShell") {
            item.startupScript = item.startupScriptPS
            item.shutdownScript = item.shutdownScriptPS
        }
        result.push(item)
    }
    return result
}

function loadScript(scriptList) {
    try {
        var config = Qt.backend.loadConfig()
        var scripts = parseScripts(scriptList, config)
        scriptList.listModel = scripts
    } catch(e) {
        console.error(e)
    }
}

function saveScript(scriptList) {
    var config = dumpScripts(scriptList.listModel)
    Qt.backend.saveConfig(config)
}

function newScript(scriptList) {
    return Qt.createQmlObject("ScriptConfig {}", scriptList, null)
}

function addScript(scriptList, config) {
    scriptList.listModel.push(config)
    saveScript(scriptList)
}

function removeScript(scriptList, config) {
    var newListModel = []
    for(var i = 0; i < scriptList.listModel.length; i++) {
        if(scriptList.listModel[i].uniq !== config.uniq) {
            newListModel.push(scriptList.listModel[i])
        }
    }
    scriptList.listModel = newListModel
    saveScript(scriptList)
}

function sortScript(scriptList) {
    var newListModel = []
    for(var i = 0; i < scriptList.listModel.length; i++) {
        newListModel.push(scriptList.listModel[i])
    }
    for(var i = 0; i < newListModel.length - 1; i++) {
        for(var j = i + 1; j < newListModel.length; j++) {
            if(newListModel[i].name > newListModel[j].name) {
                var tmp = newListModel[i]
                newListModel[i] = newListModel[j]
                newListModel[j] = tmp
            }
        }
    }
    scriptList.listModel = newListModel
}

function autoStartScript(listModel) {
    for(var i = 0; i < listModel.length; i++) {
        if(listModel[i].autoStart) {
            startScript(listModel[i])
        }
    }
}

function startScript(config) {
    if(!config.started || config.failed) {
        config.started = true
        config.failed = false
        config.timer.running = false
        config.process = Qt.backend.createScriptProcess(config)
        config.process.failed.connect(function() {
            config.failed = true
            config.timer.running = config.restartDelay > 0
        })
        config.process.start(config.startupScript, config.name)
    }
}

function stopScript(config, callback) {
    if(config.started) {
        config.started = false
        config.failed = false
        config.timer.running = false
        var startProcess = config.process
        var stopProcessCalback = function() {
            startProcess.stop()
            if(callback) {
                callback()
            }
        }
        if(config.shutdownScript !== "") {
            var stopProcess = Qt.backend.createScriptProcess(config)
            stopProcess.finished.connect(stopProcessCalback)
            stopProcess.failed.connect(stopProcessCalback)
            stopProcess.start(config.shutdownScript, config.name)
        } else {
            stopProcessCalback()
        }
    } else if(callback) {
        callback()
    }
}

function restartScript(config) {
    if(config.started && config.failed) {
        config.started = true
        config.failed = false
        config.timer.running = false
        config.process = Qt.backend.createScriptProcess(config)
        config.process.failed.connect(function() {
            config.failed = true
            config.timer.running = config.restartDelay > 0
        })
        config.process.start(config.startupScript, config.name)
    }
}

function stopAllScripts(listMode, callback, i) {
    if(i === undefined) {
        i = 0
    }
    if(i < listMode.length) {
        stopScript(listMode[i], function() {
            stopAllScripts(listMode, callback, i + 1)
        })
    } else if(callback) {
        callback()
    }
}
