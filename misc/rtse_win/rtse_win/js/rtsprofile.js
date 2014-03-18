/*
 * RT System Editor on the Web

*/

//// for FileSupport  ///////////////////
var fileApiSupported = 0;

function checkFileApi(){
  if (window.File && window.FileReader && window.FileList && window.Blob) {
    fileApiSupported = 1;
  } else {
    alert('The File APIs are not fully supported in this browser.');
  }
}

function loadedRtcProfile(txt)
{
  var parser = new DOMParser();
  var xmldoc = parser.parseFromString(txt, "text/xml");
  var val = "";

  var nodes = xmldoc.getElementsByTagName("rts:RtsProfile");

  for(var i=0; i<nodes[0].attributes.length; i++){
    val += nodes[0].attributes[i].name +":"+  nodes[0].attributes[i].value +"<br>";
  }
  var span = document.createElement('span');
  span.innerHTML = "<textarea>"+ txt +"</textarea>";
  document.getElementById('list').insertBefore(span, null);

}


function handleFileSelect(evt) {
  var files = evt.target.files; // FileList object

  // Loop through the FileList and render image files as thumbnails.
  for (var i = 0, f; f = files[i]; i++) {
    // Only process image files.
    console.log(f.type);
    if (!f.type.match('text/*')) {
      continue;
    }

    var reader = new FileReader();

    // Closure to capture the file information.
    reader.onload = (function(theFile) {
      return function(e) {  loadedRtcProfile(e.target.result); };
    })(f);

    // Read in the image file as a data URL.
    reader.readAsText(f,'utf8');
  }
}


function openRtsProfileWindow(){
  rtsp_window = window.open("rtsprofile.html", "RtsProfile", "width=600,height=600,scrollbars=1");
  rtsp_window.focus();
}

////////// RtsProfile
var RtsComponentProfile=function(comp)
{
  this.obj = comp;
  this.compType="None";
  this.instanceName=comp.obj["InstanceName"];
  this.id=comp.getId();
  this.path=comp.getFullName();
}

RtsComponentProfile.prototype.port_toXML=function(port)
{
   var res ="";
   var type = port.Type;

   if(type == 'DataInPort' || type == 'DataOutPort'){
      res += "  <rts:DataPorts xsi:type=\"rtsExt:dataport_ext\" ";
      res += " rts:name=\""+port["Name"]+"\">\n";

      var keys = ["port.port_type", "dataport.data_type", "dataport.subscription_type", "dataport.dataflow_type", "dataport.interface_type"];
      for(var i=0;i<keys.length;i++){
        var propname = keys[i];
        res += "    <rtsExt:Propertirs rtsExt:name=\""+propname+"\" rtsExt:value=\""+port["Properties"][propname]+"\"/>\n";
      }

      res += "  </rts:DataPorts>\n";

   }else if(type == 'CorbaPort'){
      res += "  <rts:ServicePorts xsi:type=\"rtsExt:serviceport_ext\" ";
      res += " rts:name=\""+port.name+"\">\n";

      var keys = ["port.port_type"];
      for(var i=0;i<keys.length;i++){
        var propname = keys[i];
        res += "    <rtsExt:Propertirs rtsExt:name=\""+propname+"\" rtsExt:value=\""+port["Properties"][propname]+"\"/>\n";
      }


      res += "  </rts:ServicePorts>\n";
   }
   return res;
}

RtsComponentProfile.prototype.toXML=function()
{
  var configSet="";
  var comp = this.obj;
  if(comp.obj.ActiveConfig){
    configSet=" rts:activeConfigurationSet=\""+comp.obj.ActiveConfig+"\"" ;
  }
  var result = "\n<rts:Components xsi:type=\"rtsExt:component_ext\" rts:isRequired=\"true\" ";
  result += "rts:compositeType=\""+this.compType;
  result += "\" rts:instanceName=\""+this.instanceName;
  result += "\" rts:pathUri=\""+this.path;
  result += "\" rts:id=\""+this.id+"\""
  result += configSet+">\n";

  //// Ports
  for(var i=0; i<comp.obj.Ports.length; i++){
    var port = comp.obj.Ports[i];
    result += this.port_toXML(port); 
  }

  //// ConfigurationSets
  var ext_props = comp.obj["ExtraProperties"];
  for(var j=0; j<comp.obj.ConfigSet.length; j++){
    var confname = comp.obj.ConfigSet[j];
    var params = getConfigParams(ext_props, confname);
    result += "  <rts:ConfigrationSets rts:id=\""+confname+"\">\n";
    for(var i=0; i<comp.obj.ConfigParams.length; i++){
      var param = comp.obj.ConfigParams[i];
      var paramname = "conf."+confname+"."+param;
      result += "    <rts:ConfigrationData rts:name=\""+param+"\" rts:data=\""+params[paramname]+"\"/>\n";
    }
    result += "  </rts:ConfigrationSets>\n";
  }
  //// ExecutionContext
  var ecs = comp.obj.ExecutionContext;
  for(var i in ecs){
    var ec = ecs[i];
    result += "  <rts:ExecutionContexts rts:id=\""+ec.Handle+"\" xsi:type=\"rtsExt:execution_context_ext\" ";
    result += " rts:kind=\""+ec.Kind+"\" ";
    result += " rts:rate=\""+ec.Rate+"\" ";
    result += "/>\n";

  }
  
  //// Participants
  
  //// Location
  var x, y0;
  [x, y] = comp.getCurrentPos();
  result += "  <rtsExt:Locationn rtsExt:x=\""+x+"\" rtsExt:y=\""+y+"\" rtsExt:height=\"-1\" rtsExt:width=\"-1\" rtsExt:direction=\""+comp.dir+"\"/>\n";
  
  //// ExtraProperties
  var keys = [];
  for (var x in ext_props){ keys.push(x);}
  keys.sort();
  for(var i in keys){
    var name = keys[i];
    var val = ext_props[name];
    result += "  <rtsExt:Properties rtsExt:name=\""+name+"\" rtsExt:value=\""+val+"\"/>\n";
  }
  
  result +="</rts:Components>\n";
  return result;

}

var RtsProfile=function(vendor, sysname, version){
  this.header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" ;

  this.id = "RTSystem:"+vendor+"."+sysname+":"+ version;
  this.version = version;
  this.creationDate=new Date();
  this.updateDate=new Date();
  this.Abstract = "Created by RTSE on the Web";

  this.Components=[];
  this.Groups = [];
  this.DataPortConnectors=[];
  this.ServicePortConnectors=[];
  this.StartUp=null;
  this.Shutdown=null;
  this.Activation=null;
  this.Deactivation=null;
  this.Resetting=null;
  this.Initializating=null;
  this.Finalizing=null;
  this.commnet=null;
  this.VersionLog=[];
  this.Properties=[];
}

RtsProfile.prototype.formatDate=function(d){
  yy = d.getYear();
  mm = d.getMonth() + 1;
  dd = d.getDate();
  hh = d.getHours();
  jj = d.getMinutes();
  ss = d.getSeconds();
  if (yy < 2000) { yy += 1900; }
  if (mm < 10) { mm = "0" + mm; }
  if (dd < 10) { dd = "0" + dd; }
  if (hh < 10) { hh = "0" + hh; }
  if (jj < 10) { jj = "0" + jj; }
  if (ss < 10) { ss = "0" + ss; }

  return  yy+"-"+mm+"-"+dd+"T"+hh+":"+jj+":"+ss;
}

RtsProfile.prototype.mkRootNode=function(){
  var nodestr = "<rts:RtsProfile rts:id=\""+this.id;
  nodestr += "\" rts:version=\""+this.version;
  nodestr += "\" rts:updateDate=\""+this.formatDate(this.creationDate);
  nodestr += "\" rts:creationDate=\""+this.formatDate(this.updateDate);
  nodestr += "\" rts:abstract=\""+this.Abstract;
  nodestr += "\" xmlns:rtsExt=\"http://www.openrtp.org/namespaces/rts_ext\" xmlns:rts=\"http://www.openrtp.org/namespaces/rts\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";

  return this.header +"\n" + nodestr+"\n";
}

RtsProfile.prototype.pushComponent=function(comp){
  this.Components.push(new RtsComponentProfile(comp));
}

RtsProfile.prototype.toXML=function(){
  var result = this.mkRootNode();

  /// Components
  for(var i=0;i<this.Components.length;i++){
    var comp = this.Components[i];
    result += comp.toXML();
  }

  // Groups
  
  // DataPortConnectors
  // ServicePortConnetors
  for(var i in connectorList){
    var conn = connectorList[i];
     //console.log(conn);
     console.log(conn.start_obj);

  }
  
  result +="</rts:RtsProfile>";
  return result;
}

function createRtsProfile(){
  profile = new  RtsProfile("AIST", "Sample", "0.1");
  var rtcs = getDisplayedRtcs();
  for(var i=0;i<rtcs.length;i++){
    var comp = rtcs[i];
    profile.pushComponent(comp);
  }
  var txt = profile.toXML();
  var ele = document.getElementById('profile');
  ele.innerHTML="<textarea cols=\"100\" rows=\"20\">"+txt+"</textarea>";
}
/////////////////////
checkFileApi();
