/*
  canvas.js

  SVG Graphic Library

  Copyright(c) 2010-2013, Isao Hara, haraisao@gmail.com

  All rights reserved. This program is made available under the terms of the
  Eclipse Public License v1.0 which accompanies this distribution, and is
  available at http://www.eclipse.org/legal/epl-v10.html

  Contributors:  Isao Hara.

 */
var selectedItems=new Array();

var nextId=1;
var Canvas=null;

var SvgObjectList = new Array();

////// SVG
var editingTextObj = null;

//////////////////////////////
var useragent = '';

/*
 *
 *
 */
function array_shift(ar, x){
  var tmp = new Array();
  var len = ar.length;
  var res = null;
  for(var i=0; i <len; i++){
    if(i==x){
      res = ar.shift();
      break;
    }else{
      tmp.push(ar.shift());
    }
  }
  len = tmp.length;
  for(var i=0; i<len;i++){
    ar.unshift(tmp.pop());
  }
  return res;
}

function array_remove(ar, x){
  var tmp = new Array();
  var len = ar.length;
  var res = null;
  for(var i=0; i <len; i++){
    if(ar[0]==x){
      res = ar.shift();
      break;
    }else{
      tmp.push(ar.shift());
    }
  }
  len = tmp.length;
  for(var i=0; i<len;i++){
    ar.unshift(tmp.pop());
  }
  return res;
}

function restruct_svg_object_list(id){
  var canvas = getSvgCanvas(id);
  SvgObjectList = new Array();
  for(var i=0;i< canvas.svg_top.childNodes.length; i++){
    var obj = canvas.svg_top.childNodes.item(i).svgobj;
    if(obj){
      SvgObjectList.push(obj);
    }
  }
  return SvgObjectList;
}

function updateAllSvgObjects(){
  for(var i in SvgObjectList){
    SvgObjectList[i].updateTransform();
  }
}
/*
 *  Check UserAgent
 *
 */
function getBrowserInfo()
{
  var userAgent = window.navigator.userAgent.toLowerCase();
  var appVersion = window.navigator.appVersion.toLowerCase();

  if (userAgent.indexOf("msie") > -1) {
    if (appVersion.indexOf("msie 6.0") > -1) { return "IE6"; }
    else if (appVersion.indexOf("msie 7.0") > -1) { return "IE7"; }
    else if (appVersion.indexOf("msie 8.0") > -1) { return "IE8"; }
    else if (appVersion.indexOf("msie 9.0") > -1) { return "IE9"; }
    else if (appVersion.indexOf("msie 10.0") > -1) { return "IE10"; }
    else{ return "UnknownIE"; }
  }
  else if (userAgent.indexOf("firefox") > -1) { return "Firefox"; }
  else if (userAgent.indexOf("opera") > -1) { return "Opera"; }
  else if (userAgent.indexOf("chrome") > -1) { return "Google Chrome"; }
  else if (userAgent.indexOf("safari") > -1) { return "Safari"; }
  else { return "Unknown"; }
}


function newID(){
  var id = 'svg_' + nextId;
  nextId++;
  return id;
}

function getSvgCanvas(svg_top_id){
  if(!svg_top_id) { svg_top_id = 'svg_top'; }
  var ele = document.getElementById(svg_top_id);
  if(ele && ele.canvas){ return ele.canvas; }
  //alert("No canvas found.");
  return null;
}

//////  
function hideItemById(id){
  var itm = document.getElementById(id);
  hideItem(itm);
}

function hideItem(itm){
  if(itm){ itm.style.display='none'; }
}

function showItemById(id){
  var itm = document.getElementById(id);
  showItem(itm);
}

function showItem(id){
  if(itm){ itm.style.display='block'; }
}

function removeChildNodesById(id){
  var itm = document.getElementById(id);
  removeChildNodes(itm);
}

function removeChildNodes(itm){
  if(!itm) return;

  var child = itm.firstChild;

  while(child){
    itm.removeChild(child);
    child = itm.firstChild;
  }
}

/////////////////////////////////

function setAttributes(obj, attrs){
  var attr_array = attrs.split(',');
  if(!obj) { return; }

  for (var i=0; i<attr_array.length;i++){
    var x = attr_array[i].split('=');
    if(x.length == 2){
      obj.setAttribute(x[0], x[1]);
    }
  }
}

function addAttributeVal(obj, itm, dv){
  SvgObject.prototype.addAttributeVal.call(obj, itm, dv);
}

function getAttributeVal(obj, name){
  return SvgObject.prototype.getAttributeVal.call(obj, name);
}


function getElementTag(obj){
  if (!obj){ return null; }
  if(!obj.tagName){ return "NoName"; }
  var tag = obj.tagName.split(':');
  if(tag.length == 2 && tag[0]=='svg'){ return tag[1]; }
  return tag[0];
}

//////////////////////////////////////////////////
// Create Primitive Object

function InfoOut(e)
{
  var canvas = getSvgCanvas();
  canvas.info_text.textContent = "Page(" + e.pageX +", "+ e.pageY +")";
  v = getPageOffset(canvas.canvas);
  x = parseInt(e.clientX) - parseInt(v[0]);
  y = parseInt(e.clientY) - parseInt(v[1]);
  canvas.info_text.textContent += "  (" + x +", "+ y +")";
}

//////////////////////////////////////////////////
function getPageOffset(obj){
  var x = obj.offsetLeft;
  var y = obj.offsetTop;

  while (obj.offsetParent != null){
    obj = obj.offsetParent;
    x += obj.offsetLeft;
    y += obj.offsetTop;
  }

  return [x, y];
}

////////////// Arrow
function setLeftArrow(){
 if( selectedItems.length == 1 ){
    setArrow(selectedItems[0], 'start', '');
 }
}

function setRightArrow(){
 if( selectedItems.length == 1 ){
    setArrow(selectedItems[0], 'end', '');
 }
}

function removeLeftArrow(){
 var svg_defs = document.getElementById('svg_defs');
 if(svg_defs && selectedItems.length == 1 ){
   try{
     var marker = getArrowMarker(selectedItems[0],'start');
     svg_defs.removeChild(marker);
     selectedItems[0].removeAttribute('marker-start');
   }catch(e){ }
 }

}

function removeRightArrow(){
 var svg_defs = document.getElementById('svg_defs');
 if(svg_defs &&  selectedItems.length == 1 ){
   try{
     var marker = getArrowMarker(selectedItems[0],'end');
     svg_defs.removeChild(marker);
     selectedItems[0].removeAttribute('marker-end');
   }catch(e){ }
 }
}

function removeAllMarker(obj){
  var svg_defs = document.getElementById('svg_defs');
  if(!svg_defs) { return false; }

  var smarker = getArrowMarker(obj, 'start');
  if(smarker){ svg_defs.removeChild(smarker); }
  var emarker = getArrowMarker(obj, 'end');
  if(emarker){ svg_defs.removeChild(emarker); }

  return true;
}

function hasArrow(pos){
  var pobj = selectedItems[0];
  if(!pobj){ return false; }
  if(getArrowMarker(pobj,pos)){ return true; }
  return false;
}

function getArrowMarker(obj, pos){
  SvgObject.prototype.getArrowMarker.call(obj, pos);
}

function setArrow(pobj, pos, type){
  if(!pobj){ return; }
  SvgObject.prototype.setArrow.call(pobj, pos);
}

/////////////////////////////////
function isChildById(element, id) {
  if(element == null ||
     element.parentNode == null ||
     element.parentNode.nodeName=='BODY'){ return false; }
  else if (element.parentNode.id == id){ return true; }
  else { return isChildById(element.parentNode, id); }
}

function escapeHTML(text){
  return text.replace( /[<>"&]/g,
    function (m) { return { '<': '&lt;', '>': '&gt;', '"': '&quot;', '&': '&amp;' }[m]; }
  );
};

function trim(str){
  return str.replace(/(^\s+)|(\s+$)/g, "");
}

function jstrlen(str) {
  var len = 0;
  str = escape(str);

  for (var i = 0; i < str.length; i++, len++) {
    if (str.charAt(i) == "%") {
      if (str.charAt(++i) == "u") {
        i += 3;
        len++;
      }
      i++;
    }
  }
  return len;
}

////////// 
function toggleRotateScaleLock(){
  var canvas = getSvgCanvas(); 
  canvas.toggleRotateScaleLock();
}

function setRotLock(){
  var canvas = getSvgCanvas(); 
  canvas.setRotLock();
}

function setScaleLock(){
  var canvas = getSvgCanvas(); 
  canvas.setScaleLock();
}

///// 
function calcDist(x1, y1, x2, y2){
 return (x1-x2)*(x1-x2) +(y1-y2)*(y1-y2);
}

function isIncludeEllipse( x1, y1, cx, cy, rx, ry){
 return ((x1-cx)*(x1-cx)/rx/rx +(y1-cy)*(y1-cy)/ry/ry) < 1;
}

function checkIntersection(obj, x, y, ex, ey){
  if(!obj){ return false; }

  var res;
  if(obj.svgobj){
    if( obj.svgobj.checkIntersection){
      return obj.svgobj.checkIntersection(x, y, ex, ey);
    }
  }else{
    return SvgObject.prototype.checkIntersection.call(obj, x, y, ex, ey);
  }
}

function getBoundingBox(obj){
 if(obj.svgobj){
   return obj.svgobj.getBoundingBox();
// }else{
//   return SvgObject.prototype.getBoundingBox.call(obj);
 }
}

////////////
function dupObject(){
  var canvas = getSvgCanvas(); 
  if(selectedItems.length == 0){ return; }
  canvas.dupItems = selectedItems;
  canvas.dupX = parseInt(canvas.svg_select.getAttribute("x"));
  canvas.dupY = parseInt(canvas.svg_select.getAttribute("y"));

  setSvgMode('Duplicate');
}

function pasteObject(x,y){
  var canvas = getSvgCanvas(); 
  if(canvas.dupItems.length == 0){ return; }

  for(var i=0;i<canvas.dupItems.length;i++){
    var obj = canvas.dupItems[i].svgobj.clone();

    obj.translate(x - canvas.dupX, y - canvas.dupY);
    canvas.appendSvgObject(obj);
  }
}

//////////////////////////////////////////////////
function getObjectPos(obj)
{
  var res;
  var x1,y1;

  if(obj.element){
     return obj.getObjectPos();
  }else{
    switch(getElementTag(obj)){
      case 'g':
        var objs = obj.childNodes;
        res = getObjectPos(objs[0]);
        break;
      default:
        if(obj.svgobj){
          res = obj.svgobj.getObjectPos();
        }else{
          console.log("Fail to getObjectPos");
          res = new Array(0,0);
        }
        break;
    }
    return res;
  }
}

function setObjectPos(obj, x, y){
  if(obj.svgobj){
    res = obj.svgobj.setObjectPos(x, y);
  }else{
    if(getElementTag(obj) == 'g'){
      SvgGroup.prototype.setObjectPos.call(obj, x, y);
    }else{
      SvgObject.prototype.setObjectPos.call(obj, x, y);
    }
  }
}

function translateObject(obj, dx, dy){
  switch(getElementTag(obj)){
    case 'g':
      var objs = obj.childNodes;
      for(var i=0;i<objs.length;i++){ translateObject(objs[i], dx, dy); }
      break;

    default:
      if(obj.svgobj){
        obj.svgobj.translate(dx,dy);
      }else{
        SvgObject.prototype.translate.call(obj, dx, dy);
      }
      break;
  }
}

function updateTransform(obj){ 
  if(obj.svgobj){
    obj.svgobj.updateTransform();
  }else{
    SvgObject.prototype.updateTransform.call(obj);
  }
}

function replaceTranslate(obj, dx, dy){
  if(obj.svgobj){
    obj.svgobj.replaceTranslate(dx, dy);
  }else{
    SvgObject.prototype.replaceTranslate.call(obj, dx, dy);
  }
}

function getRotateAngle(obj){
  if(obj.svgobj){
    return obj.svgobj.getRotateAngle();
  }else{
    return SvgObject.prototype.getRotateAngle.call(obj);
  }
}

function getTranslateValue(obj){

  if(obj.svgobj){
    return obj.svgobj.getTrasnlationValue();
  }else{
    return SvgObject.prototype.setTranslateValue.call(obj);
  }
  return null;
}

function updateRotate(obj,dx,dy){
  if(obj.svgobj){
    obj.svgobj.updateRotate(dx, dy);
  }else{
    SvgObject.prototype.updateRotate.call(obj, dx, dy);
  }
}

function getRotateAngle(obj){
  if(obj.svgobj){
    return obj.svgobj.getRotateAngle();
  }else{
    return SvgObject.prototype.getRotateAngle.call(obj);
  }
  return 0;
}

function setRotate(obj,deg){
  if(obj.svgobj){
    obj.svgobj.setRotate(deg);
  }else{
    SvgObject.prototype.setRotate.call(obj, deg);
  }
}

function getScale(obj){
  if(obj.svgobj){
    return obj.svgobj.getScale();
  }else{
    return SvgObject.prototype.getScale.call(obj);
  }
  return null;
}

function setScale(obj,scaleX, scaleY){
  if(obj.svgobj){
    obj.svgobj.setScale(scaleX, scaleY);
  }else{
    SvgObject.prototype.setScale.call(obj, scaleX, scaleY);
  }
}

////////////////////////////////////////////////////////////
function scaleSvgElement(obj, scale){
  if(obj.svgobj){
    if(obj.svgobj.constructor == SvgLine || 
      obj.svgobj.constructor == SvgPath ){
      obj.svgobj.scale(scale);
    }
  }else{
    var tag = getElementTag(obj);
    if(tag == 'path'){
      SvgPath.prototype.scale.call(obj, scale);
    }else if(tag == 'line'){
      SvgLine.prototype.scale.call(obj, scale);
    }
  }
}

function updatePathAndLine(){
  if(selectedItems.length == 0){ return; }
  var itm = selectedItems[0];
  var tag = getElementTag(itm);
  if(tag == 'path' || tag == 'line'){
    var scale = getScale(itm);
    if(scale){
      setScale(itm, 1, 1);
      scaleSvgElement(itm, scale);
    }
  }
}

///////  Text Input
function svgInputTextExec(e){
  var canvas = getSvgCanvas(); 
  var inputform = document.getElementById('svg_input');
  var color = document.getElementById('svg_color');
  var size = document.getElementById('svg_size');

  if(!inputform || !color || !size){  alert("Fail to input a text"); return; }

  var y = parseInt(inputform.style.top) + parseInt(size.value)*0.8 + 12;
  var x = parseInt(inputform.style.left) + 12;

  if(inputform.value){
    var obj = new SvgText(x, y, inputform.value, size.value, color.value);
    canvas.appendSvgObject(obj);
    selectedItems = [ obj.element ];
  }

  inputform.setAttribute("type", "hidden");
  setSvgMode('selector');
}

function svgModifyTextExec(e){
  var inputform = document.getElementById('svg_input');

  if(!inputform || !editingTextObj){ alert("Fail to modify the text"); return; }

  editingTextObj.textContent = inputform.value;
  inputform.setAttribute("type", "hidden");
  editingTextObj.style.display='block';

  editingTextObj=null;
  setSvgMode('selector');
}

function svgInputFormAdjust(e){
  var inputform = document.getElementById('svg_input');
  if(!inputform){ alert("No inputForm"); return; }
  inputform.size = jstrlen(inputform.value) + 1;
}

function addTextInputEvent(form, id){
  if(document.addEventListner){
    if(id){
      form.addEventListener("onChange", svgModifyTextExec,false);
    }else{
      form.addEventListener("onChange", svgInputTextExec,false);
    }
  }else{
    if(id){
      form.setAttribute("onChange", "svgModifyTextExec()");
    }else{
      form.setAttribute("onChange", "svgInputTextExec()");
    }
  }

}

function putInputForm(x, y, txt, size, id){
  var inputform = document.getElementById('svg_input');
  var canvas = getSvgCanvas(); 

  if(!inputform){ /// creaet input form
    inputform = document.createElement('input');
    inputform.setAttribute("id", "svg_input");
    inputform.setAttribute("style", "position:absolute;top:0px;left:0px; border:0px none");
    canvas.canvas.appendChild(inputform);

    if(document.addEventListner){
      inputform.addEventListener("onkeydown", svgInputFormAdjust,false);
    }else{
      inputform.setAttribute("onkeydown", "svgInputFormAdjust()");
    }
  }

  addTextInputEvent(inputform, id);

  inputform.setAttribute("type", "text");
  inputform.style.left = x+'px';
  inputform.style.top = y+'px';
  inputform.style.fontSize = size+'px';
  inputform.style.background =  'none';
  inputform.value = txt;
  inputform.size = jstrlen(txt) + 1;
  inputform.focus();
}


/////// CanvasCalss
//
var SvgCanvas = function(id, width, height){
  this.id = id;
  this.width = width;
  this.height = height;

  this.canvas=null;
  this.canvasOffsetX = 0;
  this.canvasOffsetY = 0;

  this.svgObjectList=new Array();

  this.selectedItems=new Array();


  ////// SVG DefaultValues
  this.svg_scale_dir = null;

  this.svg_top = null;
  this.svg_select = null;
  this.svg_defs = null;

  this.svg_font_size = 24;
  this.svg_font_family = 'Helvetica';

  this.svg_color = '#000000';
  this.svg_fill_color = '#ffffff';
  this.svg_line_width = 1;

  this.dupItems = null;
  this.dupX = 0;
  this.dupY = 0;


  this. editingTextObj = null;
  this. svg_rotate_locked = false;
  this. svg_scale_locked = false;

  this. is_newPath;

  this.initCanvas(id, width, height);
}

SvgCanvas.prototype.initCanvas=function(id, w, h){
  this.canvas = document.getElementById(id);

  if(this.canvas){
    removeChildNodes(this.canvas);
    this.canvas.style.display='block';
    this.canvas.style.top='180px';
    this.canvas.style.bottom='50px';
    this.canvas.style.left='0px';
    this.canvas.style.right='10px';
    this.canvas.style.width=w+'px';
    this.canvas.style.height=h+'px';

    //// Set SVG Top Element
    var top_element = this.toSvgElement('', '100%','100%');
    this.canvas.appendChild(top_element);
    this.svg_top = document.getElementById('svg_top');
    this.initSvgElementId(this.svg_top);
    this.svg_defs = null;
    this.svg_top.canvas = this;


    //// SelectBox
    this.svg_select = this.createSvgElement('rect',
	   'x=0,y=0,width=1,height=1,visibility=hidden,stroke-dasharray=9 5',
	   'none', 'blue', 1);
    this.svg_select.setAttribute("id", "svg_select");
    this.appendSvgElement(this.svg_select);

    this.info_text = this.addText(0, 0, "", 12, '#008800');
    this.refreshOffsets();

    useragent = getBrowserInfo();
  }
}

SvgCanvas.prototype.setCanvasSize=function(w, h){
  if(this.canvas){
    this.canvas.style.width=w+'px';
    this.canvas.style.height=h+'px';
  }else{
    alert("Canvas isn't initialized");
  }
}

SvgCanvas.prototype.resize=function(dw, dh){
  if(this.canvas){
    w =  parseInt(this.canvas.style.width) + dw;
    h =  parseInt(this.canvas.style.height) + dh;
    this.canvas.style.width=w+'px';
    this.canvas.style.height=h+'px';
  }else{
    alert("Canvas isn't initialized");
  }
}

SvgCanvas.prototype.refreshOffsets=function(){
  var offxy = getPageOffset(this.canvas);
  this.canvasOffsetX = offxy[0];
  this.canvasOffsetY = offxy[1];
}

SvgCanvas.prototype.toSvgElement=function(str, w, h){
  var xmlsvg = "xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
  var parser = new DOMParser();
  var header = "<svg:svg currentScale=\"1\" width=\""+w+"\" height=\""+h+"\"  "+xmlsvg+" id=\"svg_top\">";
  var footer = "</svg:svg>";
  var xmlDoc = parser.parseFromString(header+str+footer, "text/xml");
  var xmlRoot = xmlDoc.documentElement;
  var ele =  document.importNode(xmlRoot,true);

  return ele;
}

SvgCanvas.prototype.initSvgElementId=function(top_ele){
  nextId = 1;
  this.checkSvgElementId(top_ele);
  this.appendSvgElementId(top_ele);
}

SvgCanvas.prototype.checkSvgElementId=function(top_ele){
  var objs = top_ele.childNodes;

  for(var i=0; i<objs.length ;i++){
    if(objs[i].tagName){
      var id = objs[i].getAttribute("id");

      if(id && id.match(/svg_[0-9]+/i)){
         var n = parseInt(RegExp.lastMatch.substr(4));
         if(n >= nextId){ nextId = n+1; }
      }
      this.checkSvgElementId(objs[i]);
    }
  }
}

SvgCanvas.prototype.appendSvgElementId=function(top_ele){
  var objs = top_ele.childNodes;

  for(var i=0; i<objs.length ;i++){
    if(objs[i].tagName){
      var id = objs[i].getAttribute("id");
      if(!id){
         objs[i].setAttribute("id", "svg_"+ nextId);
         nextId++;
      }
    }
    this.appendSvgElementId(objs[i]);
  }
}

SvgCanvas.prototype.getX=function(x){
 return x - this.canvasOffsetX;
}

SvgCanvas.prototype.getY=function(y){
 return y - this.canvasOffsetY;
}

SvgCanvas.prototype.createElement=function(tag, id){
  var svg_ns = 'http://www.w3.org/2000/svg';

  if(!id || id=='new'){ id = newID(); }
  var ele= document.createElementNS(svg_ns, tag);
  ele.setAttribute("id", id);
  return ele;
}

SvgCanvas.prototype.defElement=function(node){
  if(!this.svg_defs){ this.svg_defs = document.getElementById('svg_defs'); }
  if(!this.svg_defs){
    this.svg_defs = createElement('defs', 'svg_defs');
    this.svg_top.insertBefore(svg_defs, svg_top.firstChild);
  }
  if(node){ this.svg_defs.appendChild(node); }
}

SvgCanvas.prototype.appendSvgObject=function(obj){
  this.appendSvgElement2(obj.element, null);
  obj.canvas=this;
  SvgObjectList.push(obj);
}

SvgCanvas.prototype.appendSvgElement=function(obj){
  this.appendSvgElement2(obj, null);
}

SvgCanvas.prototype.appendSvgElement2=function(obj, root_ele){
  if(!root_ele){ root_ele = this.svg_top; }
  if(!root_ele){ return; }
  root_ele.appendChild(obj);
}

SvgCanvas.prototype.pushSvgElement2=function(obj, root_ele){
  if(!root_ele){ root_ele = this.svg_top; }
  if(!root_ele){ return; }
  root_ele.insertBefore(obj,root_ele.firstChild);
}

SvgCanvas.prototype.removeSvgElement=function(obj){
  this.removeSvgElement2(obj, null);
}

SvgCanvas.prototype.removeSvgElement2=function(obj, root_ele){
  if(!root_ele){ root_ele = this.svg_top; }
  if(!root_ele){ return; }
  try{
    root_ele.removeChild(obj);
  }catch(err){
    console.log(root_ele+","+obj+" : "+err);
  }
}

SvgCanvas.prototype.removeSvgObject=function(obj){
  this.removeSvgElement2(obj.element, null);
  array_remove(SvgObjectList, obj);
}


SvgCanvas.prototype.findSvgElement=function(obj, root_ele){
  if(!root_ele){ root_ele = this.svg_top; }
  if(!root_ele){ return false; }

  nodes = root_ele.childNodes;
  for (var i=0; i<nodes.length;i++){
    if(nodes[i] == obj){
      return true;
    }
  }
  return false;
}

SvgCanvas.prototype.createSvgElement=function(tag, attrs, fill, color, lw){
  var ele = this.createElement(tag, 'new');
  if (attrs){ setAttributes(ele, attrs); }
  if (fill){ ele.setAttribute('fill', fill); }
  if (color){ ele.setAttribute('stroke', color); }
  if (lw){ ele.setAttribute('stroke-width', lw); }

  return ele;
}

SvgCanvas.prototype.createSvgText=function(txt, x, y, size, color){
  var ele = this.createElement('text', 'new');

  ele.setAttribute('x', x);
  ele.setAttribute('y', y);
  ele.setAttribute('font-size', size);
  ele.setAttribute('fill', color);
  ele.textContent=txt;

  return ele;
}

SvgCanvas.prototype.createSvgImage=function(fname, width, height, attrs){
  var ele = this.createElement('image', 'new');
  ele.setAttributeNS('http://www.w3.org/1999/xlink', 'xlink:href', fname);
  ele.setAttribute('width', width);
  ele.setAttribute('height', height);
  setAttributes(ele, attrs);

  return ele;
}

SvgCanvas.prototype.delSvgElement=function(){
  if(!this.svg_top){ return; }
  for(var i=0; i< selectedItems.length; i++){
    var obj = selectedItems[i];
    this.removeAllMarker(obj);
    this.svg_top.removeChild(obj);
  }
  this.hideSelectedRectangle();
}

SvgCanvas.prototype.createMarker=function(pid, id, child){
  var parent = document.getElementById(pid);
  if(!parent){ return; }
  var ele = this.createElement('marker', pid+'_'+id);
  ele.appendChild(child);
  return ele;
}

SvgCanvas.prototype.removeAllMarker=function(obj){
  if(!this.svg_defs){ return false; }
  var smarker = getArrowMarker(obj,'start');
  if(smarker){ this.svg_defs.removeChild(smarker);}
  var emarker = getArrowMarker(obj,'end');
  if(emarker) { this.svg_defs.removeChild(emarker); }
  return true;
}

//// Primitive
SvgCanvas.prototype.addCircle=function(x, y, r, color){
  var obj = new SvgCircle(x,y,r,color);
  this.appendSvgObject(obj);
  return obj.element;
}

SvgCanvas.prototype.addPolygon=function(x, y, points, color){
  var obj = new SvgPolygon(x,y,points,color);
  this.appendSvgObject(obj);
  return obj.element;
}

SvgCanvas.prototype.addRect=function(x, y, w, h, color){
  var obj = new SvgRect(x,y,w,h,color);
  this.appendSvgObject(obj);
  return obj.element;
}

SvgCanvas.prototype.addLine=function(x1, y1, x2, y2, color, style, width){
  var obj = new SvgLine(x1,y1,x2,y2,color,style,width);
  this.appendSvgObject(obj);
  return obj.element;
}

SvgCanvas.prototype.addPath=function(x1, y1, x2, y2, color, style, width){
  var obj = new SvgPath(x1,y1,x2,y2,color,style,width);
  this.appendSvgObject(obj);
  return obj.element;
}

SvgCanvas.prototype.addText=function(x, y, txt, size, color){
  var obj = new SvgText(x, y, txt, size, color);
  this.appendSvgObject(obj);
  return obj.element;
}


SvgCanvas.prototype.groupSvgObjects=function(objs){
  try{ 
    if(!objs){ objs = selectedItems; }
    if(!objs || objs.length == 0){ return null; }

    var ele = this.createElement('g', 'new');
    for(var i=0;i<objs.length;i++){
      var obj = objs[i];
       obj.parentNode.removeChild(obj);
       ele.appendChild(obj);
    }
    this.appendSvgElement(ele);
    selectedItems = new Array(ele);

    return ele;
  }catch(e){ return null;}
}

SvgCanvas.prototype.ungroupSvgObjects=function(obj){
  try{ 
    if(!obj && selectedItems.length > 0){ obj = selectedItems[0]; }

    if(getElementTag(obj) != 'g') return;
    var parent = obj.parentNode;
    selectedItems = new Array();
    while(obj.firstChild != null){
      selectedItems.push(obj.firstChild);
      parent.appendChild(obj.firstChild);
    }
    parent.removeChild(obj);

  }catch(e){ return;}
}

SvgCanvas.prototype.setObjectColor=function(obj, color_sel, color){
  try{
    var tag = getElementTag(obj);
    if(tag == "NoName"){ return; }
    if(tag == "g"){
      var objs = obj.childNodes;
      for(var i=0;i<objs.length;i++){
        this.setObjectColor(objs[i], color_sel, color);
      }
    }
    obj.setAttribute(color_sel, color);

    if(color_sel == 'fill'){
      this.svg_fill_color = color;
    }else{
      this.svg_color = color;
    }

  }catch(e){
    console.log(e);
  }
}

SvgCanvas.prototype.selectedRectangle=function(x, y, ex, ey){
  if(!this.svg_select){ return null; }

  this.svg_select.setAttribute("x",x);
  this.svg_select.setAttribute("y",y);
  this.svg_select.setAttribute("width", ex-x);
  this.svg_select.setAttribute("height", ey-y);
  this.svg_select.setAttribute("visibility", "visible");
  return this.svg_select;
}

SvgCanvas.prototype.hideSelectedRectangle=function(){
  this.svg_select.setAttribute("x",0);
  this.svg_select.setAttribute("y",0);
  this.svg_select.setAttribute("width",0);
  this.svg_select.setAttribute("height",0);
  this.svg_select.setAttribute("visibility","hidden");
}

SvgCanvas.prototype.setSelectBox=function(){
  if(!this.svg_select){ return; }

  if(selectedItems.length == 0){
    this.hideSelectedRectangle()
    return;
  }

  var bbox = new Array(1000,1000,0,0);
  
  for(var i=0;i<selectedItems.length;i++){
    var bp = getBoundingBox(selectedItems[i]);

    if(bp[0] < bbox[0]) bbox[0]=bp[0];
    if(bp[1] < bbox[1]) bbox[1]=bp[1];
    if(bp[2] > bbox[2]) bbox[2]=bp[2];
    if(bp[3] > bbox[3]) bbox[3]=bp[3];
  } 
  this.selectedRectangle(bbox[0], bbox[1], bbox[2], bbox[3]);
}

SvgCanvas.prototype.updateSelectedRectangle=function(){
   if(!this.svg_select){ return; }
   if(selectedItems.length > 0){
     this.svg_select.setAttribute("visibility","visible");
   }else{
     this.hideSelectedRectangle();
   }
}

SvgCanvas.prototype.clearSelectedItems=function(){
  for(i in selectedItems){ delete selectedItems[i]; }
  selectedItems=new Array();
  this.updateSelectedRectangle();
}

SvgCanvas.prototype.moveSelectedRectangle=function(dx, dy){
  if(!this.svg_select) return;

  if(useragent.indexOf("IE") < 0){
    this.svg_select.setAttribute("transform","translate("+dx+","+dy+")");
  }else{
    this.svg_select.setAttribute("transform","translate("+dx+" "+dy+")");
  }
}

///////
SvgCanvas.prototype.getSvgContent=function(){
  if(!this.svg_top){ return ""; }

  var str = "";
  var elements = this.svg_top.childNodes;
  for(var i=0; i<elements.length; i++){
    if(elements[i] != this.svg_select){
      var tmp = trim(this.formatTag(elements[i]));
      if(tmp){ str += tmp + '\n'; }
    }
  }
  return str;
}

SvgCanvas.prototype.formatTag=function(ele){
  var str="";
  if(ele.nodeType == 1){
    var tag_a = ele.tagName.split(':');
    var tag;
    if(tag_a.length == 1){ tag = "svg:"+tag_a[0]; }else{ tag = ele.tagName; }

    str += "<"+tag;
    var attrs = ele.attributes;
    for(var i=0; i<attrs.length; i++){
      //str += " "+attrs[i].nodeName+"=\""+attrs[i].nodeValue+"\"";
      str += " "+attrs[i].nodeName+"=\""+attrs[i].value+"\"";
    }
    var cn = ele.childNodes;
    if(cn.length > 0){
      str +=">\n";
      for(var i=0; i<cn.length; i++){
        var tmp = trim(this.formatTag(cn[i]));
        if(tmp) str += " "+tmp+"\n";
      }
      str += "</"+tag+">";
    }else{
      str +=" />";
    }
    return str;
  }else if(ele.nodeType==3){
    return ele.textContent;
  }
}

SvgCanvas.prototype.downItem=function(){
  if(selectedItems.length != 1){ return; }

  var itm = selectedItems[0];
  var nodes = this.svg_top.childNodes;
  for(var i=0; i< nodes.length; i++){ if(nodes[i] == itm) break; }
  if (i > 0){ this.svg_top.insertBefore(itm, nodes[i-1]); }
}

SvgCanvas.prototype.upItem=function(){
  if(selectedItems.length != 1){ return; }
  var itm = selectedItems[0];
  var nodes = this.svg_top.childNodes;
  for(var i=0; i< nodes.length; i++){ if(nodes[i] == itm) break; }

  if (i == nodes.length-2){
    this.svg_top.appendChild(itm);
  }else if (i < nodes.length-1){
    this.svg_top.insertBefore(itm, nodes[i+2]); 
  }
}

SvgCanvas.prototype.bottomItem=function(){
  if(selectedItems.length != 1){ return; }
  var itm = selectedItems[0];
  this.svg_top.insertBefore(itm, this.svg_top.firstChild); 
}

SvgCanvas.prototype.topItem=function(){
  if(selectedItems.length != 1){ return; }
  var itm = selectedItems[0];
  this.svg_top.removeChild(itm);
  this.svg_top.appendChild(itm);
}

SvgCanvas.prototype.setRotLock=function(){
  if(!this.svg_rotate_locked){
    this.svg_rotate_locked = true;
    this.svg_scale_locked = false;
    try{
      var lock = document.getElementById('lock');
      lock.style.top = '450px';
      lock.style.display='block';
    }catch(e){}
  }
}

SvgCanvas.prototype.setScaleLock=function(){
  if(!this.svg_scale_locked){
    this.svg_scale_locked = true;
    this.svg_rotate_locked = false;
    try{
      var lock = document.getElementById('lock');
      lock.style.top = '470px';
      lock.style.display='block';
    }catch(e){}
  }
}

SvgCanvas.prototype.toggleRotateScaleLock=function(){
  try{
    var lock = document.getElementById('lock');
    if(this.svg_rotate_locked){
      this.svg_rotate_locked=false;

      if(lock.style.top == '450px'){
        lock.style.display='none';
      }else{
        this.svg_scale_locked=true;
        lock.style.top = '470px';
      }
      return;
    }

    if(this.svg_scale_locked){
      this.svg_scale_locked=false;

      if(lock.style.top == '470px'){
        lock.style.display='none';
      }else{
        this.svg_rotate_locked=true;
        lock.style.top = '450px';
      }
      return;
    }
  }catch(e){ alert("Error in toggleRotateScaleLock"); }
}

//////// SVG Elements
var SvgObject = function(id, tag){
  if(!id){ return; }

  this.svg_ns = 'http://www.w3.org/2000/svg';
  this.lineW = new Array(0.25, 0.5, 0.75, 1, 1.5, 2.25, 3, 4.5, 6);
  this.StrokeDash = new Array("", "2", "6 2", "6 2 1 2", "8 2", "8 2 1 2", "8 2 1 2 1 2" );
  this.strokeWidthIndex = 3;
  this.strokeDashIndex = 0;

  if(!id || id=='new'){ id = newID(); }
  var obj = document.getElementById(id);
  this.id = id;

  if(!obj){
    this.element = document.createElementNS(this.svg_ns, tag);
    this.element.setAttribute("id", id);
    this.tag = tag;
    this.type = "SvgObject";
  }else{
    this.element = obj;
    this.tag = obj.tag;
    this.type = obj.type;
  }

  this.selectBox;
  this.element.svgobj = this;
  this.obj = null;
  this.canvas = null;
  this.selected = false;
}

SvgObject.prototype.isSelected=function(){
  return this.selected;
}

SvgObject.prototype.setSelected=function(v){
  this.selected = v;
  return this.selected;
}

SvgObject.prototype.setSelectBox=function(){
  try{
    var bbox = this.element.getBBox();

    var x = bbox.x - 4;
    var y = bbox.y - 4;
    var w = bbox.width+8;
    var h = bbox.height+8;

    var attrs = 'x='+x+',y='+y+',width='+w+',height='+h+',fill=none,stroke=#ff0000,stroke-width=1';
    this.selectBox = document.createElementNS(this.svg_ns, 'rect');
    this.selectBox.setAttribute('id', this.id+"-select"); 
    setAttributes(this.selectBox, attrs); 
  }catch(e){
    console.log(e);
    this.selectBox = null;
  }
}

SvgObject.prototype.showSelectBox=function(){
  if(!this.selectBox){ this.setSelectBox(); }
  if(this.selectBox){
    if(!this.selected){
      this.element.parentNode.insertBefore(this.selectBox, this.element);
      this.selected = true;
    }
  }
}

SvgObject.prototype.hideSelectBox=function(){
  if(this.selectBox){
    //Canvas.removeSvgElement(this.selectBox);
    if(this.selected){
      this.element.parentNode.removeChild(this.selectBox);
      this.selected = false;
    }
    //this.selectBox.setAttribute('display','none');
  }
}

SvgObject.prototype.getStrokeWIdx=function(lw){
  var res=0;
  for(var i=0;i< this.lineW.length;i++){
    if(lw <= this.lineW[i]){ return res; }
    res = i; 
  }
  return this.lineW.length-1;
}

SvgObject.prototype.getStrokeW=function(idx){ return this.lineW[idx]; }

SvgObject.prototype.selectStrokeW=function(idx){
  if(this.obj){
    var lw = this.getStrokeW(idx);
    var plw = parseFloat(this.obj.getAttribute('stroke-width'));
    this.obj.setAttribute('stroke-width', lw);
    this.updateDashArray(lw, plw, this.obj.getAttribute('stroke-dasharray'));
  }
}

SvgObject.prototype.updateStrokeWidth=function(){
  if(this.obj){
    var val = this.obj.getAttribute('stroke-width'); 
    if(!val){ return; }
    var lw = parseFloat(val);
    this.strokeWidthIndex = getStrokeWIdx(lw);
    this.updateStrokeDash();
  }
}

SvgObject.prototype.updateStrokeDash=function(){
  if(this.obj){
    var val = this.obj.getAttribute('stroke-width'); 
    if(!val){ return; }
    var lw = parseFloat(val);
    var darr = this.obj.getAttribute('stroke-dasharray'); 
    if(!darr){ return; }
  }
}

SvgObject.prototype.selectStrokeDash=function(idx){
  if(this.obj){
    var lw = this.obj.getAttribute('stroke-width');
    if(!lw ){ lw=1; }
    else{ lw=parseFloat(lw); }

    var darr = StrokeDash[idx];
    if(darr=="") try{ this.obj.removeAttribute('stroke-dasharray'); }catch(e){}
    else updateDashArray(lw, 1, darr);
  }
}

SvgObject.prototype.getDashArrayIndex=function(plw, darr){
  if(!darr || darr=="") return 0;
  var d_arr = darr.split(' ');
  var top = Math.round(parseFloat(d_arr[0])/plw);
  if(top == 6){
    if(d_arr.length == 2) return 2;
    if(d_arr.length == 4) return 3;
  }else if(top == 8){
    if(d_arr.length == 2) return 4;
    if(d_arr.length == 4) return 5;
    if(d_arr.length == 6) return 6;
  }else if(top == 2){
    return 1;
  }
  return 0;
}

SvgObject.prototype.updateDashArray=function(lw, plw, darr){
  if(this.obj) { 
    if(darr && darr != ""){
       var d_arr = darr.split(' ');
       darr = "";
       for(var i=0; i<d_arr.length; i++){
          var val = Math.round(parseFloat(d_arr[i])/plw);
          darr += val * lw  + ' ';
       } 
       this.obj.setAttribute('stroke-dasharray', trim(darr));
       return true;
    }
  }
  return false;
}

SvgObject.prototype.setAttributes=function(attrs){
  var attr_array = attrs.split(',');

  for (var i=0; i<attr_array.length;i++){
    var x = attr_array[i].split('=');
    if(x.length == 2){
      this.element.setAttribute(x[0], x[1]);
    }
  }
}

SvgObject.prototype.addAttributeVal=function(itm, dv){
  var obj = this.element;
  if(!obj) {  obj = this; }
  if(obj.hasAttribute(itm)){
    var x = parseInt(obj.getAttribute(itm));
    x = x + dv;
    obj.setAttribute(itm, x)
  }else alert("No such attribute:"+itm);
}

SvgObject.prototype.getAttributeVal=function(name){
  try{
    var obj = this.element;
    if(!obj) {  obj = this; }
    return parseInt(obj.getAttribute(name)); 
  }catch(e){ return 0; }
}

SvgObject.prototype.getAttribute=function(name){
  return this.element.getAttribute(name); 
}

SvgObject.prototype.removeAttribute=function(name){
  return this.element.removeAttribute(name); 
}

SvgObject.prototype.setAttribute=function(name, val){
  return this.element.setAttribute(name,val); 
}

SvgObject.prototype.checkIntersection=function(x, y, ex, ey){
  var bbox = this.getBBox();
  if(!bbox){ return false; }
  var ox = bbox.x;
  var oy = bbox.y;
  var oex = bbox.x+bbox.width;
  var oey = bbox.y+bbox.height;
  if( ex < ox ||  oex < x || ey < oy || oey < y){ return false; }
  return true;
}

SvgObject.prototype.translate=function(dx, dy){
  if(this.addAttributeVal){
    this.addAttributeVal("x", dx);
    this.addAttributeVal("y", dy);
    if(this.selectBox){ translateObject(this.selectBox, dx, dy); }
  }else{
    addAttributeVal(this, "x", dx);
    addAttributeVal(this, "y", dy);
  }
}

SvgObject.prototype.getObjectPos=function(){
  
  if(this.getAttributeVal){
    var x1 = this.getAttributeVal("x");
    var y1 = this.getAttributeVal("y");
    
    return new Array(x1, y1);
  }else{
    var x1 = parseInt(this.getAttribute('x')); 
    var y1 = parseInt(this.getAttribute('y')); 

    return new Array(x1, y1);
  }
}

SvgObject.prototype.setObjectPos=function(x, y){
  var pos;
  if(this.getObjectPos){
    pos = this.getObjectPos();
  }else{
    pos = getObjectPos(this);
  }
  var dx = x - pos[0];
  var dy = y - pos[1];
 
  if(this.translate){
    this.translate(dx, dy);
  }else{
    translateObject(this, dx, dy);
  }
}

SvgObject.prototype.updateTransform=function(){
  try{
    var trans = this.getAttribute("transform");
    if(!trans){ return; }
   
    if(useragent.indexOf("IE") < 0){
      search_str = "translate(.+,.+)";
      sep_char = ',';
    }else{
      search_str = "translate(.+ .+)";
      sep_char = ' ';
    }
    if(trans.match(new RegExp(search_str,"i"))){
      var str = RegExp.lastMatch;
      var vals = str.substr(10,str.length-11).split(sep_char) ;
      var dx = parseInt(vals[0]);
      var dy = parseInt(vals[1]);

      if(this.translate){
        this.translate(dx, dy);
        this.replaceTranslate(0, 0);
        this.updateRotate();
      }else{
        translateObject(this, dx, dy);
        replaceTranslate(this, 0, 0);
        updateRotate(this);
      }
    }
  }catch (e){
    alert("Error in updateTransform:"+e);
  }

  if(this.selectBox){ updateTransform(this.selectBox); }
}

SvgObject.prototype.replaceTranslate=function(dx, dy){
  var trans = this.getAttribute("transform");

  if(trans && trans.indexOf("translate")>=0){
    var strs = trans.split(' ');
    trans = "";
    for(var i=0; i<strs.length; i++){
      if(strs[i].indexOf("translate")>=0){
        if (dx == 0 && dy == 0){
          strs[i]="";
        }else{
          if(useragent.indexOf("IE") < 0){
            strs[i] = "translate("+dx+","+dy+")";
          }else{
            strs[i] = "translate("+dx+" "+dy+")";
            strs[i+1] = "";
          }
        }
      }
      
      trans += strs[i]+" ";
    }
    if(trim(trans)){
      this.setAttribute("transform", trim(trans));
    }else{
      this.removeAttribute("transform");
    }
  }else{
    if(trans){
      if(useragent.indexOf("IE") < 0){
        trans += " translate("+dx+","+dy+")";
      }else{
        trans += " translate("+dx+" "+dy+")";
      }
      this.setAttribute("transform", trans);
    }else{
      if(useragent.indexOf("IE") < 0){
        this.setAttribute("transform","translate("+dx+","+dy+")");
      }else{
        this.setAttribute("transform","translate("+dx+" "+dy+")");
      }
    }
  }
  updateRotate(this,dx,dy);

  if(this.selectBox){
    replaceTranslate(this.selectBox, dx, dy);
    updateRotate(this.selectBox ,dx,dy);
  }
}

SvgObject.prototype.getBBox=function(){
  try{
    return this.element.getBBox();
  }catch(e){
//    alert(e);
  }
  return null;
}

SvgObject.prototype.updateRotate=function(){
  var trans = this.getAttribute("transform");

  if(!trans || trans.indexOf("rotate") < 0) return; 

  var bbox = this.getBBox();

  var x = bbox.x+bbox.width/2;
  var y = bbox.y+bbox.height/2;
  var strs = trans.split(' ');
  trans = "";

  for(var i=0; i<strs.length ;i++){
    if(strs[i].indexOf("rotate") >= 0){
       var deg = strs[i].substr(7, strs[i].indexOf(",")-7);
       if(dx) x += dx;
       if(dy) y += dy;
       strs[i] = "rotate("+deg+","+x+","+y+")";
    }
    trans += strs[i]+" ";
  }
  this.setAttribute("transform",trim(trans));
}

SvgObject.prototype.getRotateAngle=function(){
  var trans = this.getAttribute("transform");
  if(!trans || trans.indexOf("rotate") < 0) return 0; 
  var strs = trans.split(' ');
  for(var i=0; i<strs.length ;i++){
    if(strs[i].indexOf("rotate") >= 0){
       var deg = strs[i].substr(7, strs[i].indexOf(",")-7);
       return parseInt(deg);
    }
  }
  return 0;
}

SvgObject.prototype.setRotate=function(deg){
  var trans = this.getAttribute("transform");
  var bbox = this.getBBox();
  var x = bbox.x+bbox.width/2;
  var y = bbox.y+bbox.height/2;

  if(!trans){ trans=""; }
  if(trans.indexOf("rotate") < 0){
    trans = "rotate("+deg+","+x+","+y+")";
    this.setAttribute("transform", trans);
  }else{
    var strs = trans.split(' ');
    trans = "";
    for(var i=0; i<strs.length ;i++){
      if(strs[i].indexOf("rotate") >= 0){ strs[i] = "rotate("+deg+","+x+","+y+")"; }
      trans += strs[i]+" ";
    }
    this.setAttribute("transform",trim(trans));
  }
}

SvgObject.prototype.getScale=function(){
  var trans = this.getAttribute("transform");
  if(!trans || trans.indexOf("scale") < 0){ return null; }
  var strs = trans.split(' ');
  for(var i=0; i<strs.length ;i++){
    if(strs[i].indexOf("scale") >= 0){
       var degs = strs[i].substr(6, strs[i].indexOf(")")-6);
       var degs = degs.split(',');
       return new Array(parseFloat(degs[0]), parseFloat(degs[1]));
    }
  }
  return null;
}

SvgObject.prototype.setScale=function(scaleX, scaleY){
  if(this.canvas){ return; }
  var trans = this.getAttribute("transform");

  var x = parseInt(this.canvas.svg_select.getAttribute("x"));
  var y = parseInt(this.canvas.svg_select.getAttribute("y"));

  var dx = x - scaleX*x;
  var dy = y - scaleY*y;

  if(!trans){ trans=""; }
  if(trans.indexOf("scale") < 0){
    trans = "scale("+scaleX+","+scaleY+")";
    this.setAttribute("transform", trans);
  }else{
    var strs = trans.split(' ');
    trans = "";
    for(var i=0; i<strs.length ;i++){
      if(strs[i].indexOf("scale") >= 0){ strs[i] = "scale("+scaleX+","+scaleY+")"; }
      if(scaleX == 1 && scaleY == 1){ continue; }
      trans += strs[i]+" ";
    }
    this.setAttribute("transform", trim(trans));
  }

  if(this.replaceTranslate){
    this.replaceTranslate(dx/scaleX, dy/scaleY);
  }else{
    replaceTranslate(this, dx/scaleX, dy/scaleY);
  }
}

SvgObject.prototype.getBoundingBox=function(){
  var res = new Array(4);
  var bbox = this.getBBox();

  res[0] = bbox.x-1;
  res[1] = bbox.y-1;
  res[2] = bbox.x+bbox.width+2;
  res[3] = bbox.y+bbox.height+2;
  return res;
}

SvgObject.prototype.getTranslateValue=function(){
  var trans = this.getAttribute("transform");
  var res = new Array(0, 0);

  if(trans && trans.indexOf("translate")>=0){
    var val = trans.match(/translate\(([-\d]\d*)[ ,]([-\d]\d*)\)/);
    if(val){
       res[0] = RegExp.$1;
       res[1] = RegExp.$2;
    }
  }
  return res;
}

SvgObject.prototype.setLeftArrow=function(){
  this.setArrow('start');
}

SvgObject.prototype.setRightArrow=function(){
  this.setArrow('end');
}

SvgObject.prototype.removeLeftArrow=function(){
  var svg_defs = document.getElementById('svg_defs');
  try{
    var marker = this.getArrowMarker('start');
    svg_defs.removeChild(marker);
    this.removeAttribute('marker-start');
  }catch(e){ }
}

SvgObject.prototype.removeRightArrow=function(){
  var svg_defs = document.getElementById('svg_defs');
  try{
    var marker = this.getArrowMarker('end');
    svg_defs.removeChild(marker);
    this.removeAttribute('marker-end');
  }catch(e){ }
}

SvgObject.prototype.removeAllMarker=function(){
  var svg_defs = document.getElementById('svg_defs');
  if(!svg_defs) { return false; }

  var smarker = this.getArrowMarker('start');
  if(smarker){
    svg_defs.removeChild(smarker);
  }
  var emarker = this.getArrowMarker('end');
  if(emarker){
    svg_defs.removeChild(emarker);
  }
  return true;
}

SvgObject.prototype.hasArrow=function(pos){
  if(this.getArrowMarker(pos)) return true;
  return false;
}

SvgObject.prototype.getArrowMarker=function(pos){
  try{
    var id = this.getAttribute('id');
    return document.getElementById(id+'_'+pos+'arrow');
  }catch(e){ return null; }
}

SvgObject.prototype.setArrow=function(pos){
  if(!this.canvas){ return; }
//  var marker = this.getArrowMarker(pos);
  var marker = getArrowMarker(this, pos);

  if(marker){ return; }

  var arrow = new Array();
  arrow['start'] = 'd=M 20 0 L 0 10 20 20 Z';
  arrow['end'] = 'd=M 0 0 L 20 10 0 20 Z';

  var obj = SvgCanvas.prototype.createElement('path', 'new');
  setAttributes(obj, arrow[pos]);
  var pid = this.getAttribute("id");
  var marker_id = pid+'_'+pos+'arrow';
  marker = SvgCanvas.prototype.createElement('marker', marker_id);
  marker.appendChild(obj);
  
  var refX = 10;
  var refY = 10; 

  setAttributes(marker,'markerWidth=10,markerHeight=10,orient=auto,viewBox=0 0 20 20,markerUnits=strokeWidth,refX='+refX+',refY='+refY);
  marker.setAttribute("fill", this.getAttribute("stroke")); 
  this.canvas.defElement(marker);

  var mid = "url(#"+marker_id+")";
  var mattr = "marker-"+pos;
  this.setAttribute(mattr, mid);
}

SvgObject.prototype.scale=function(scale, dir){
}

SvgObject.prototype.updateScale=function(){
}

SvgObject.prototype.clone=function(){
  var obj = new SvgObject(this.id, this.tag);

  obj.element = this.element.cloneNode(true);
  obj.setAttribute("id", newID());

  return obj;
}

SvgObject.prototype.remove=function(){
  try{
    var ele = this.element;
    ele.parentNode.removeChild(ele);
    array_remove(SvgObjectList, this);
  }catch(e){
  }
}

SvgObject.prototype.onMouseDown=function(e, x1, y1, x2, y2, flag){
  if(this.checkIntersection(x1, y1, x2, y2)){
    if(!this.selected){
      this.showSelectBox();
      selectedItems.push(this);
    }
  }else if(this.selected){
    array_remove(selectedItems, this);
    this.hideSelectBox();
  }
}

SvgObject.prototype.onMouseMove=function(e, dx, dy, x, y){
  this.replaceTranslate(dx, dy);
}

SvgObject.prototype.onMouseUp=function(e){
  this.updateTransform();
}


//////// SVG Elements
// Rectangle
var SvgRect = function(x, y, w, h, color){
  SvgObject.call(this, 'new', 'rect');

  if(!color){ color='#aaffaa'; }
  var attrs = 'x='+x+',y='+y+',width='+w+',height='+h+',fill='+color+',stroke=#000000,stroke-width=1';
  this.setAttributes(attrs); 
  this.type = "SvgRect";
  this.width = w;
  this.height = h;
}

SvgRect.prototype = new SvgObject();

SvgRect.prototype.checkIntersection=function(x, y, ex, ey){
  var lx = parseInt(this.getAttribute("x"));
  var w = parseInt(this.getAttribute("width"));
  var ly = parseInt(this.getAttribute("y"));
  var h = parseInt(this.getAttribute("height"));
  var rx = lx + w;
  var uy = ly + h;
  var cx = (lx + rx )/ 2;
  var cy = (ly + uy )/ 2;

  if(x <= cx && ex >= cx && y <= cy && ey >= cy){
     return true;
  }else if(x >= lx && x <= rx && y >= ly && y <= uy){
     return true;
  }else{
     return false;
  }
}

SvgRect.prototype.scale=function(scale, dir){
  if( dir != 'y'){
    this.setAttribute("width", Math.round(this.width * scale ));
  }
  if( dir != 'x'){
     this.setAttribute("height", Math.round(this.height * scale ));
  }
}

SvgRect.prototype.updateScale=function(){
  this.width = parseInt(this.getAttribute("width"));
  this.height = parseInt(this.getAttribute("height"));
}

SvgRect.prototype.clone=function(){
  var obj = new SvgRect(this.getAttribute('x'), this.getAttribute('y'), this.width, this.height, this.getAttribute('fill'));

  return obj;
}

// Circle
var SvgCircle = function(x, y, r, color){
  SvgObject.call(this, 'new', 'circle');

  if(!color){ color='#ff00ff'; }
  var attrs = 'cx='+x+',cy='+y+',r='+r+',fill='+color+',stroke=#000000,stroke-width=1';
  this.setAttributes(attrs); 
  this.r = r;
  this.type = "SvgCircle";
}

SvgCircle.prototype = new SvgObject();

SvgCircle.prototype.checkIntersection=function(x, y, ex, ey){
  var res = SvgObject.prototype.checkIntersection.call(this,x, y, ex, ey);

  if(res){
    var cx = this.getAttributeVal("cx"); 
    var cy = this.getAttributeVal("cy"); 
    var r = this.getAttributeVal(obj,"r"); 

    if(cx <x && cy < y && !isIncludeEllipse(x,y,cx,cy,r,r)){ res=false; }
    else if(cx < x && cy > ey && !isIncludeEllipse(x,ey,cx,cy,r,r)){ res=false; }
    else if(cx > ex && cy > ey && !isIncludeEllipse(ex,ey,cx,cy,r,r)){ res=false; }
    else if(cx > ex && cy < y && !isIncludeEllipse( ex,y,cx,cy,r,r)){ res=false; }
  }
  return res;
}

SvgCircle.prototype.translate=function(dx, dy){
  this.addAttributeVal("cx", dx);
  this.addAttributeVal("cy", dy);
  if(this.selectBox){ translateObject(this.selectBox, dx, dy); }
}

SvgCircle.prototype.getObjectPos=function(){
  var x1 = this.getAttributeVal("cx");
  var y1 = this.getAttributeVal("cy");
  return new Array(x1, y1);
}

SvgCircle.prototype.scale=function(scale){
  this.setAttribute("r", Math.round(this.r * scale ));
}

SvgCircle.prototype.updateScale=function(){
  this.r = parseInt(this.getAttribute("r"));
}

SvgCircle.prototype.clone=function(){
  var obj = new SvgCircle(this.getAttribute('cx'), this.getAttribute('cy'), getAttribute('r'), this.getAttribute('fill'));
  return obj;
}

// Circle
var SvgEllipse = function(x, y, rx, ry, color){
  SvgObject.call(this, 'new', 'ellipse');

  if(!color){ color='#ff00ff'; }
  var attrs = 'cx='+x+',cy='+y+',rx='+rx+',rx='+ry+',fill='+color+',stroke=#000000,stroke-width=1';
  this.setAttributes(attrs); 

  this.rx=rx;
  this.ry=ry;
  this.type="svgEllipse";
}

SvgEllipse.prototype = new SvgObject();

SvgEllipse.prototype.checkIntersection=function(x, y, ex, ey){
  var res = SvgObject.prototype.checkIntersection.call(this,x, y, ex, ey);

  if(res){
    var cx = this.getAttributeVal("cx"); 
    var cy = this.getAttributeVal("cy"); 
    var rx = this.getAttributeVal("rx"); 
    var ry = this.getAttributeVal("ry"); 

    if(cx <x && cy < y && !isIncludeEllipse(x,y,cx,cy,rx,ry)){ res=false; }
    else if(cx < x && cy > ey && !isIncludeEllipse(x,ey,cx,cy,rx,ry)){ res=false; }
    else if(cx > ex && cy > ey && !isIncludeEllipse(ex,ey,cx,cy,rx,ry)){ res=false; }
    else if(cx > ex && cy < y && !isIncludeEllipse( ex,y,cx,cy,rx,ry)){ res=false; }
  }
  return res;
}

SvgEllipse.prototype.translate=function(dx, dy){
  this.addAttributeVal("cx", dx);
  this.addAttributeVal("cy", dy);
  if(this.selectBox){ translateObject(this.selectBox, dx, dy); }
}

SvgEllipse.prototype.getObjectPos=function(){
  var x1 = this.getAttributeVal("cx");
  var y1 = this.getAttributeVal("cy");
  return new Array(x1, y1);
}

SvgEllipse.prototype.scale=function(scale, dir){
  if( dir != 'y'){
    this.setAttribute("rx", Math.round(this.rx * scale ));
  }
  if( dir != 'x'){
     this.setAttribute("ry", Math.round(this.ry * scale ));
  }
}

SvgEllipse.prototype.updateScale=function(){
  this.rx = parseInt(this.getAttribute("rx"));
  this.ry = parseInt(this.getAttribute("ry"));
}

SvgEllipse.prototype.clone=function(){
  var obj = new SvgEllipse(this.getAttribute('cx'), this.getAttribute('cy'), getAttribute('rx'),this.getAttribute('ry'), this.getAttribute('fill'));

  return obj;
}

//////////////////////////////////////
// Polygon
var SvgPolygon = function(x, y, points, color){
  SvgObject.call(this, 'new', 'polygon');

  if(!color){ color='#aaffaa'; }
  var attrs = ',fill='+color+',stroke=#000000,stroke-width=1';
  this.setAttributes(attrs); 
  if(points){
    this.setAttribute('points', points);
    this.translate(x, y);
  }
  this.type ="SvgPolygon";
}

SvgPolygon.prototype = new SvgObject();

SvgPolygon.prototype.checkIntersection=function(x, y, ex, ey){
  var points_str = this.getAttribute("points");
  if(!points_str) { return false; }
  var points = points_str.split(' ');
  var lx, ly, rx, ry, cx, cy;
  var res = false;

  for(var i=0;i < points.length ; i++){
    var pp = points[i].split(',');
    if(i==0){
      lx = rx = pp[0];
      ly = uy = pp[1];
    }else{
      if(lx > pp[0]) { lx = pp[0];}
      if(rx < pp[0]) { rx = pp[0];}
      if(ly > pp[1]) { ly = pp[1];}
      if(uy < pp[1]) { uy = pp[1];}
    }
  }
  cx = (parseFloat(lx) +  parseFloat(rx) )/ 2;
  cy = (parseFloat(ly) +  parseFloat(uy) )/ 2;

  if((x <= cx && ex >= cx && y <= cy && ey >= cy) || 
     (x >= lx && x <= rx && y >= ly && y <= uy)){
    res = true;
  }

  return res;
}

SvgPolygon.prototype.translate=function(dx, dy){
  var points=this.getAttribute("points").split(' ');
  var newPoints = new Array();
  for(var i in points){
    var pp = points[i].split(',');
    pp[0] = parseInt(pp[0]) + dx;
    pp[1] = parseInt(pp[1]) + dy;
    newPoints.push(pp.join(','));
  }
  this.setAttribute("points", newPoints.join(' '));
  if(this.selectBox){ translateObject(this.selectBox, dx, dy); }
}

SvgPolygon.prototype.getObjectPos=function(){
  var points=this.getAttribute("points").split(' ');
  var pp = points[0].split(',');
  var x1 = parseInt(pp[0]);
  var y1 = parseInt(pp[1]);
  return new Array(x1, y1);
}

SvgPolygon.prototype.scale=function(scale, dir){
}

SvgPolygon.prototype.updateScale=function(){
}

SvgPolygon.prototype.clone=function(){
  var obj = new SvgPolygon(this.getAttribute('x'), this.getAttribute('y'), this.getAttribute('points'), this.getAttribute('fill'));
  return obj;
}

SvgPolygon.prototype.setObjectPos=function(x, y){
  var points=this.getAttribute("points").split(' ');
  var p0 = points[0].split(',');
  var dx = x - p0[0];
  var dy = y - p0[1];

  var newPoints = new Array();
  for(var i in points){
    var pp = points[i].split(',');
    pp[0] = parseInt(pp[0]) + dx;
    pp[1] = parseInt(pp[1]) + dy;
    newPoints.push(pp.join(','));
  }
  this.setAttribute("points", newPoints.join(' '));
}

/////////////////////////////
// Line
var SvgLine = function(x1, y1, x2, y2, color, style, width){
  SvgObject.call(this, 'new', 'line');
  var attrs = 'x1='+x1+',y1='+y1+',x2='+x2+',y2='+y2+',fill='+style+',stroke='+color+',stroke-width='+width;
  this.setAttributes(attrs); 
  this.editMode = null;
  this.type = "SvgLine";
}

SvgLine.prototype = new SvgObject();

SvgLine.prototype.checkIntersection=function(x, y, ex, ey){
  var res = SvgObject.prototype.checkIntersection.call(this,x, y, ex, ey);

  if(res){
    var x1 = this.getAttributeVal("x1"); 
    var y1 = this.getAttributeVal("y1"); 
    var x2 = this.getAttributeVal("x2"); 
    var y2 = this.getAttributeVal("y2"); 
    var d = (y2-y1)/(x2-x1);
    var xx = Math.abs(x2-x1);
    var sign = 1;

    if(x2-x1 < 0){ sign = -1; }
    for(var i=0; i < xx; i++){
      var nx = i*sign + x1;
      var ny = d * i*sign + y1;
      if(x < nx && nx < ex && y < ny && ny < ey){ return true; }
    }
  }
  return false;
}

SvgLine.prototype.translate=function(dx, dy){
  this.addAttributeVal("x1", dx); 
  this.addAttributeVal("y1", dy); 
  this.addAttributeVal("x2", dx); 
  this.addAttributeVal("y2", dy); 
}

SvgLine.prototype.getObjectPos=function(){
  var x1 = this.getAttributeVal("x1");
  var y1 = this.getAttributeVal("y1");
  return new Array(x1, y1);
}

SvgLine.prototype.setStartPos=function(x, y){
  this.setAttribute("x1",x);
  this.setAttribute("y1",y);
}

SvgLine.prototype.setEndPos=function(x, y){
  this.setAttribute("x2",x);
  this.setAttribute("y2",y);
}

SvgLine.prototype.scale=function(scale){
  if(!scale || scale.length != 2){ return; }

  var sX=scale[0];
  var sY=scale[1];
  var x1 = parseInt(this.getAttribute("x1"));
  var y1 = parseInt(this.getAttribute("y1"));
  var x2 = parseInt(this.getAttribute("x2"));
  var y2 = parseInt(this.getAttribute("y2"));
  var bbox = this.getBBox();
  var dx = bbox.x - sX*bbox.x;
  var dy = bbox.y - sY*bbox.y;

  this.setAttribute("x1", Math.round(x1*sX)+dx);
  this.setAttribute("y1", Math.round(y1*sY)+dy);
  this.setAttribute("x2", Math.round(x2*sX)+dx);
  this.setAttribute("y2", Math.round(y2*sY)+dy);
}

SvgLine.prototype.scale2=function(scale, dir){
  var scaleX = scale;
  var scaleY = scale;
  if(dir == 'x'){ scaleY=1; }
  else if(dir == 'y'){ scaleX=1; }
  this.setScale(scaleX, scaleY);
}

SvgLine.prototype.updateScale=function(){
  var scale = this.getScale();
  if(scale){
    this.setScale(1,1);
    this.scaleSvgElement(scale);
  }
}

SvgLine.prototype.clone=function(){
  var obj = new SvgLine(this.getAttribute('x1'), this.getAttribute('y1'), this.getAttribute('x2'), this.getAttribute('y2'), this.getAttribute('stroke'), this.getAttribute('fill'), this.getAttribute('stroke-width'));
  return obj;
}

SvgLine.prototype.onMouseMove=function(e, dx, dy, x, y){
  var lx = x;
  var ly = y;

  if(!this.editMode){
    var x1 = this.getAttributeVal("x1");
    var y1 = this.getAttributeVal("y1");
    var x2 = this.getAttributeVal("x2");
    var y2 = this.getAttributeVal("y2");
    var xc = (x1+x2)/2;
    var yc = (y1+y2)/2;
    var eS = Math.min(Math.abs(x1-lx), Math.abs(y1-ly));
    var eC = Math.min(Math.abs(xc-lx), Math.abs(yc-ly));
    var eE = Math.min(Math.abs(x2-lx), Math.abs(y2-ly));

    var minVal = Math.min(eS, Math.min(eC, eE));
    if(minVal == eS){
      this.editMode = 'start';
    } else if(minVal == eE){
      this.editMode = 'end';
    }else{ this.editMode='move'; }
  }
  if(this.editMode == 'start'){
    this.setAttribute("x1",lx );
    this.setAttribute("y1",ly );
  }else if(this.editMode == 'end'){
    this.setAttribute("x2",lx );
    this.setAttribute("y2",ly );
  }else{
    this.replaceTranslate(dx, dy);
  }
}

//////////////////////////////////////////
// Path
var SvgPath = function(x1, y1, x2, y2, color, style, width){
  SvgObject.call(this, 'new', 'path');
  if(!color){ color='#000000'; }
  if(x1 === undefined){
    return;
  } 
  var attrs = 'd=M '+x1+' '+y1+' L '+x2+' '+y2+',fill='+style+',stroke='+color+',stroke-width='+width;
  this.setAttributes(attrs); 
  this.type = "SvgPath";
}

SvgPath.prototype = new SvgObject();

SvgPath.prototype.getPoints=function(){
  var res = new Array();
  var d = this.getAttribute("d"); 
  if(!d) { return res; }
  var p = d.split(' ');
  var is_x=true;
  var x, y;

  for(var i=0; i<p.length;i++){
    if(p[i].match('[MLHVCSQTA]','i')){
    }else{
      if(is_x){
        x = parseInt(p[i]);
      }else{
        y = parseInt(p[i]);
        res.push(new Array(x, y));
      }
      is_x = !is_x;
    }
  }
  return res;
}

SvgPath.prototype.checkIntersection=function(x, y, ex, ey){
  var p = this.getPoints();
  for(var i=0;i<p.length;i++){
    var ox=p[i][0];
    var oy=p[i][1];
    if(x < ox && ox < ex && y < oy && oy < ey) {
       return true;
    }
  } 
  return false;
}

SvgPath.prototype.translate=function(dx, dy){
  var d = this.getAttribute("d");
  var p = d.split(' ');
  var res = "";
  var is_x=true;
  var val;
  for(var i=0; i<p.length;i++){
    if(trim(p[i]) == "") continue;
    if(p[i].match('[MLHVCSQTA]','i')){
      res += ' '+p[i]; 
    }else{
      if(is_x){
        val = parseInt(p[i])+dx;
      }else{
        val = parseInt(p[i])+dy;
      }

      res += ' '+ val; 
      is_x = !is_x;
    }
  }

  var new_path = trim(res);
  
  this.setAttribute("d", new_path);
}

SvgPath.prototype.getObjectPos=function(){
  var path = this.getAttribute("d");
  var pp = path.split(' ');
  var x1 = parseInt(pp[0]);
  var y1 = parseInt(pp[1]);
  return new Array(x1, y1);
}

SvgPath.prototype.scale=function(scale){
  if(!scale || scale.length != 2 ){ return; }
  var path = this.getAttribute("d").split(' ');
  var bbox = this.getBBox();
  var newpath = "";
  var sX=scale[0];
  var sY=scale[1];
  var isX=true;
  var dx = bbox.x - sX*bbox.x;
  var dy = bbox.y - sY*bbox.y;

  for(var i=0;i<path.length ;i++){
    if(path[i].match(/[0-9]+/)){
      var val = parseInt(path[i]);
      if(isX){
        path[i] =  Math.round(val*sX) + dx ;
        isX = false;
      }else{
        path[i] =  Math.round(val*sY) + dy ;
        isX = true;
      }  
    }
    newpath += path[i] + ' ';
  }
  
  this.setAttribute("d",trim(newpath));
}

SvgPath.prototype.scale2=function(scale, dir){
  var scaleX = scale;
  var scaleY = scale;
  if(dir == 'x'){ scaleY=1; }
  else if(dir == 'y'){ scaleX=1; }
  this.setScale(scaleX, scaleY);
}

SvgPath.prototype.updateScale=function(){
  var scale = this.getScale();
  if(scale){
    this.setScale(1,1);
    this.scaleSvgElement(scale);
  }
}

SvgPath.prototype.clone=function(){
  var obj = new SvgObject('new', 'path');
  obj.element = this.element.cloneNode(false);
  obj.type = "SvgPath";
  return obj;
}

/////////////////////////
//
var SvgConnector = function(x1, y1, x2, y2, color, style, width){
  SvgObject.call(this, 'new', 'path');
  this.c1_flag = false;
  this.cm_flag = false;
  this.c2_flag = false;

  this.ctrlPoint = null;

  this.dir = 'we';
  this.type = "SvgConnector";

  if(!x1) { return; }
  if(!color){ color='#000000'; }
  var attrs = ',fill='+style+',stroke='+color+',stroke-width='+width;

  this.setAttributes(attrs); 

  this.update(x1,  y1, x2, y2);
}

SvgConnector.prototype = new SvgPath();

SvgConnector.prototype.setDir=function(sp, ep){
  this.dir = sp+ep;
//  console.log(this.dir);
}

SvgConnector.prototype.update=function(x1, y1, x2, y2){
  var xm1, xm2, ym1, ym2, xm, ym;
  var new_path1 = '';
  var new_path2 = '';
  var mlen=25;

  if(this.dir=='we' || this.dir == 'ew'){
    if(this.dir=='ew'){
       var tx = x1;
       var ty = y1;
       x1 = x2;
       y1 = y2;
       x2 = tx;
       y2 = ty;
    }
    xm = Math.round((x1+x2)/2);
    ym = Math.round((y1+y2)/2);

    xm1 = Math.round(x1 - Math.min(Math.max(mlen, xm/2), 20)) ;
    xm2 = Math.round(x2 + Math.min(Math.max(mlen, xm/2), 20)) ;

    ym1 = Math.round((y1 + ym)/2);
    ym2 = Math.round((y2 + ym)/2);

  }else if(this.dir=='ee'){
    xm = Math.max(x1, x2) + mlen;
    ym = Math.round((y1 + y2)/2);

    xm1 = xm; xm2 = xm; ym1 = y1; ym2 = y2;

  }else if(this.dir=='ww'){
    xm = Math.min(x1, x2) - mlen;
    ym = Math.round((y1 + y2)/2);

    xm1 = xm; xm2 = xm; ym1 = y1; ym2 = y2;
  }

  if(!this.c1_flag){ this.ctrl1=[xm1, ym1]; }
  if(!this.cm_flag){ this.ctrlm=[xm,  ym ]; }
  if(!this.c2_flag){ this.ctrl2=[xm2, ym2]; }

  
//  var new_path  = 'M ';
  if(this.dir=='we' || this.dir=='ew'){

    if(xm1 < xm2 || this.c2_flag || this.c1_flag){
      new_path1 = ' L '+this.ctrl1[0]+' '+this.ctrlm[1];
      new_path2 = ' L '+this.ctrl2[0]+' '+this.ctrlm[1];
    }else{
      new_path1 = ' L '+this.ctrlm[0]+' '+y1;
      new_path2 = ' L '+this.ctrlm[0]+' '+y2;
    }

  }else if(this.dir=='ee' || this.dir=='ww'){
    new_path1 = ' L '+this.ctrlm[0]+' '+y1;
    new_path2 = ' L '+this.ctrlm[0]+' '+y2;
  }

  var new_path  = 'M '+x1+' '+y1
      new_path += ' L '+this.ctrl1[0]+' '+y1;
      new_path += new_path1;
      new_path += ' L '+this.ctrlm[0]+' '+this.ctrlm[1];
      new_path += new_path2;
      new_path += ' L '+this.ctrl2[0]+' '+y2;
      new_path += ' L '+x2+' '+y2;

  this.setAttribute('d', new_path);
}

SvgConnector.prototype.getTerminalPoints=function(){
  var p = this.getPoints();
  if(p){
    var last = p.length-1;
    var res;
    if(this.dir == 'ew'){
      res=[ p[last], p[0] ];
    }else{
      res=[ p[0], p[last] ];
    }
    return res;
  }
  return null;
}

SvgConnector.prototype.getSelectPoints=function(){
  var p = this.getPoints();
  if(p){
    var last = p.length-1;
    var res=[ p[0], p[last] ,this.ctrlm, this.ctrl1, this.ctrl2];
    return res;
  }
  return null;
}

function onTheLine(p1, p2, x, y){ 
  var px, pn; 

  if( p1[0] == p2[0] ){
    if( p1[1] > p2[1] ){ px = p1[1]; pn = p2[1];
    }else{ px = p2[1]; pn = p1[1]; }
    if((p1[0] - x)* (p1[0] - x) < 9 &&  px > y && pn < y ){
      return true;
    }
  }else if( p1[1] == p2[1] ){
    if( p1[0] > p2[0] ){ px = p1[0]; pn = p2[0];
    }else{ px = p2[0]; pn = p1[0]; }
    if((p1[1] - y)* (p1[1] - y) < 9 &&  px > x && pn < x ){
      return true;
    }
  }
  return false;
}

SvgConnector.prototype.checkIntersection=function(x, y, ex, ey){
  var p = this.getPoints();
  var p1, p2;

  if(onTheLine(p[2], p[4], x, y)){ return true; }
  if(onTheLine(p[1], p[2], x, y)){ return true; }
  if(onTheLine(p[4], p[5], x, y)){ return true; }
  if(onTheLine(p[0], p[1], x, y)){ return true; }
  if(onTheLine(p[5], p[6], x, y)){ return true; }

  if(calcDist(this.ctrlm[0],this.ctrlm[1], x, y) < 25){ return true; }
  if(calcDist(this.ctrl1[0],this.ctrl1[1], x, y) < 25){ return true; }
  if(calcDist(this.ctrl2[0],this.ctrl2[1], x, y) < 25){ return true; }

  return false;
}

SvgConnector.prototype.setSelectBox=function(){
  try{
    var p = this.getSelectPoints();

    this.selectBox = document.createElementNS(this.svg_ns, 'g');
    this.selectBox.setAttribute('id', this.id+"-select"); 

    for(var i=0;i<p.length;i++){
      var ele=document.createElementNS(this.svg_ns, 'circle');
      var x1 = p[i][0];
      var y1 = p[i][1];
      var color="#ffff00";
      if(i<2){ color="#000000";}
      else if(i==2){ color="#ffff00";}
      else if(i==3){ color="#0fff00";}
      else if(i==4){ color="#ff0f00";}
  
      var attrs1  = 'cx='+x1+',cy='+y1;
          attrs1 +=',r=5,fill='+color+',stroke=#000000,stroke-width=1';

      setAttributes(ele, attrs1); 
      this.selectBox.appendChild(ele);
    }

  }catch(e){
    console.log(e);
    this.selectBox = null;
  }
}

SvgConnector.prototype.updateSelectBox=function(){
  var p = this.getSelectPoints();

  var nodes = this.selectBox.childNodes;

  for(var i=0;i<p.length;i++){
    var x1=p[i][0];
    var y1=p[i][1];
    var attrs = "cx="+x1+",cy="+y1;
    setAttributes(nodes[i], attrs); 
    if(i>2){
      if(p[0][0] < p[1][0] || this.c2_flag || this.c1_flag){
        nodes[i].style.display='block';
      }else{
        nodes[i].style.display='none';
      }
    }
  }
}

SvgConnector.prototype.showSelectBox=function(){
  if(!this.selectBox){ this.setSelectBox(); }
  if(this.selectBox){
    if(!this.selected){
      this.element.parentNode.insertBefore(this.selectBox, this.element);
      this.updateSelectBox();       
      this.selected = true;
    }
  }
}

SvgConnector.prototype.onMouseDown=function(e, x1, y1, x2, y2, flag){
  SvgObject.prototype.onMouseDown.call(this, e, x1, y1, x2, y2, flag);

  this.ctrlPoint = null;
  var mouse_state=e.which;

  if(calcDist(this.ctrlm[0],this.ctrlm[1], x1, y1) < 25){
    if(mouse_state == 2){
      this.cm_flag=false;
    }else{
      this.ctrlPoint = 'm';
      this.p1 = this.ctrlm[0];
      this.p2 = this.ctrlm[1];
    }
  }else if(calcDist(this.ctrl1[0],this.ctrl1[1], x1, y1) < 25){
    if(mouse_state == 2){
      this.c1_flag=false;
    }else{
      this.ctrlPoint = 'e';
      this.p1 = this.ctrl1[0];
      this.p2 = this.ctrl1[1];
    }
  }else if(calcDist(this.ctrl2[0],this.ctrl2[1], x1, y1) < 25){
    if(mouse_state == 2){
      this.c2_flag=false;
    }else{
      this.ctrlPoint = 's';
      this.p1 = this.ctrl2[0];
      this.p2 = this.ctrl2[1];
    }
  }
  return false;
}

SvgConnector.prototype.onMouseMove=function(e, dx, dy, x, y){
  if(this.ctrlPoint == 'm'){
    this.cm_flag = true;
    this.ctrlm[0] = this.p1 + dx;
    this.ctrlm[1] = this.p2 + dy;
  }else if(this.ctrlPoint == 'e'){
    this.c1_flag = true;
    this.ctrl1[0] = this.p1 + dx;
    this.ctrl1[1] = this.p2 + dy;
  }else if(this.ctrlPoint == 's'){
    this.c2_flag = true;
    this.ctrl2[0] = this.p1 + dx;
    this.ctrl2[1] = this.p2 + dy;
  }

  var pos = this.getTerminalPoints()
  this.update(pos[0][0], pos[0][1], pos[1][0], pos[1][1]);
  this.updateSelectBox();
}

SvgConnector.prototype.onMouseUp=function(e){
  var pos = this.getTerminalPoints()
  this.update(pos[0][0], pos[0][1], pos[1][0], pos[1][1]);
  this.updateSelectBox();
}

///////////////////////////////////////////
// Group
var SvgGroup = function(objs){
  SvgObject.call(this, 'new', 'g');
  this.members = new Array();
  this.type = "SvgGroup";

  this.addSvgObjects(objs);
}

SvgGroup.prototype = new SvgObject();

SvgGroup.prototype.isMember=function(obj){
  for(var i=0;i<this.members.length; i++){
    if(this.members[i] == obj){
      return true;
    }
  }  
  return false;
}

SvgGroup.prototype.addSvgObjects=function(objs){
  if(objs){
    for(var i=0;i<objs.length;i++){
      this.addSvgObject(objs[i]);
    }
  }
}

SvgGroup.prototype.addSvgObject=function(obj){
  if(obj.element.parentNode){
    obj.element.parentNode.removeChild(obj.element);
  }
  this.element.appendChild(obj.element);
  this.members.push(obj);
}

SvgGroup.prototype.addSvgElement=function(obj){
  if(obj.parentNode){
    obj.parentNode.removeChild(obj);
  }
  this.element.appendChild(obj);
  if(obj.svgobj){
    var svgobj = new SvgObject(obj.getAttribute('id'), '');
    this.members.push(svgobj);
  }
}

SvgGroup.prototype.translate=function(dx, dy){
  for(var i=0;i<this.members.length;i++){
    try{
      var obj = this.members[i];
      obj.translate(dx, dy);
    }catch(e){}
  }
  if(this.selectBox){ translateObject(this.selectBox, dx, dy); }
}

SvgGroup.prototype.getObjectPos=function(){
  if(this.members &&  this.members.length > 0){
    var obj = this.members[0];
    return obj.getObjectPos();
  }else{
    try{
      var objs = this.childNodes;
      res = getObjectPos(objs[0]);
      return res;
    }catch(e){
      return new Array(0,0);
    }
  }
}

SvgGroup.prototype.setObjectPos=function(x, y){
  var pos;
  if(this.getObjectPos){
    pos = this.getObjectPos();
  }else{
    pos = SvgGroup.prototype.getObjectPos.call(this);
  }
  var dx = x - pos[0];
  var dy = y - pos[1];
 
  if(this.translate){
    this.translate(dx, dy);
  }else{
    translateObject(this, dx, dy);
  }
}

SvgGroup.prototype.clone=function(){
  var obj = new SvgGroup([]);

  for(var i=0;i<this.members.length;i++){
    var cobj = this.members[i].clone();
    if(cobj){
      obj.addSvgObject(cobj);
    }
  }
  return obj;
}

/////////////////////////////////////////////
// Text
var SvgText=function(x, y, txt, size, color){
  this.font_width = 0.8;
  this.fontsize = size;

  SvgObject.call(this, 'new', 'text');
  if(!color){ color='#000000'; }

  y = y + size*this.font_width;
  var attrs = 'x='+x+',y='+y+',font-size='+size+',fill='+color+',stroke=none';
  this.setAttributes(attrs); 
  this.setText(txt); 
  this.type="SvgText";
}

SvgText.prototype = new SvgObject();

SvgText.prototype.setText=function(txt){
  this.element.textContent=txt;
}

SvgText.prototype.scale=function(scale){
  this.fontsize = Math.round(this.fontsize*scale);
  this.setAttribute("font-size", this.fontsize );
}

SvgText.prototype.clone=function(){
  var obj = new SvgObject('new', 'text');
  obj.element=this.element.cloneNode(true);
  obj.type="SvgText";

  return obj;
}

//////////////////////////////////////
// Image
var SvgImage=function(fname, width, height, attrs){
  SvgObject.call(this, 'new', 'image');

  this.element.setAttributeNS('http://www.w3.org/1999/xlink', 'xlink:href', fname);
  this.setAttribute('width', width);
  this.setAttribute('height', height);

  this.width =  width;
  this.height = height;

  this.setAttributes(attrs);
  this.type = "SvgImage";
}

SvgImage.prototype = new SvgObject();

SvgImage.prototype.scale=function(scale, dir){
  if( dir != 'y'){
    this.setAttribute("width", Math.round(this.width * scale ));
  }
  if( dir != 'x'){
     this.setAttribute("height", Math.round(this.height * scale ));
  }
}

SvgImage.prototype.updateScale=function(){
  this.width = parseInt(this.getAttribute("width"));
  this.height = parseInt(this.getAttribute("height"));
}

