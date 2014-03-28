function rtm_naming() {
  var objFileSys = new ActiveXObject("Scripting.FileSystemObject");
  var objFile = objFileSys.GetFile(location.pathname.slice(1));
  var path = objFile.ParentFolder + "\\..\\demo\\rtm-naming.bat";
  var obj = new ActiveXObject("WScript.Shell");
  obj.Run(path + " 2809 f:"); // 2 values in double-quote represent arguments to the command passed.
}
