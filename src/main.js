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

function init(backend) {
    Qt.backend = backend
}

function shellName() {
    return Qt.backend.shell
}

function dumpScripts(listModel) {
    var result = []
    for(var i = 0; i < listModel.length; i++) {
        if(Qt.backend.shell === "Bash") {
            listModel[i].startupScriptBash = listModel[i].startupScript
            listModel[i].shutdownScriptBash = listModel[i].shutdownScript
        }
        if(Qt.backend.shell === "PowerShell") {
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
        if(Qt.backend.shell === "Bash") {
            item.startupScript = item.startupScriptBash
            item.shutdownScript = item.shutdownScriptBash
        }
        if(Qt.backend.shell === "PowerShell") {
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
