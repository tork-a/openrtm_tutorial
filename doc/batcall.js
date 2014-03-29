/**
* Call .bat file under %DRIVE_HOME%\demo
* @param {String} bat_name   Name of the .bat file. If the file is in subdirectory 
                  under `demo` folder, double-backslash '\\' needs to be used to separate
                  directories. 

                  Eg. If the .bat is at %DRIVE_HOME%/demo/demofolder-a/sample.bat, 
                      you pass demofolder-a\\sample.bat
* @return None
*/
function callbat_relativepath(bat_name) {
  var objFileSys = new ActiveXObject("Scripting.FileSystemObject");
  var objFile = objFileSys.GetFile(location.pathname.slice(1));
  var path = objFile.ParentFolder + "\\..\\demo\\" + bat_name;
  var obj = new ActiveXObject("WScript.Shell");
  obj.Run(path + " 2809 F:"); // 2 values in double-quote represent arguments to the command passed.
}

function call_rtmnamerserver() {
    callbat_relativepath("rtm-naming.bat");
}

function call_rtseditor() {
    callbat_relativepath("RTSE.bat");
}

function call_OpencvRtmDemo_0() {
    callbat_relativepath("OpencvRtmDemo\\0_StartDemo.bat");
}


