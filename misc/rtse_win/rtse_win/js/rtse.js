/*
 * RT System Editor on the Web

*/

//// for RTSE  ///////////////////
var Rtc_table = new Array();

/// for RT-Component //////////////////
var Color_active = "#88ff88";
var Color_deactive = "#4444ff";
var Color_error = "#ff0000";

var Color_select = "#ff00ff";
var Color_mark = "#ffff00";

var Color_conenctedDataport = "#aaffaa";
var Color_conenctedCorbaport = "#aaaaff";
var Color_noconnection = "#4444ff";

var rtsh_com = "/cgi-bin/rtsh.py"

var property_window=null;
var edit_config_window=null;
var nsview_window=null;



///////////////
var updateTimerID = null;

function checkPath(path){
  try{
    if (path.charAt(path.length -1) == "/"){
       path = path.substr(0, path.length -1);
    }
  }catch(e){
    ;
  }
  return path;
}

function refreshProps2(s){
  console.log(s);
}

function requestComponentProps2(name)
{
  if (! name){ console.log("no name"); return; }
  var data=new Array();
  data['name'] = name;
  data['cmd'] = "rtcat";
  postRequest(rtsh_com, data, refreshProps2, null);
}

function updateComponentInfo(flag){
  stopUpdateTimer();
  requestComponentProps2();
}

function autoUpdateComponentInfo(){
  requestComponentProps2();
}

function stopUpdateTimer(){
  if(updateTimerID != null){
    clearTimer(updateTimerID);
    updateTimerID = null;
  }
}

function autoUpdateTimer(sec){
  if (sec <= 0) return;
  if(updateTimerID == null){
    updateTimerID = setTimer("autoUpdateComponentInfo()", sec);
  }else{
    clearTimer(updateTimerID);
    updateTimerID = setTimer("autoUpdateComponentInfo()", sec);
  }
}

function setTimer(func, n){
  if (n <= 0) return;
  var timerID = setTimeout(func, n*1000);
  return timerID;
}

function clearTimer(id){
  clearTimeout(id);
}



///////////////
//
function displayInfoMessage(msg)
{
  var infoview = document.getElementById('rtse_info');
  if(infoview){
    infoview.innerHTML=msg
  }
}

function getSelectedComponentPort(e, x1, y1, x2, y2, obj)
{
  if(x1 > x2) { var tmp = x1; x1=x2; x2=tmp; } 
  if(y1 > y2) { var tmp = y1; y1=y2; y2=tmp; } 
 

  for(var i=0;i<Rtc_table.length;i++){
    var Rtc = Rtc_table[i];
    var port = Rtc.checkPort(x1, y1, x2, y2);

    if(port){
      if(port && port != obj && port.isConnectablePort(obj)){
        return port;
      }else{
        return null;
      }
    }
  }
  return null;
}


function addConnector(p1, p2){
  var obj = new SvgConnector(p1[0],p1[1],p2[0],p2[1], '#8800ff','none', 1);
  getSvgCanvas().appendSvgObject(obj);
  return obj;
}

function newLine(x, y){
  var obj= new SvgLine(x, y, x+1, y, '#8800ff', 'none', 1);
  getSvgCanvas().appendSvgObject(obj);
  return obj;
}

function addComponent(x, y, cmp, flag){
  var dataports = cmp.Ports;
  var name = cmp.Name;
  var path = cmp.PathUrl;

  if( path.charAt(path.length -1)!= '/' ){ path = path+"/"; }
  var comp = findRtc(path+name);
  if(!comp){ comp = new RtObject(name, path); }

  comp.createSvgObjects(x, y, dataports);
  comp.obj = cmp;

  if (flag){ comp.showItem(); }

  return comp;
}


function allRtcActivate(){
  rtcs = getDisplayedRtcs();
  for ( x in rtcs){
	var rtc = rtcs[x];
    requestActivate(rtc.getFullName());
  }
}

function allRtcDeactivate(){
  rtcs = getDisplayedRtcs();
  for ( x in rtcs){
	var rtc = rtcs[x];
    requestDeactivate(rtc.getFullName());
  }
}

function getDisplayedRtcs(){
  var res = new Array();
  for ( x in Rtc_table){
	var rtc = Rtc_table[x];
	if(rtc.is_display()){
      res.push(rtc);
	}
  }
  return res;
}

function getAllRtcsFromNsView()
{
  var itms = document.getElementsByClassName('rtc');
  var res = new Array();

  for(x in itms){
    var rtc_item = itms[x];
    if(rtc_item.getAttribute){
	  res.push(rtc_item.getAttribute('id'));
	}
  }
  return res;
}

function refreshAll(){
  rtcs = getDisplayedRtcs();

  for(var i=rtcs.length; i > 0;i--){
    rtcs[i-1].updatePortColor();
  }

}

function exitAll(){
  rtcs = getAllRtcsFromNsView();

  for(var i=rtcs.length; i > 0;i--){
    requestExit(rtcs[i-1]);
  }

}


///// for NS View
/// toggle display tree.

function toggleDisplay(ele){
  if(ele.style.display == 'block'){
    ele.style.display = 'none';
  }else{
    ele.style.display = 'block';
  }
}

function nsItemFunc(ele){
  var nodes = ele.childNodes;
  var n = nodes.length;

  for(var i=0; i<n ;i++){
    var node = nodes[i];
    if(typeof node != 'undefined'){
      if(node.nodeName == 'UL'){
        toggleDisplay(node);
        return false;
      }
    }
  }
  return false;
}

////  
function isString(obj){
  return (Object.prototype.toString.call(obj)  == "[object String]");
}


/////
function removeCompItem(str){
 var itm = document.getElementById(str);
 if(itm){
   itm.parentNode.removeChild(itm);
 }

 return null;

}

function delZombieComponents(){
 requestDelZombie();
 return null;

}
////  create '<li>' 
var selectedElement = null;

function setSelectedItem(ele){
  if(selectedElement){
     selectedElement.style.background='#ffffff';
     selectedElement.style.color='#000000';
  }
  selectedElement = ele;
  selectedElement.style.background='#0000aa';
  selectedElement.style.color='#ffffff';
  
  var name = ele.innerHTML;
  var path = ele.getAttribute('path');
  var comp = component_list[path+"/"+name];
  if(comp){
    refreshComponentProp( comp );
  }else{
    dnd_showProps(null, path, name);
  }
}

function addSelectedComponent(){
  if(selectedElement){
    var name = selectedElement.innerHTML;
    var path = selectedElement.getAttribute('path');
    requestComponentInfo(path, name)
  }
}

function findCompItem(path, name){
  var eles = document.getElementsByClassName('rtc_item');
  if(eles){
    for(i in eles){
      if(eles[i].getAttribute){
        var name1 = eles[i].innerHTML; 
        var path1 = eles[i].getAttribute('path');
        if (name == name1 && checkPath(path) == checkPath(path1)){
          return eles[i];
        }
      }
    }
  }
  return null;
}

function mkCompItem(p, obj){
  var res,itm;

  if(isString(obj)){
    res = document.createElement("li");
    if(obj.substr(0,1) == '*'){
      res.className="zmb";
      res.onclick = function(e){ e.stopPropagation(); };
      res.innerHTML=obj.substr(1);
	}else if(obj.substr(0,1) == '#'){
      res.className="unknown";
      res.onclick = function(e){ e.stopPropagation(); };
      res.innerHTML=obj.substr(1);
    }else{
      res.className="rtc";
      res.onclick = function(e){ e.stopPropagation(); };
      itm = res.appendChild(document.createElement("A"));
      itm.className="rtc_item";
      itm.href="#";
      itm.innerHTML=obj;
      itm.setAttribute('path', p);
      itm.onclick = function(e){
                     setSelectedItem(this);
                     e.stopPropagation();
                  };
      res.setAttribute('id', p+'/'+obj);
      setDraggable(itm);
    }

  }else{
    res = document.createElement("li");
    res.className="ctx";
    var itm_val = document.createElement("span");
    itm_val.innerHTML = obj.folder;
    if( obj.components.length > 0 ){
      itm_val.className = "haschild";
    }
    res.appendChild( itm_val);
    res.onclick = function(e){
                     nsItemFunc(this);
                     e.stopPropagation();
                  };

    itm = res.appendChild( document.createElement("ul") );
    itm.style.display = 'none';

    p = p + "/"+ obj.folder;

    for(var i=0;i< obj.components.length; i++){
      itm.appendChild(mkCompItem(p, obj.components[i] ));
    }
  }
  
  return res;
}

/// create Name Service View
function mkNsView(txt){
  var content = JSON.parse(txt);
  var res, itm, itm_val;
  var p;

  res = document.createElement("ul");
  res.className = "host";
  res.setAttribute("hostname", content.hostname);
  p="/"+content.hostname;
  itm = res.appendChild( document.createElement("li"));
  itm.className = "host";

  itm_val = document.createElement("span");
  itm_val.innerHTML = content.hostname;
  if( content.components.length > 0 ){
    itm_val.className = "haschild";
  }
  itm.appendChild( itm_val);
 
  itm.onclick = function(e){
                   nsItemFunc(this);
                   e.stopPropagation();
                };

  itm = itm.appendChild( document.createElement("ul") );
  itm.style.display = 'none';

  for(var i=0; i< content.components.length; i++ ){
    itm.appendChild(mkCompItem(p, content.components[i] ));
  }

  return res;
}

function mkConnectors(id)
{
  var inside = document.getElementById('InportSide');
  var outside = document.getElementById('OutportSide');
  var ele = document.getElementById(id);
  
  var Rtc1 = findRtc(inside.rtc);
  var Rtc2 = findRtc(outside.rtc);
  
  if(!Rtc1 || !Rtc2){
    ele.innerHTML = "Can't find valid components.";
    selectedItems=[];
    return;
  }

  props = document.getElementById('props');
  props.innerHTML = Rtc1.getName() +"<->"+Rtc2.getName();

  var candidate_connection = new Array();

  var outports,inports;

  outports = Rtc2.ports;
  inports = Rtc1.ports;
  if(outports && inports){
    for(var i=0; i< outports.length; i++){
      for(var j=0; j< inports.length; j++){
        if(outports[i].isConnectablePort(inports[j])){
          candidate_connection.push(new Array(outports[i], inports[j]));
        }
      }
    }
  }

  /*
  outports = Rtc1.dataoutport;
  inports = Rtc2.datainport;

  for(var i=0; i< outports.length; i++){
    for(var j=0; j< inports.length; j++){
      if(outports[i].compDataType(inports[j])){
        candidate_connection.push(new Array(outports[i], inports[j]));
      }
    }
  }

  outports = Rtc2.dataoutport;
  inports = Rtc1.datainport;

  for(var i=0; i< outports.length; i++){
    for(var j=0; j< inports.length; j++){
      if(outports[i].compDataType(inports[j])){
        candidate_connection.push(new Array(outports[i], inports[j]));
      }
    }
  }

  outports = Rtc2.serviceport;
  inports = Rtc1.serviceport;

  if(outports && inports){
    for(var i=0; i< outports.length; i++){
      for(var j=0; j< inports.length; j++){
        if(outports[i].compServicePort(inports[j])){
          candidate_connection.push(new Array(outports[i], inports[j]));
        }
      }
    }
  }
  */

  if(candidate_connection.length == 0){
    ele.innerHTML = "Can't make connection between them.";
  }else{
    str = "<table>";

    for(var i=0; i<candidate_connection.length; i++){
      var outp = candidate_connection[i][0];
      var inp = candidate_connection[i][1];
      var inportStr = inp.getFullName() ;
      var outportStr = outp.getFullName();

      if(findConnector(inportStr, outportStr)){
        ctrlButton = "<img src=\"images/Connect.gif\" onClick=\"removeConnector2('"+outportStr+"','"+inportStr+"');\"><br>";
      }else{
        ctrlButton = "<img src=\"images/Disconnect.gif\" onClick=\"createConnector2('"+outportStr+"','"+inportStr+"');\"><br>";
      }
      str += "<tr><td>"+ctrlButton+"</td><td onClick=\"selectConnection('"+outp.getFullName()+"','"+inp.getFullName()+"')\">"+outp.getName() + "_" + inp.getName() +"<//td><tr>";
    }

    str += "</table>";
    ele.innerHTML = str;
  }
  selectedItems=[];
  return;
}

function removeConnector2(name1, name2){
  removeConnector(name1, name2);
  mkConnectors('property');
}

function createConnector2(name1, name2){
  createConnector(name1, name2, true);
  mkConnectors('property');
}


var currentSelectedConnector = null;

function selectConnection(outp, inp)
{
  if(currentSelectedConnector){
    currentSelectedConnector.hideSelectBox();
  }
  var conn = findConnector(outp, inp);
  if(conn){
    conn.showSelectBox();
    currentSelectedConnector = conn;
    return null;
  }

  var oname = outp.split(':'); 
  var iname = inp.split(':'); 
  var oport = getComponentPort(oname[0], oname[1]);
  var iport = getComponentPort(iname[0], iname[1]);
  var opos = oport.currentPos();
  var ipos = iport.currentPos();

  if(!connectorLine){
    connectorLine = new SvgLine(opos[0], opos[1], ipos[0], ipos[1], '#8800ff', 'none', 1);
    connectorLine.setAttribute('stroke-dasharray','2 2');
    getSvgCanvas().appendSvgObject(connectorLine);
  }else{
    connectorLine.setAttribute('x1', opos[0]);
    connectorLine.setAttribute('y1', opos[1]);
    connectorLine.setAttribute('x2', ipos[0]);
    connectorLine.setAttribute('y2', ipos[1]);
  }
}

function swapConnectors()
{
  var inside = document.getElementById('InportSide');
  var outside = document.getElementById('OutportSide');

  var tmpRtc = outside.rtc;
  var tmpname = outside.innerHTML;

  outside.rtc = inside.rtc;
  outside.innerHTML = inside.innerHTML;

  inside.rtc = tmpRtc;
  inside.innerHTML = tmpname;

  selectItems=[];
}

///////////////////////////////////
//  Drag and Drop
//
var addEventHandler = (function () {

  if (document.addEventListener) {
    return function (el, type, fn) {
      if (el && el.nodeName || el === window) {
        el.addEventListener(type, fn, false);
      } else if (el && el.length) {
        for (var i = 0; i < el.length; i++) {
          addEventHandler(el[i], type, fn);
        }
      }
    };
  } else {

    return function (el, type, fn) {
      if (el && el.nodeName || el === window) {
        el.attachEvent('on' + type, function () { return fn.call(el, window.event); });
      } else if (el && el.length) {
        for (var i = 0; i < el.length; i++) {
          addEventHandler(el[i], type, fn);
        }
      }
    };
  }
})();

/////
var dnd_token=';';

function setDraggable(el)
{
  if(el.nodeName == "A"){
    el.setAttribute('draggable', 'true');
  
    addEventHandler(el, 'dragstart', function (e) {
      e.dataTransfer.effectAllowed = 'copy'; // only dropEffect='copy' will be dropable
      var path = this.getAttribute('path'); 
      e.dataTransfer.setData('Text', path + dnd_token + this.firstChild.nodeValue); 
    });
  }
}

function setDropFunc(el, func)
{
  addEventHandler(el, 'dragover', function (e) {
    if (e.preventDefault) e.preventDefault(); // allows us to drop
    this.className = 'over';
    e.dataTransfer.dropEffect = 'copy';
    return false;
  });

  addEventHandler(el, 'dragenter', function (e) {
    this.className = 'over';
    if (e.preventDefault) e.preventDefault(); // allows us to drop
     e.dataTransfer.dropEffect = 'copy';
    return false;
  });

  addEventHandler(el, 'dragleave', function (e) {
    this.className = '';
  });

  addEventHandler(el, 'drop', func);
}


//////
// for addComponent
var add_cmp_x=10;
var add_cmp_y=10;

function dnd_addComponent(e, path, name)
{
  var canvas = getSvgCanvas();
  add_cmp_x = canvas.getX(e.pageX);
  add_cmp_y = canvas.getY(e.pageY);

  requestComponentInfo(path, name);
}

function dnd_showProps(e, path, name)
{
  var props = document.getElementById('props');
  props.innerHTML=name;
  var view = document.getElementById("property");
  view.innerHTML='<img src="images/indicator.gif">';

  requestComponentProp(path, name)
}

function dnd_setInport(e, path, name)
{
  var inport = document.getElementById('InportSide');
  if(path.substr(-1) == '/'){
    inport.rtc = path + name;
  }else{
    inport.rtc = path + "/" +name;
  }
  inport.innerHTML=name;
}

function dnd_setOutport(e, path, name)
{
  var outport = document.getElementById('OutportSide');
  if(path.substr(-1) == '/'){
    outport.rtc = path + name;
  }else{
    outport.rtc = path + "/" +name;
  }
  outport.innerHTML=name;

}

/***************************/
//
//  initilize RTSE on the Web
//
function initRtse(w,h)
{
  var diagram = document.getElementById('diagram');
  setDropFunc(diagram, function (e) {
    var full_name = e.dataTransfer.getData('Text');
    var str = full_name.split(dnd_token);
    var c_path = str[0];
    var c_name = str[1];
    dnd_addComponent(e, c_path, c_name);
    clearSelectedObjects();
    return false;
  });

  var props = document.getElementById('props');
  setDropFunc(props, function (e) {
    var full_name = e.dataTransfer.getData('Text');
    var str = full_name.split(dnd_token);
    var c_path = str[0];
    var c_name = str[1];
    dnd_showProps(e, c_path, c_name);
    return false;
  });

  var inport = document.getElementById('InportSide');
  setDropFunc(inport, function (e) {
    var full_name = e.dataTransfer.getData('Text');
    var str = full_name.split(dnd_token);
    var c_path = str[0];
    var c_name = str[1];
    dnd_setInport(e, c_path, c_name);
    return false;
  });

  var outport = document.getElementById('OutportSide');
  setDropFunc(outport, function (e) {
    var full_name = e.dataTransfer.getData('Text');
    var str = full_name.split(dnd_token);
    var c_path = str[0];
    var c_name = str[1];
    dnd_setOutport(e, c_path, c_name);
    return false;
  });

  component_list = new Array();

  var ele = new SvgCanvas('diagram', w, h);
}

///////  
//   RPC for Ajax
//
function newXMLRequest(){
  if(this.XMLHttpRequest){
    return new XMLHttpRequest();
  }else {
    return new ActiveXObject("Microsoft.XMLHTTP");
  }
}

function createRequestData(data){
  var str="app=rtse";
  for (var i in data){
    str = str +"&"+ i +"="+encodeURIComponent(data[i]);
  }
  return str;
}

function postRequest(url, data, func, args){
  var postData=createRequestData(data);
  var obj=newXMLRequest();

  obj.onreadystatechange = function(){
    if (obj.readyState == 4 && obj.status == 200){
      if(args){
        func(obj.responseText, args);
      }else{
        func(obj.responseText);
      }
    }
  }
  obj.open("POST", url, true);
  obj.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
  obj.send(postData);
  
}

///////////////////////
// Check '<ul>' element
function isULElement(obj){
  return (Object.prototype.toString.call(obj)  == "[object HTMLUListElement]");
}

//////
//  callback function
function refreshNsView(s){
  var view = document.getElementById("NsView");
  var nsview = mkNsView(s);

  hostname = nsview.getAttribute("hostname");

  nodes = view.childNodes;
  for(var i=0; i<nodes.length; i++){
    if(isULElement(nodes[i])){
      if( nodes[i].getAttribute("hostname") == hostname){
        view.removeChild(nodes[i]);
      }
    }
  }
  var indicator = document.getElementById('indicator');
  if(indicator){ indicator.style.display='none'; }
  view.appendChild( nsview );
}

/////

function addNsView(){
  var hin = document.getElementById('hostname_in');
  if(hin.value){
    var indicator = document.getElementById('indicator');
    indicator.style.display='block';
    getNsView(hin.value);
  }
}

function getNsView(host){
  var data=new Array();
  data['host'] = host;
  data['cmd'] = "rtlist";
  postRequest(rtsh_com, data, refreshNsView, null);
  return;
}

/**** Request to a component ********/

function requestComponentProp(path, name)
{
  var data=new Array();
  data['cwd'] = path;
  data['name'] = name;
  data['cmd'] = "rtcat2";
  postRequest(rtsh_com, data, refreshProps, null);
}

function requestComponentsState( names ){
  var data=new Array();
  data['names'] = names;
  data['cmd'] = "getState";
  postRequest(rtsh_com, data, refreshState, null);
}

function requestComponentInfo(path, name)
{
  var data=new Array();
  data['cwd'] = path;
  data['name'] = name;
  data['cmd'] = "rtcat2";

  if(!component_list[path +"/"+name]){
    displayInfoMessage("<font color=\"blue\">Request Component Info..., Please wait for a while.</font>");
    postRequest(rtsh_com, data, addRtc, 1);

  }else{
    var comp = component_list[path +"/"+name];
    if(comp.display == 'none'){
      comp.showItem();
    }
    setObjectPos(component_list[path +"/"+name], add_cmp_x, add_cmp_y);
  }
}

function requestConnection(obj1, obj2)
{
  var data=new Array();

  data['name1'] = obj1.getFullName();
  data['name2'] = obj2.getFullName();
  data['cmd'] = "rtcon";
  displayInfoMessage("<font color=\"blue\">Request to connect components..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishConnection, null);
  selectedItems=[];
}

function requestConnectionInfo(obj1, obj2)
{
  var data=new Array();

  data['name1'] = obj1.getFullName();
  data['name2'] = obj2.getFullName();
  data['cmd'] = "rtconinfo";

  postRequest(rtsh_com, data, finishConnection, null);
  selectedItems=[];
}

function finishPortInfo(s, obj1)
{
  var p = JSON.parse(s);
  console.log(p);

}

function requestPortInfo(obj1)
{
  var data=new Array();

  data['name'] = obj1.getFullName();
  data['cmd'] = "rtinfo";

  postRequest(rtsh_com, data, finishPortInfo, obj1);
  selectedItems=[];
}

function requestDisconnection(obj1, obj2)
{
  var data=new Array();

  data['name1'] = obj1.getFullName();
  data['name2'] = obj2.getFullName();
  data['cmd'] = "rtdis";
  displayInfoMessage("<font color=\"blue\">Request to disconnect components..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishDisconnection, null);
  selectedItems=[];
}

function requestExit(fullname)
{
  var data=new Array();

  data['name1'] = fullname;
  data['cmd'] = "rtexit";
  displayInfoMessage("<font color=\"blue\">Exit the component..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishExit, null);
  selectedItems=[];

}

function requestActivate(fullname)
{
  var data=new Array();

  data['name1'] = fullname;
  data['cmd'] = "rtact";
  displayInfoMessage("<font color=\"blue\">Actiate the component..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishActivation, null);
  selectedItems=[];

}

function requestDeactivate(fullname)
{
  var data=new Array();

  data['name1'] = fullname;
  data['cmd'] = "rtdeact";
  displayInfoMessage("<font color=\"blue\">Deactiate the component..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishDeactivation, null);
  selectedItems=[];

}

function requestReset(fullname)
{
  var data=new Array();

  data['name1'] = fullname;
  data['cmd'] = "rtreset";
  displayInfoMessage("<font color=\"blue\">Reset the component..., Please wait for a while.</font>");
  postRequest(rtsh_com, data, finishReset, null);
  selectedItems=[];
}

function requestGetStatus(fullname)
{
  var data=new Array();

  data['name1'] = fullname;
  data['cmd'] = "rtstate";
  postRequest(rtsh_com, data, finishGetStatus, null);
  selectedItems=[];
}

function requestDelZombie()
{
  var data=new Array();
  data['hosts'] = getHostnames();
  data['cmd'] = "delzombie";

  postRequest(rtsh_com, data, finishDelZombie, null);
}

function requestSetConfigurationParam(name, setname, conf, value)
{
  var data=new Array();
  data['name'] = name;
  data['setname'] = setname;
  data['conf'] = conf;
  data['value'] = value;
  data['cmd'] = "rtconf_set";

  postRequest(rtsh_com, data, finishSetConfig, null);
}

function requestSetConfigurationSet(name, setname)
{
  var data=new Array();
  data['name'] = name;
  data['setname'] = setname;
  data['cmd'] = "rtconf_act";

  postRequest(rtsh_com, data, null, null);
}

/*** finish Procedure *****/

function finishConnection(s){
  displayInfoMessage(s);
  mkConnectors('Connector');
  refreshAll();
}

function finishConnectionInfo(s){
  mkConnectors('Connector');
  refreshAll();
}

function finishDisconnection(s){
  displayInfoMessage(s);
  mkConnectors('Connector');
  refreshAll();
}

function finishExit(s){
  var data = JSON.parse(s);
  deleteRtc(data.name);
  removeCompItem(data.name);
  displayInfoMessage(data.name+" exited");
}

function finishActivation(s){
  var data = JSON.parse(s);
  var comp = findRtc(data.name);
  comp.setStatus(data.result);
  showRtcMenu([comp]);
}

function finishDeactivation(s){
  var data = JSON.parse(s);
  var comp = findRtc(data.name);
  comp.setStatus(data.result);
  showRtcMenu([comp]);
}

function finishReset(s){
  var data = JSON.parse(s);
  var comp = findRtc(data.name);
  comp.setStatus(data.result);
  showRtcMenu([comp]);
}

function finishGetStatus(s){
  var data = JSON.parse(s);
  var comp = findRtc(data.name);

  comp.setStatus(data.result);
}

function finishDelZombie(s){
  alert(s);
  var itms = document.getElementsByClassName('zmb');
  var len = itms.length;
  for(var i=len-1;i>=0;i--){
    itms[i].parentNode.removeChild(itms[i]);
  }
}

function finishSetConfig(s){
  var data = JSON.parse(s);
  var itms = document.getElementsByClassName('config_input_item');
  var len = itms.length;

  for(var i=0;i<len;i++){
    var ele = itms[i];
    if(ele.name == data.ConfigName){
      resetInputColor(ele);
      return;
    }
  }
}



/****  function ****/
function editConfigurationValue(){
  edit_config_window = window.open("editConfig.html", "Configure", "width=400,height=400,scrollbars=1");
  edit_config_window.focus();
}

function openPropWindow(){
  property_window = window.open("props.html", "Properties", "width=350,height=600,scrollbars=1");
  property_window.focus();
}

function isValueChanged(ele){

  if(ele.tagName == "INPUT"){
     if(ele.defaultValue != ele.value) { return true; }
  }
  return false;
}

function applyConfigurationValues(){
  var form = document.getElementById("configuration_form");
  var ele = document.getElementById("active_config_set_name");
  var setname=ele.innerHTML;

  var compname = form.rtc_name.value;
  for(var i=0; i< form.elements.length ;i++){
    var ele = form.elements[i];
    if(isValueChanged(ele)){
      requestSetConfigurationParam(compname, setname, ele.name, ele.value);
    }
  }
  requestSetConfigurationParam(compname, setname, ele.name, ele.value);
  
}

function setModifyColor(ele){
  ele.style.backgroundColor="#ffff00";
}

function resetInputColor(ele){
  ele.style.backgroundColor="#ffffff";
}

function createParamsView(comp){
  var params  = comp.ConfigParams;
  var exprops  = comp.ExtraProperties;
  var activeSet = comp.ActiveConfig;
  var result="";
  var rtc_name = comp.PathUrl+"/"+comp.Name;

  if (!params || params.length == 0){
    return "No parameter";
  }
  result += "<form id=\"configuration_form\">";
  result += "<input type=\"hidden\" name=\"rtc_name\" value=\""+rtc_name+"\">";
  result += "<input type=\"hidden\" name=\"config_set_name\" value=\""+activeSet+"\">";
  result += "<table class=\"configview\">";
  result += "<tr><th>ParamName</th><th>Value</th>";
  params.sort();

  for(i in params){
    var name = "conf."+activeSet+"."+params[i];

    var val= "<input class=\"config_input_item\" type=\"text\" name=\""+params[i]+"\" value=\""+exprops[name]+"\" onChange=\"setModifyColor(this);\">";
    result += "<tr>";
    result += "<td class=\"title\">"+params[i]+"</td>";
    result += "<td class=\"value\">"+val+"</td>";
    result += "</tr>";
  }
  result += "</table>";
  result += "</form>";
  return result;
}

function getConfigParams(props, setname){
  var res = new Array();
  var keys=getMatchKeys(props,"conf."+setname+".");

  for(i in keys){
    res[keys[i]] = props[keys[i]];
  }
  return res.sort();
}

function changeConfigSet(setname){
 if (selectedItems[0]){
   var comp = selectedItems[0].obj;
   var props = comp["ExtraProperties"];
   var params = getConfigParams(props, setname);
   var form = document.getElementById("configuration_form");
   var paramnames  = comp.ConfigParams;
   
   var title = document.getElementById("active_config_set_name");

   title.innerHTML = setname;

   for(var i in paramnames){
     form.elements[paramnames[i]].value = params["conf."+setname+"."+paramnames[i]];
     setModifyColor(form.elements[paramnames[i]]);
   }
 }else{
   alert("ERROR: No component selected or the component has not drop on the system diagram");
 }

}

function createParamSetView(comp){
  var conf_sets = comp.ConfigSet;
  var active_set = comp.ActiveConfig;
  
  var result="";

  if (!conf_sets){
    return "No configuration set";
  }
  result += "<table class=\"configview\">";
  result += "<tr><th>Active</th><th>Name</th>";
  for(i in conf_sets){
    var name = conf_sets[i];
    result += "<tr>";
    if(name == active_set){
      result += "<td><input onChange=\"changeConfigSet('"+name+"');\" type=\"radio\" name=\"config_set\" value=\""+name+"\" checked></td>";
    }else{
      result += "<td><input onChange=\"changeConfigSet('"+name+"');\" type=\"radio\" name=\"config_set\" value=\""+name+"\"></td>";
    }
    result += "<td>"+name+"</td>";
    result += "</tr>";
  }
  result += "</table>";
  return result;
}


function refreshConfigView(comp){
  var confset = document.getElementById("ConfigSetList");
  var confview = document.getElementById("ConfigrationList");
  var confname = document.getElementById("active_config_set_name");
//  var comp = JSON.parse(s);

  confname.innerHTML = comp.ActiveConfig;
  if(comp.ConfigSet){
    confset.innerHTML= createParamSetView(comp);
    confview.innerHTML= createParamsView(comp);
  }else{
    confset.innerHTML="No configuration";
    confview.innerHTML="No configuration";
  }
//  confset = comp.ConfigSets  
}

function refreshProps(s){
  var view = document.getElementById("property");
  var jdata = JSON.parse(s);
  var path = jdata.PathUrl+"/"+jdata.Name;

  if(component_list[ path ]){
    component_list[path].obj = jdata;
    console.log("Update component data");
  }else{
    addRtc(s, 0);
  }
  
  view.innerHTML = createPropTable(jdata);

  if(property_window && !property_window.closed){ openPropWindow(); }
  refreshConfigView(jdata);
}

function refreshComponentProp(comp){
  var view = document.getElementById("property");
  view.innerHTML = createPropTable(comp.obj);

  if(property_window && !property_window.closed){ openPropWindow(); }
  refreshConfigView(comp.obj);
}

function refreshState(s){
  var cmp = JSON.parse(s);
  console.log(s);  
}

function restoreConnection(Rtc){
  /// restore connection
  var cmp = Rtc.obj;
  if(cmp){
    if(cmp.Ports){
      for(var i=0; i<cmp.Ports.length;i++){
        var p = cmp.Ports[i];
        for(var j=0;j<p.Connections.length;j++){
          createConnectors(Rtc.getFullName()+":"+p.Name, p.Connections[j].DestPorts, false );

        }
      }
    }
  }
}

function addRtc(s, flag){
  try{
    var cmp = JSON.parse(s);
    var Rtc = addComponent(add_cmp_x, add_cmp_y, cmp, flag);

    Rtc.setStatus(cmp.State);
    restoreConnection(Rtc);

    component_list[cmp.PathUrl+"/"+cmp.Name] = Rtc;
    Rtc.updatePortColor();
    if(!flag){
      Rtc.deleteItem();
    }

    displayInfoMessage("");
  }catch(e){
    displayInfoMessage("Error in addRtc."+e);
  }
}


function commandFinishAlert(s){
  alert(s);
}

///////
//
function findRtc(name){
  var i;
  for(i=0; i<Rtc_table.length;i++){
    if(Rtc_table[i].getFullName() == name){
      return Rtc_table[i];
    }
  }
  return null;
}

function deleteRtc(name){
  var i;
  for(i=0; i<Rtc_table.length;i++){
    if(Rtc_table[i].getFullName() == name){
      var comp = array_shift(Rtc_table, i);
      comp.exit();
      delete comp;
    }
  }
  return null;
}

function getComponentPort(cname, pname){
  var Rtc = findRtc(cname);
  if(Rtc){
    return Rtc.findPort(pname);
  }
  return null;
}

function createConnectors(name1, names2, flag){
  for(var i=0; i < names2.length;i++){
    var name2 = names2[i];
    updateConnector(name1, name2, flag);
  }
}

function updateConnector(name1, name2, flag){
  if(findConnector(name1, name2)){
    if(flag){ alert("connector exists"); }
    return null;
  }
  var c_name1 = name1.split(':');
  var c_name2 = name2.split(':');

  var start_obj = getComponentPort(c_name1[0], c_name1[1]);
  var end_obj = getComponentPort(c_name2[0], c_name2[1]);

  if( start_obj && end_obj){
    updateConnectorFromItem(start_obj, end_obj)
  }else{
    if(flag){ alert("Can't find Rtc: "+c_name1+" or "+c_name2); }
  }

  if(connectorLine){
    connectorLine.remove();
    connectorLine = null;
  }
}

function updateConnectorFromItem(port1, port2){
  var connector = findConnector(port1.getFullName(), port2.getFullName());

  if(!connector){
    ///// I should rewrite this procedure.....
    if(port1.porttype == "corba" && port2.porttype == "corba"){
      connector = new ConnectorObject(port1, port2, '', '', '');
    }else{
      connector = new ConnectorObject(port1, port2, 'corba_cdr', 'push', 'flush');
    }
    connectorList.push(connector);
    port1.updateColor();
    port2.updateColor();

    /// Request Connection
    requestConnectionInfo(port1, port2)
  }
  
  return connector;
}

function createConnector(name1, name2, flag){
  if(findConnector(name1, name2)){
    if(flag){ alert("connector exists"); }
    return null;
  }
  var c_name1 = name1.split(':');
  var c_name2 = name2.split(':');

  var start_obj = getComponentPort(c_name1[0], c_name1[1]);
  var end_obj = getComponentPort(c_name2[0], c_name2[1]);

  if( start_obj && end_obj){
    createConnectorFromItem(start_obj, end_obj)
  }else{
    if(flag){ alert("Can't find Rtc: "+c_name1+" or "+c_name2); }
  }

  if(connectorLine){
    connectorLine.remove();
    connectorLine = null;
  }
}

function createConnectorFromItem(port1, port2){
  var connector = findConnector(port1.getFullName(), port2.getFullName());

  if(!connector){
    ///// I should rewrite this procedure.....
    if(port1.porttype == "corba" && port2.porttype == "corba"){
      connector = new ConnectorObject(port1, port2, '', '', '');
    }else{
      connector = new ConnectorObject(port1, port2, 'corba_cdr', 'push', 'flush');
    }
    connectorList.push(connector);
    port1.updateColor();
    port2.updateColor();
    /// Request Connection
    requestConnection(port1, port2)
  }
  
  return connector;
}

function getHostnames(){
  var eles = document.getElementsByClassName('host')
  var hostnames = new Array()

  for(var i=0; i< eles.length; i++){
	 name = eles[i].getAttribute("hostname");
	 if(name){
	   hostnames.push(name);
    }
  }
  return  hostnames.join(',')

}

function drawContent(){
  var str = getSvgCanvas().getSvgContent();
  alert(str);
}

function showRtcMenu(objs){
  if(!objs || objs.length == 0){
    displayInfoMessage("");

  }else if(objs.length == 1){
    var obj = objs[0];
    if(obj.getAttribute('Type') != 'RTC'){ return; }

    var name = obj.getFullName();

    var msg = '<img src="images/Activate.gif" onClick="requestActivate(\''+name+'\');" >';
        msg += '<img src="images/Deactivate.gif" onClick="requestDeactivate(\''+name+'\');" >';
	msg += '| &nbsp;&nbsp;'
        msg += '<img src="images/Delete.png" onClick=\"deleteComp(\''+name+'\');\" value="Del">';
	msg += '| &nbsp;&nbsp;'
	msg += '<input type="button" onClick="requestReset(\''+name+'\');" value=\"Reset\" >';
	msg += '| &nbsp;&nbsp;'
	msg += '<img src="images/Exit.gif" onClick="requestExit(\''+name+'\');" >';
	msg += '<input type="button" onClick="flipPortPos(\''+name+'\');" value=\"FlipPorts\" >';
	msg += '<input type="button" onClick="toggleDisplayName(\''+name+'\');" value=\"ToggleName\" >';

     if(getSvgMode() == "selectRtcMode"){
	msg += "&nbsp;&nbsp;("+getSvgMode() +")";
     }

    displayInfoMessage(msg);

  }else if(objs.length == 2){
    var obj1 = objs[0];
    var obj2 = objs[1];
    if(obj1.getAttribute('Type') != 'RTC' || obj2.getAttribute('Type') != 'RTC'){
      return;
    }

    var name1 = obj1.getFullName();
    setOutportSide(name1);

    var name2 = obj2.getFullName();
    setInportSide(name2);

    props = document.getElementById('props');
    props.innerHTML=obj1.getName() +"<->"+obj2.getName();

    var msg = "";
	msg += '<img src="images/Connection.gif" onClick="mkConnectors(\'property\');" value="Conn">';

	msg += '<input type="button" onClick="mkConnectors(\'property\');" value="Conn">';

    displayInfoMessage(msg);
  }else{
    displayInfoMessage(objs);
  }
}

function setInportSide(name){
  var Rtc = findRtc(name);
  if(Rtc){
    dnd_setInport(null, Rtc.path,  Rtc.name);
  }
}

function setOutportSide(name){
  var Rtc = findRtc(name);
  if(Rtc){
    dnd_setOutport(null, Rtc.path,  Rtc.name);
  }
}

function deleteComp(name){
  var Rtc = findRtc(name);
  if(Rtc){
    Rtc.deleteItem();
	  selectedItems = [];
  }
}

function flipPortPos(name){
  var Rtc = findRtc(name);
  if(Rtc){
    Rtc.flipPorts();
  }
}

function toggleDisplayName(name){
  var Rtc = findRtc(name);
  if(Rtc){
    Rtc.toggleDisplayName();
  }
}

function findConnector(name1, name2){
  var cname1 = name1+"_"+name2;
  var cname2 = name2+"_"+name1;
  for(var i=0;i<connectorList.length;i++){
    if(connectorList[i] && (connectorList[i].name == cname1 || connectorList[i].name == cname2)){
      return connectorList[i];
    }
  }
  return null;
}

function removeConnector(name1, name2){
  var cname1 = name1+"_"+name2;
  var cname2 = name2+"_"+name1;

  for(var i=0;i<connectorList.length;i++){
    if(connectorList[i] && (connectorList[i].name == cname1 || connectorList[i].name == cname2)){
      var conn = array_shift(connectorList, i);
      conn.clean();
      sobj = conn.start_obj;
      eobj = conn.end_obj;
      delete conn;
      array_remove(selectedItems, conn);
      requestDisconnection(sobj,eobj);
      return;
    }else{
      console.log("connector not found");
    }
  }
  return;
}

function removeConnectorFromList(obj){
  for(var i=0;i<connectorList.length;i++){
    if(connectorList[i] && connectorList[i] == obj){
      var conn = array_shift(connectorList, i);
      conn.clean();
      delete conn;
      return;
    }
  }
  return;
}

function getConnector(name1, name2){
  var cname1 = name1+"_"+name2;
  var cname2 = name2+"_"+name1;

  for(var i=0;i<connectorList.length;i++){
    if(connectorList[i] && (connectorList[i].name == cname1 || connectorList[i].name == cname2)){
      return connectorList[i];
    }
  }
  return null;
}

function resetConnectorController(name1, name2){
  var conn =  getConnector(name1, name2);

  if(conn){
    conn.cm_flag=false;
    conn.c1_flag=false;
    conn.c2_flag=false;
    conn.updatePos();
  }else{
    alert("no");
  }
  selectedItems=[];
}

function showConnectorMenu(obj){

  if(obj){
    var inportStr = obj.start_obj.getFullName();
    var outportStr = obj.end_obj.getFullName();

    var msg = " <img src=\"images/Connect.gif\" onClick=\"removeConnector('"+outportStr+"','"+inportStr+"');\"> | ";

        msg += " <input type=\"button\" onClick=\"resetConnectorController('"+outportStr+"','"+inportStr+"');\" value=\"Reset\">";

    displayInfoMessage(msg);
  }else{
    displayInfoMessage(obj);
  }
}

////// Convert JSON -> HTML
//
function createPropItem(title, data){
//  if (!data) { return ""; }
  return "<tr><td>"+title+"</td><td>" + data + "</td></tr>";
}

function getMatchKeys(ar, k){
  var res = new Array();
  for(var key in ar){
    if (key.substr(0, k.length) == k){
      res.push(key);
    }
  }
  return res.sort();
}

function in_array(k, ar){
  for(var i in ar){
    var v=ar[i];
    if (k.substr(0, v.length) == v){ return true; }
  }

  return false;
}

function getMatchKeys2(ar, k, ext){
  var res = new Array();
  for(var key in ar){
    if (key.substr(0, k.length) == k && !in_array(key, ext)){
      res.push(key);
    }
  }
  return res.sort();
}

function toggleRtcProp(id){
  var ele = document.getElementById(id);
  var ele_c = document.getElementById(id+'_close');
  if(ele.style.display == 'none'){
    ele_c.style.display='none';
    ele.style.display='block';
  }else{
    ele_c.style.display='block';
    ele.style.display='none';
  }
}

function createExtPropTable(props){
  var result="";
  var names1=["implementation_id", "type_name", "description", "version", "vendor", "category", "activity_type", "max_instance", "language", "lang_type"];
  var names2 = getMatchKeys2(props, "conf.",  ["conf.__widget__.", "conf.__constraints__."]);
  var names3 =[ "conf.__widget__.", "conf.__constraints__.", "sdo.service.", "kind", "instance_name", "config.version", "openrtm.", "os.", "corba.", "exec_cxt.", "logger.", "naming.", "config_file", "port." ];

  var names = names1.concat(names2, names3);

  result += "<div id=\"rtc_properties_close\" >&hellip;</div>"; 
  result += "<table id=\"rtc_properties\" style=\"display:none;\" >"; 

  for(var i in names){
    var name = names[i];
    if (name.charAt(name.length -1) == '.'){
      keys = getMatchKeys(props, name);
      for(var i in keys){
        var k = keys[i];
        if(props[k]){
          result += createPropItem(k, props[k]);
        }
      }
    }else{
      if(props[name]){
        result += createPropItem(name, props[name]);
      }
    }
  }
  result += "</table>"; 
  return result;
}


function createEcTable(ecs){
  var result="";
  result += "<div id=\"ec_props_close\">&hellip;</div>"; 
  result += "<div id=\"ec_props\" style=\"display:none\">"; 
  for(var i in ecs){
    var ec = ecs[i];
    result += "<table>"; 
    result += createPropItem("ID", ec.Handle);
    result += createPropItem("State", ec.State);
    result += createPropItem("Kind", ec.Kind);
    result += createPropItem("Rate", ec.Rate);
    result += createPropItem("Owner", ec.Owner);
    result += "</table>"; 
  }
  result += "</div>"; 
  return result;
}

function createPortTable(port,idx){
  var result="";
  var id = "port_"+idx;
  result += "<table id=\""+id+"\" style=\"display:none;\">"; 
  result += createPropItem("Name", port["Name"]);

  if(port.Type == "CorbaPort"){
    result += createPropItem("PortType", port["Properties"]["port.port_type"]);
    result += createPropItem("InterfaceName", port["Interfaces"][0]["InstanceName"]);
    result += createPropItem("TypeName", port["Interfaces"][0]["TypeName"]);
    result += createPropItem("Polarity", port["Interfaces"][0]["Polarity"]);
  }else{
    result += createPropItem("DataType", port["Properties"]["dataport.data_type"]);
    result += createPropItem("InterfaceType", port["Properties"]["dataport.interface_type"]);
    result += createPropItem("DataflowType", port["Properties"]["dataport.dataflow_type"]);
    result += createPropItem("SubscriptionType", port["Properties"]["dataport.subscription_type"]);
  }
  result += "</table>";

  if(port.Type == "CorbaPort"){
    result += "<table id=\""+id+"_close\">";
    result += "<tr><td>"+port["Name"]+" # "+port["Interfaces"][0]["InstanceName"]+"("+port["Properties"]["dataport.subscription_type"]+")<td></tr>";
    result += "</table>";
  }else{
    result += "<table id=\""+id+"_close\">";
    result += "<tr><td>"+port["Name"]+" # "+port["Properties"]["dataport.data_type"]+"<td></tr>";
    result += "</table>";
  }
  return result;
}



function createPropTable(jdata){
//  var jdata = JSON.parse(data);

  var propName = ["PathUrl", "InstanceName", "TypeName", "Description", "Version", "Vendor", "Category", "State"];
  var result = "";

  if(!jdata){ return result; }
  jdata.Version = jdata.ExtraProperties.version;
  jdata.Vendor = jdata.ExtraProperties.vendor;

  result += "<table id=\"rtc_property_table\" class=\"props\">"; 
  for( var i in propName){
    var name  = propName[i];
    var value  = jdata[name];
    result += createPropItem(name, value);
  }
/// ExtraProperties
  extProps = createExtPropTable(jdata.ExtraProperties);
  result += "<tr><td onClick=\"toggleRtcProp('rtc_properties');\">properties</td><td>" + extProps + "</td></tr>";

/// ExecutionContext
  ecs = createEcTable(jdata.ExecutionContext);
  result += "<tr><td onClick=\"toggleRtcProp('ec_props');\">EC</td><td>" + ecs + "</td></tr>";

/// ExecutionContext
  for(var j in jdata.Ports){
    var port = jdata.Ports[j];
    data = createPortTable(port, j);
    if(port.Type == "DataInPort"){
      var id = "port_"+j
      result += "<tr><td onClick=\"toggleRtcProp('"+id+"');\">InPort</td><td>" + data + "</td></tr>";

    }else if(port.Type == "DataOutPort"){
      result += "<tr><td onClick=\"toggleRtcProp('"+id+"');\">OutPort</td><td>" + data + "</td></tr>";

    }else if(port.Type == "CorbaPort"){
      result += "<tr><td onClick=\"toggleRtcProp('"+id+"');\">ServicePort</td><td>" + data + "</td></tr>";
    }else{
    }
  }

  result += "</table>"; 
//  result=data;
  return result;
}

///////////////
var ConnectorObject = function(start, end, interfacetype, dataflowtype, subscriptiontype ){
  if(start.porttype == 'in' && end.porttype == 'out'){
    this.start_obj=start;
    this.end_obj=end;

  }else{
    this.start_obj=end;
    this.end_obj=start;
  }

  this.name =  this.start_obj.getFullName() + "_" + this.end_obj.getFullName() ;
  this.interface_type=interfacetype;
  this.dataflow_type=dataflowtype;
  this.subscription_type = subscriptiontype;

  this.data_type=start.datatype;

  var spos = this.start_obj.currentPos();
  var epos = this.end_obj.currentPos();

  SvgConnector.call(this, spos[0], spos[1], epos[0], epos[1], '#8800ff','none', 1);

  this.setAttribute("Type", "Connector");

  this.start_obj.pushConnector(this);
  this.end_obj.pushConnector(this);

  this.setDir(this.start_obj.place, this.end_obj.place);
  this.updatePos();

  getSvgCanvas().appendSvgObject(this);
}

ConnectorObject.prototype= new SvgConnector();

ConnectorObject.prototype.clean=function(){
  this.start_obj.removeConnector(this);
  this.end_obj.removeConnector(this);
  this.hideSelectBox();

  getSvgCanvas().removeSvgObject(this);
}

ConnectorObject.prototype.updatePos=function(){
  var p1 = this.start_obj.currentPos();
  var p2 = this.end_obj.currentPos();

  this.setDir(this.start_obj.place, this.end_obj.place);
  SvgConnector.prototype.update.call(this, p1[0], p1[1], p2[0], p2[1]);
}

ConnectorObject.prototype.moveConnector=function(dist, dx, dy){
  var points = this.getTerminalPoints();

  if(points){
    if(dist == 'start'){
      var p1 = this.start_obj.currentPos();
      x1 = parseInt(p1[0]) + dx;
      y1 = parseInt(p1[1]) + dy;
      x2 = points[1][0];
      y2 = points[1][1];
    }else{
      var p2 = this.end_obj.currentPos();
      x1 = points[0][0];
      y1 = points[0][1];
      x2 = parseInt(p2[0]) + dx;
      y2 = parseInt(p2[1]) + dy;
    }

    this.setDir(this.start_obj.place, this.end_obj.place);
    this.update(x1, y1, x2, y2);
  }
}

ConnectorObject.prototype.show=function(){
  this.setAttribute('display', 'block');
}

ConnectorObject.prototype.hide=function(){
  this.setAttribute('display', 'none');
}

ConnectorObject.prototype.disconnect=function(){
  this.start_obj.obj.removeConnector(this);
  this.end_obj.obj.removeConnector(this);
  getSvgCanvas().removeSvgObject(this);

  requestDisconnection(this.start_obj, this.end_obj);
}

ConnectorObject.prototype.getDirection=function(portObj){
  var direction = null;
  if(this.start_obj == portObj){ 
    direction = 'start';
  }else if(this.end_obj == portObj){
    direction = 'end';
  }
  return direction;
}

ConnectorObject.prototype.onMouseUp=function(e, dx, dy){
  SvgConnector.prototype.onMouseUp.call(this, e, dx, dy);
  if(this.selected){ showConnectorMenu(this); }
}

/////////////
var PortObject = function(name, type, datatype){
  this.name=name;
  this.porttype=type;
  this.datatype=datatype;

  this.connectors = new Array();
  this.owner = null;
  this.label=null;

  if(this.porttype == 'in'){
    this.text_color = '#ff00ff';
    this.place = 'w';
    //this.place = 'e';
  }else if(this.porttype == 'out'){
    this.text_color = '#008800';
    this.place = 'e';
    //this.place = 'w';
  }else if(this.porttype == 'corba'){
    this.text_color = '#0088ff';
    this.place = 'e';
    //this.props = datatype.split("#");
    this.props = datatype;
  }else{
    this.text_color ='#0000ff';
    this.place = 'w';
  }

  this.points = this.getShape();
  SvgPolygon.call(this, 0, 0, this.points, Color_deactive);

  this.setAttribute('porttype', this.getPortType());

  this.label = new SvgText(0, 0, this.name, 12, this.text_color);
  this.obj = null;
}

PortObject.prototype = new SvgPolygon();

PortObject.prototype.flipPlace=function(){
  if(this.place == 'w'){
    this.place = 'e';
  }else{
    this.place = 'w';
  }

  this.setAttribute("points", this.getShape(this.place));
}

PortObject.prototype.getShape=function(place){
  if(!place){  place = this.place; }

  if(this.porttype == 'in'){
    if(place == 'w'){
      this.points = '0,0 15,0 15,10 0,10 5,5 0,0';
    }else{
      this.points = '0,0 0,10 15,10 10,5 15,0 0,0';
    }
  }else if(this.porttype == 'out'){
    if(place == 'e'){
      this.points = '0,0 10,0 15,5 10,10 0,10 0,0';
    }else{
      this.points = '0,5 5,0 15,0 15,10 5,10 0,5';
    }
  }else if(this.porttype == 'corba'){
    this.points = '0,0 15,0 15,10 0,10 0,0';
  }else{
    this.points = '0,0 15,0 15,10 0,10 0,0';
  }
  return this.points;
}

PortObject.prototype.getPlace=function(){
  if(this.place == 'e'){
    return 55;
  }else{
    return 0;
  }
}

PortObject.prototype.getPortType=function(){
  var res=null;
  if(this.porttype == 'in'){
    res = 'datainport';
  }else if(this.porttype == 'out'){
    res = 'dataoutport';
  }else if(this.porttype == 'corba'){
    res = 'corbaport';
  }
  return res;
}

PortObject.prototype.placeItem=function(x, y, place){
  if(!place){ place = this.place; }
  this.translate(x, y);
  this. placeItemLabel(x, y, place);
}

PortObject.prototype.placeItemLabel=function(x, y,place){
  this.label.setAttribute("y", y);
  if(place == 'w'){
    this.label.setAttribute("x", x - 5);
    this.label.setAttribute('text-anchor', 'end');

  }else if(place == 'e' ){
    this.label.setAttribute("x", x + 20);
    this.label.setAttribute('text-anchor', 'start');
  }else{
    alert("Error: place '"+ place + "' is not supported");
  }
}

PortObject.prototype.setOwner=function(obj){
  this.owner = obj;
}

PortObject.prototype.updateInfo=function(){

}

PortObject.prototype.pushConnector=function(obj){
  this.connectors.push(obj);
}

PortObject.prototype.removeConnector=function(obj){
  var len = this.connectors.length;
  for(var i=0;i<len;i++){
    if(this.connectors[i] == obj){
      array_remove(this.connectors, obj);
      return;
    }
  }
}

PortObject.prototype.getName=function(){
  return this.owner.getName()+":"+this.name;
}

PortObject.prototype.getFullName=function(){
  return this.owner.getFullName()+":"+this.name;
}

PortObject.prototype.toggleDisplayName=function(){

  if(this.label){
    var clr  =this.label.getAttribute('fill');
    if(clr == 'none'){
      this.label.setAttribute('fill', this.text_color);
    }else{
      this.label.setAttribute('fill', 'none');
      this.text_color=clr;
    }
  }
}

PortObject.prototype.currentPos=function(){
  var pos = this.getObjectPos();
  if (this.porttype == 'in'){
    if(this.place == 'w'){
      pos[0] += 5;
    }else{
      pos[0] += 10;
    }
    pos[1] += 5;
  }else if(this.porttype == 'out'){
    if(this.place == 'e'){
      pos[0] += 15;
      pos[1] += 5;
    }
  }else{
    pos[0] += 5;
    pos[1] += 5;
  }
  return pos;
}

PortObject.prototype.compDataType=function(obj){
  if(this.porttype == 'corba'){
    return this.compServicePort(obj);

  }else{
    var type1 = this.datatype;
    var type2 = obj.datatype;

    if( type1 == type2 ){ return true; }
    if(type1.match(/(IDL:RTC\/)([\w]+)(:1\.0)/)){ type1 = RegExp.$2; }
    if(type2.match(/(IDL:RTC\/)([\w]+)(:1\.0)/)){ type2 = RegExp.$2; }

    if( type1 == type2 ){ return true; }

    return false;
  }
}

PortObject.prototype.exit=function(){
  while( this.connectors.length > 0){
    var conn = this.connectors.pop();
    removeConnectorFromList(conn);
  }
}

PortObject.prototype.isConnectablePort=function(obj){
  var ttype = this.getConnectableType();

  if(ttype == obj.getPortType()){
    if(ttype == 'corbaport'){
      if(this.compServicePort(obj)){ return true; }
    }else{
      if(this.compDataType(obj)){ return true; }
    }
  }
  return false;
}

PortObject.prototype.getConnectableType=function(){
  var type = this.getPortType();
  if(type == 'datainport') { return 'dataoutport'; }
  if(type == 'dataoutport') { return 'datainport'; }
  if(type == 'corbaport') { return 'corbaport'; }
  return null;
}

PortObject.prototype.setColor=function(c){
   this.setAttribute('fill', c);
}

PortObject.prototype.updateConnectors=function(){
  for(var k=0;k<this.connectors.length;k++){
     this.connectors[k].updatePos();
  }
}

PortObject.prototype.compServicePort=function(port){
  var props = port.props;

  for(var i=0;i<this.props.length;i++){
    var plist1 = this.props[i];
    for(var j=0;j<port.props.length;j++){
      var plist2 = port.props[j];
      if(plist1.InstanceName == plist2.InstanceName &&
           plist1.TypeName == plist2.TypeName &&
           plist1.Polarity != plist2.Polarity ){
        return true;
      }
    }
    
  }
  return false;
}

PortObject.prototype.updateColor=function(){
  if (this.connectors.length  > 0 || this.obj.Connections.length > 0){

    var type = this.getPortType();

    if(type == 'corbaport') {
      this.setColor( Color_conenctedCorbaport);
    }else{
      this.setColor( Color_conenctedDataport );
    }

  }else{
    this.setColor(Color_noconnection);
  }

}

PortObject.prototype.showConnections=function(){
  for(var k=0;k<this.connectors.length;k++){
     this.connectors[k].show();
  }
}

PortObject.prototype.hideConnections=function(){
  for(var k=0;k<this.connectors.length;k++){
     this.connectors[k].hide();
  }
}

///////////////

var RtObject = function(name, path){
  this.name=name;
  this.path=path;

  this.datainport = new Array();
  this.dataoutport = new Array();
  this.serviceport = new Array();

  this.label=null;
  this.display='none';
  this.body=null;

  this.east_ports = new Array();
  this.west_ports = new Array();
  this.ports = new Array();
  this.obj=null;

  this.dir="RIGHT";

  Rtc_table.push(this); 
  SvgGroup.call(this, null);

}

RtObject.prototype = new SvgGroup();

RtObject.prototype.getFullName=function(){
  return this.path + this.name;
}

RtObject.prototype.getName=function(){
  return this.name;
}

RtObject.prototype.getId=function(){
  var id=null;
  if(this.obj){
    id = "RTC:"+this.obj["Vendor"]+":"+this.obj["Category"]+":"+this.obj["TypeName"]+":"+this.obj.ExtraProperties["version"];
  }else{
    alert("Object not found");
  }
  return id;
}

RtObject.prototype.addPort=function(name, type, datatype){
  var port = new PortObject(name, type, datatype);

  if(type == 'in'){
    this.datainport.push(port);
  }else if(type == 'out'){
    this.dataoutport.push(port);
  }else if(type == 'corba'){
    this.serviceport.push(port);
  }

  if(port.place == 'w'){
    this.west_ports.push(port);
  }else{
    this.east_ports.push(port);
  }

  this.ports.push(port);
  port.setOwner(this);
  return port;
}

RtObject.prototype.getPorts=function(){
  return new Array(this.datainport, this.dataoutport, this.serviceport);
}

RtObject.prototype.getAllPorts=function(){
  return this.ports;
}

RtObject.prototype.updatePortColor=function(){
  var ports = this.getAllPorts();

  for(var i=0;i<ports.length;i++){
    ports[i].updateColor();
  }
}

RtObject.prototype.findPort=function(name){
  for(var i=0; i<this.ports.length;i++){
    if(this.ports[i].name == name){
      return this.ports[i];
    }
  }
  return null;
}

RtObject.prototype.toggleDisplayName=function(){
  var i;
  for(var i=0; i<this.ports.length;i++){
    this.ports[i].toggleDisplayName();
  }
}

RtObject.prototype.deleteItem=function(){
  getSvgCanvas().removeSvgObject(this);
//  this.setAttribute('display','none');
  this.display='none';

  for(var i=0; i<this.ports.length;i++){
    this.ports[i].hideConnections();
  }

}

RtObject.prototype.showItem=function(){
  getSvgCanvas().appendSvgObject(this);
  this.setAttribute('display','block');
  this.display='block';

  for(var i=0; i<this.ports.length;i++){
    this.ports[i].showConnections();
  }
}

RtObject.prototype.is_display=function(){
  return (this.display == 'block');
}

RtObject.prototype.exit=function(){
  var i;
  getSvgCanvas().removeSvgObject(this);

  for(i=0;i<this.ports.length;i++){
    this.ports[i].exit();
  }
}

RtObject.prototype.setColor=function(color){
  this.body.setAttribute('fill', color);
}

RtObject.prototype.setStatus=function(st){
  if(st == 'Active'){
    this.setColor(Color_active);
  }else if(st == 'Error'){
    this.setColor(Color_error);
  }else {
    this.setColor(Color_deactive);
  }
}

RtObject.prototype.createPortObjects=function(dataports){
  if(dataports){
    for(var i=0; i<dataports.length;i++){
      var p = dataports[i];
      var portname = p.Name;
      var porttype = p.Type;

      if(porttype == 'DataOutPort'){
        var portdatatype = p.Properties['dataport.data_type'];
        port_obj = this.addPort(portname, 'out', portdatatype);
        port_obj.obj = p;
      }else if(porttype == 'DataInPort'){
        var portdatatype = p.Properties['dataport.data_type'];
        port_obj = this.addPort(portname, 'in', portdatatype);
        port_obj.obj = p;
      }else if(porttype == 'CorbaPort' ){
        var portdatatype = p.Interfaces;
        port_obj = this.addPort(portname, 'corba', portdatatype);
        port_obj.obj = p;
      }
    }
  }

  return [this.west_ports, this.east_ports];
}

RtObject.prototype.placePortObjects=function(x, y, flag){
  var i;

  for(i in this.west_ports){
    var port_obj = this.west_ports[i];
    var dp = this.getPortPlace('w', i);
    port_obj.placeItem(x+dp[0], y+dp[1]);
    port_obj.updateConnectors();

    if(flag){
      this.addSvgObject(port_obj);
      this.addSvgObject(port_obj.label);
    }
  }

  for(i in this.east_ports){
    var port_obj = this.east_ports[i];
    var dp = this.getPortPlace('e', i);
    port_obj.placeItem(x+dp[0], y+dp[1]);
    port_obj.updateConnectors();

    if(flag){
      this.addSvgObject(port_obj);
      this.addSvgObject(port_obj.label);
    }
  }
}

RtObject.prototype.parsePortInfo=function(dataports){
  var ar_out = new Array();
  var ar_in = new Array();
  var ar_c = new Array();
  var ar_outdata = new Array();
  var ar_indata = new Array();
  var n_out = 0, n_in = 0, n_c = 0;
  var ports = dataports.split(',');

  for(i in ports){ /// check number of dataports 
    var portname = null;
    var porttype = null;
    var portdatatype = null;;

    if(ports[i].match(/(\w+)\((\w+);([\w:\/\.]*)\)/)){
      portname = RegExp.$1;
      porttype = RegExp.$2;
      portdatatype = RegExp.$3;
    }

    if(porttype == 'DataOutPort'){
      ar_out[n_out] = portname;
      ar_outdata[n_out] = portdatatype;
      n_out += 1;
    }else if(porttype == 'DataInPort'){
      ar_in[n_in] = portname;
      ar_indata[n_in] = portdatatype;
      n_in += 1;
    }else if(porttype == 'CorbaPort' ){
      ar_c[n_c] = portname;
      n_c += 1;
    }
  }
  return [ar_in, ar_out, ar_c, ar_indata, ar_outdata];
}

RtObject.prototype.getPortPlace=function(dir, idx){
  var margin = 10;  
  var padding = 15;  
  var padding_x_e = 55;  

  var xx = 0;
  var yy = margin + idx*padding;

  if(dir == 'e'){ xx += padding_x_e; }
  return [xx, yy];
}

RtObject.prototype.getBodySize=function(n_ports_w, n_ports_e){
  var margin = 10 + 5;  
  var width = 50;

  if(!n_ports_w){ n_ports_w = this.west_ports.length; }
  if(!n_ports_e){ n_ports_e = this.east_ports.length; }

  var height = margin + Math.max(n_ports_w, n_ports_e) * 15;

  return [width, height];
}

RtObject.prototype.getCurrentPos=function(){
  var pos = [0,0];

  if(this.body){
    pos[0] = this.body.getAttributeVal('x')-10;
    pos[1] = this.body.getAttributeVal('y');
  }
  return pos;
}

RtObject.prototype.refreshPortPos=function(){
  var all_ports = this.west_ports.concat(this.east_ports);
  this.west_ports = new Array();
  this.east_ports = new Array();

  for(var i=0; i<all_ports.length;i++){
    var obj = all_ports[i];
    if(obj.place == 'w'){
      this.west_ports.push(obj);
    }else{
      this.east_ports.push(obj);
    } 
  }
}

RtObject.prototype.flipPorts=function(){
  var all_ports = this.west_ports.concat(this.east_ports);
  this.west_ports = new Array();
  this.east_ports = new Array();

  if(this.dir == "RIGHT"){ this.dir = "LEFT"; }
  else{ this.dir="RIGHT"; }

  for(var i=0; i<all_ports.length;i++){
    var obj = all_ports[i];
    obj.flipPlace();

    if(obj.place == 'w'){
      this.west_ports.push(obj);
    }else{
      this.east_ports.push(obj);
    } 
  }

  var pos = this.getCurrentPos();
  this.placePortObjects(pos[0], pos[1], false);
}

RtObject.prototype.createSvgObjects=function(x, y, dataports){
  if(this.members.length == 0){
    var data;

    this.createPortObjects(dataports);
    var w_h = this.getBodySize();
    var width = w_h[0];
    var height = w_h[1];

    this.body = new SvgRect(x+10, y, width, height, Color_deactive);
    this.body.element.setAttribute('type','body');
    this.addSvgObject(this.body);

    this.placePortObjects(x, y, true);

    if(this.name){
      this.label = new SvgText(x+10+width/2,y+height+10,this.name,12,'#0000ff');
      this.label.setAttribute("text-anchor", "middle");
      this.addSvgObject(this.label);
    }

    this.setAttribute('Type', 'RTC');
    this.display = 'block';
  }
 
  return this;
}


RtObject.prototype.setSelectBox=function(){
  this.body.setSelectBox();
}

RtObject.prototype.showSelectBox=function(){
  this.body.showSelectBox();
  this.setSelected(true);
}

RtObject.prototype.hideSelectBox=function(){
  this.body.hideSelectBox();
  this.setSelected(false);
}

RtObject.prototype.checkPort=function(x1, y1, x2, y2){
  var ports = this.getAllPorts();

  for(var i=0; i<ports.length; i++){
    var port = ports[i];

    if(port && port.checkIntersection(x1, y1, x2, y2)){
      return port;
    }
  }
  return null;
}

RtObject.prototype.checkIntersection=function(x1, y1, x2, y2){
  return SvgObject.prototype.checkIntersection.call(this, x1, y1, x2, y2);
}

RtObject.prototype.onMouseDown=function(e, x1, y1, x2, y2, flag){

  if(this.checkIntersection(x1, y1, x2, y2)){

    var port = this.checkPort(x1, y1, x2, y2);

    if(port){
      array_remove(selectedItems, this);
      this.hideSelectBox();
      selectedPort=port;

    }else{
      if(!this.isSelected()){
        if(this.body.checkIntersection(x1, y1, x2, y2)){
          this.showSelectBox();
          selectedItems.push(this);
        }
      }
      selectedPort=null;
    }
  }else if(flag && this.isSelected()){
    array_remove(selectedItems, this);

    this.hideSelectBox();
    selectedPort=null;
  }else{
  }
}

RtObject.prototype.moveConnector=function(e, dx, dy){
  var ports = null;
  portlist = this.getPorts();

  for(var i=0; i< portlist.length;i++){
    ports = portlist[i];

    for(var j=0; j< ports.length;j++){
      for(var k=0;k<ports[j].connectors.length;k++){
        var conn = ports[j].connectors[k];
        var direction = conn.getDirection(ports[j]);

        if(direction){
          conn.moveConnector(direction, dx, dy);
        }
      }
    }
  }
}

RtObject.prototype.moveEndConnector=function(e){
  portlist = this.getPorts();

  for(var i=0; i< portlist.length;i++){
    ports = portlist[i];
    for(var j=0; j< ports.length;j++){
      ports[j].updateConnectors();
/*
      for(var k=0;k<ports[j].connectors.length;k++){
        ports[j].connectors[k].updatePos();
      }
*/
    }
  }
}

RtObject.prototype.onMouseMove=function(e, dx, dy, x, y){
  SvgObject.prototype.onMouseMove.call(this ,e, dx, dy, x, y);
  this.moveConnector(e, dx, dy);
}

RtObject.prototype.onMouseUp=function(e){
  SvgObject.prototype.onMouseUp.call(this ,e);

  ele = findCompItem(this.path, this.name);
  if(ele){
    setSelectedItem(ele);
  }

  this.moveEndConnector(e);
}

