/*
  canvas.js

  SVG Graphic Library

  Copyright(c) 2010-2013, Isao Hara, haraisao@gmail.com

  All rights reserved. This program is made available under the terms of the
  Eclipse Public License v1.0 which accompanies this distribution, and is
  available at http://www.eclipse.org/legal/epl-v10.html

  Contributors:  Isao Hara.

 */

////////////////
var mouse_state=0;
var modeSVG=null;

var sx=0;
var sy=0;

/// SVG Default Values
var svg_scale_dir = null;
var firstTouch = new Date();

//// for RTSE  ///////////////////
var selectedPort=null;
var selectedPort2=null;
var connectorLine=null;
var connectorList = new Array();

//////////////////////////////////

function setSvgMode(m){
  modeSVG = m;
}

function getSvgMode(){
  return modeSVG;
}

function checkTargetId(e, id){
  return (isChildById(e.target, id) || e.target.id == id);
}

function clearSelectedObjects(){
  for(var i=0; i<selectedItems.length;i++){
    var obj = selectedItems[i];
    obj.hideSelectBox();
  }
  selectedItems = new Array();
}

function getSelectedObjects(e, x1, y1, x2, y2, flag)
{
  //restruct_svg_object_list();
  var svg_objects = SvgObjectList;
  ///// swap x1,x2
  if(x1 > x2) { var tmp = x1; x1=x2; x2=tmp; } 
  if(y1 > y2) { var tmp = y1; y1=y2; y2=tmp; } 

  var val="";
  if(svg_objects){
    for(var i=0; i<svg_objects.length;i++){
      var obj = svg_objects[i]; 
      var itm = document.getElementById(obj.id);
      if(itm){
        obj.onMouseDown(e, x1, y1, x2, y2, flag);
      }else{
        array_remove(SvgObjectList, obj);
      }
    }
  }
}

///// EventHandler for iPad
function drawConnectionEditLine(e, x, y){
  if( selectedPort ){
    connectorLine = newLine(x, y);
    connectorLine.setAttribute('stroke-dasharray','2 2');
    selectedPort.setColor(Color_select);
  }
}

function finishDrawConnectionEditLine(e){
  if(selectedPort){
    if(selectedPort2){
      createConnectorFromItem(selectedPort, selectedPort2);

      selectedPort2.setColor( Color_deactive);
      selectedPort2 = null;
    }

    connectorLine.remove();
    connectorLine = null;
    selectedPort.setColor(Color_deactive);
    selectedPort = null;
  }else{
    if(connectorLine){
      connectorLine.remove();
      connectorLine = null;
    }
  }
}

function updateConnectionEditLine(e, x, y){
  if(selectedPort){
    if(connectorLine){ connectorLine.setEndPos(x, y); }
    var obj = getSelectedComponentPort(e, x, y, x, y, selectedPort);

    if(obj){
      obj.setColor(Color_mark);
      selectedPort2 = obj;
    }else{
      if(selectedPort2){
        selectedPort2.setColor(Color_deactive);
        selectedPort2 = null;
      }
    }
  }
}

function onTouchStartCore(e, sx, sy, mode){
  var flag=true;
  if(mode){  flag=false; }
  getSelectedObjects(e, sx-1, sy-1, sx+2, sy+2, flag);
}

function onTouchMoveCore(e, pageX, pageY){
  if( selectedItems.length > 0){
    var dx = pageX - sx;
    var dy = pageY - sy;
    var x = pageX;
    var y = pageY;

    for(var i=0; i<selectedItems.length;i++){
      var obj = selectedItems[i];
      obj.onMouseMove(e, dx, dy, x, y);
    }
  }
}

function onTouchEndCore(e, mode){
  for(var i=0; i<selectedItems.length;i++){
    var obj = selectedItems[i];
    obj.onMouseUp(e);
  }

  if(selectedItems.length == 0){
    updateAllSvgObjects();
  }

  showRtcMenu(selectedItems);
  finishDrawConnectionEditLine(e);

  //e.preventDefault();
  return true;
}

///////
/// for iPad
function onTouchStart(e){
  var canvas = getSvgCanvas('svg_top');

  canvas.refreshOffsets();

  sx = canvas.getX(e.touches[0].pageX);
  sy = canvas.getY(e.touches[0].pageY);

  if (e.touches.length == 1){
    // for double click
    var touchtime = new Date();
    var dt = touchtime.getTime() - firstTouch.getTime();

    if(checkTargetId(e, canvas.id)){

      if(getSvgMode() == 'addComponent'){

        var name = selectedElement.innerHTML;
        var path = selectedElement.getAttribute('path');

        add_cmp_x = sx;
        add_cmp_y = sy;

        requestComponentInfo(path, name);

        setSvgMode('selector');

      }else{
        if(dt < 300 ){
          onDoubleTap(e); 
          e.preventDefault();
          return false;
        }
        if(getSvgMode() == 'selectRtcMode'){
          onTouchStartCore(e, sx, sy, true);
        }else{
          onTouchStartCore(e, sx, sy, false);
          drawConnectionEditLine(e, sx, sy);
        }

        setSvgMode('');
      }

      firstTouch = touchtime;
      return false;

    }else{
      if(dt < 300 ){
        onDoubleTap(e); 
        return false;

      }else{ onTouchStartCore(e, sx, sy, false); }
      firstTouch = touchtime;
    }

  }else{
  }

  return true;
}

function onTouchMove(e){
  var canvas = getSvgCanvas('svg_top');
  if(!checkTargetId(e, canvas.id)) { return true; }

  var pageX = canvas.getX(e.touches[0].pageX);
  var pageY = canvas.getY(e.touches[0].pageY);

  if (e.touches.length == 1){ 
    onTouchMoveCore(e, pageX, pageY);
    updateConnectionEditLine(e, pageX, pageY);

  }else if (e.touches.length == 2){ // for Rotation

    if(selectedItems.length == 1){
      e.preventDefault();

      var dx = e.touches[0].pageX-e.touches[1].pageX;
      var dy = e.touches[0].pageY-e.touches[1].pageY;
      var th = Math.abs(Math.atan2(dy , dx)/Math.PI *180);

      if(th > 165 ||  th < 25){
        svg_scale_dir = 'x';
      }else if(th > 65 &&  th < 115){
        svg_scale_dir = 'y';
      }else{
        svg_scale_dir = null;
      }
    }

  }
}

function onTouchEnd(e){
  var canvas = getSvgCanvas('svg_top');
  if(!checkTargetId(e, canvas.id)) { return true; }

  return onTouchEndCore(e, false);
}

///// Gesture Event Handle
function onGestureStart(e){
  selectedItems[0];
}
 
function scaleGesture(obj, scale){
  if(obj.svgobj){
    obj.svgobj.scale(scale, svg_scale_dir);
  }
}

function onGestureChange(e){
  var canvas = getSvgCanvas('svg_top');

  var scale = e.scale;
  var rotation = e.rotation;

  if(selectedItems.length > 0){
    e.preventDefault();
    if(!canvas.svg_scale_locked){ scaleGesture(selectedItems[0], scale); }
    if(!canvas.svg_rotate_locked){ setRotate(selectedItems[0], rotation); }
  }
}

function onGestureEnd(e){
  if(obj.svgobj){
    obj.svgobj.updateScale();
  }
}

//////////
function onDoubleTap(e){
  var canvas = getSvgCanvas('svg_top');

  if(selectedItems.length == 1 ){

    if(selectedItems[0].getAttribute('Type') == 'RTC'){
      setSvgMode('selectRtcMode');
      displayInfoMessage( getSvgMode());
      return false; 
    }
  }else{
    if(selectedElement){
      if(selectedElement == e.target){
        displayInfoMessage("add Component...");
        setSvgMode('addComponent');
        return false;
      }
     }
  }
}

function getScaleValue(e, dx, dy){
  var th = Math.abs(Math.atan2(dy , dx)/Math.PI *180);
  var scale=1;
  var scale_dir=null;


  if(th > 165 ||  th < 25){
    scale_dir = 'x';
    scale = -dx/50; 
  }else if(th > 65 &&  th < 115){
    scale_dir = 'y';
    scale = -dy/50; 
  }else{
   scale_dir = null;

   if(dx > 0 && dy > 0){ scale = -dx*dy/2500; }
   else if(dx < 0 && dy < 0){ scale = dx*dy/2500; }
   else{ scale = 1; }
  }

  if(scale >= 0){
    if(scale < 1){ scale=1; }
  }else{
    if(scale > -1){ scale=1; }
    else{ scale= -1/scale; }
  }
  return [scale, scale_dir];
}

///// For PC

var mouse_down_flag = false;

function onMouseDown(e){
  var canvas = getSvgCanvas('svg_top');

  canvas.refreshOffsets();
  mouse_state=e.which;

  sx = canvas.getX(e.pageX);
  sy = canvas.getY(e.pageY);

  if(checkTargetId(e, canvas.id)) {
    mouse_down_flag = true;
    if(e.altKey){

    }else if(e.ctrlKey){
      resize_sx = e.pageX;
      resize_sy = e.pageY;

    }else{
      if(getSvgMode() == 'addComponent'){

        var name = selectedElement.innerHTML;
        var path = selectedElement.getAttribute('path');

        add_cmp_x = sx;
        add_cmp_y = sy;

        requestComponentInfo(path, name);

        setSvgMode('selector');

      }else{
        if(getSvgMode() == 'selectRtcMode'){
          onTouchStartCore(e, sx, sy, true);
          setSvgMode('selector')
        }else{
          onTouchStartCore(e, sx, sy, e.shiftKey);
        }
        drawConnectionEditLine(e, sx, sy);
      }
    }

    return false;
  }
 return true;
}


function onMouseMove(e){
  var canvas = getSvgCanvas('svg_top');

  if(!mouse_down_flag || !checkTargetId(e, canvas.id) ) { return true; }
  var pageX = canvas.getX(e.pageX);
  var pageY = canvas.getY(e.pageY);

  if(mouse_state == 1) {
    if(e.altKey){
      var dx = sx - pageX;
      var dy = sy - pageY;
      var scales = getScaleValue(e, dx, dy)
      var scale = scales[0];
      svg_scale_dir = scales[2];

      if(selectedItems.length > 0 && !canvas.svg_scale_locked){
        scaleGesture(selectedItems[0], scale);
      }

    }else if(e.ctrlKey){
      resize_dx = e.pageX - resize_sx;
      resize_dy = e.pageY - resize_sy;

      canvas.resize(resize_dx , resize_dy);

      resize_sx = e.pageX;
      resize_sy = e.pageY;

    }else {
      onTouchMoveCore(e, pageX, pageY);
    }
  }
  updateConnectionEditLine(e, pageX, pageY);

  return false;
}

function onMouseUp(e){
  var canvas = getSvgCanvas('svg_top');
  mouse_down_flag = false;

  if(!checkTargetId(e, canvas.id)) { return true; }

  mouse_state = 0;
  return onTouchEndCore(e, e.shiftKey);
}

function onKeyDown(e){
  var shift,ctrl,alt,keycode;
  var retval = true;

  if (!e){
    keycode = window.event.keyCore;
    shift = window.event.shiftKey;
    ctrl = window.event.ctrlKey;
    alt = window.event.altKey;
  }else{
    keycode = e.which;
    ctrl = typeof e.modifiers == 'undefined' ? e.ctrlKey : e.modifires & Event.CONTROL_MASK;
    shift = typeof e.modifiers == 'undefined' ? e.shiftKey : e.modifires & Event.SHIFT_MASK;
    alt = typeof e.modifiers == 'undefined' ? e.altKey : e.modifires & Event.ALT_MASK;
  }

  // A: 65 - Z: 90
  if(alt){
    switch(keycode){
      case 90:
        if(selectedItems.length > 0){
          if(selectedItems[0].getAttribute('Type') == 'RTC'){
            selectedItems[0].toggleDisplayName();
          }
        }
        break;
      case 88:
        if(selectedItems.length > 0){
          if(selectedItems[0].getAttribute('Type') == 'RTC'){
            selectedItems[0].deleteItem();
          }
        }
        break;
/*
      case 65:
		if(modeSVG == 'selector'){
          setSvgMode('');
		}else{
          setSvgMode('selector');
		}
        break;
*/
      case 67:
        mkConnectors('Connector');
        break;

      case 73:
        if(selectedItems.length > 0){
          if(selectedItems[0].getAttribute('Type') == 'RTC'){
	    var obj = selectedItems[0];
            dnd_setInport(e, obj.path, obj.name);
          }
        }
        break;

      case 79:
        if(selectedItems.length > 0){
          if(selectedItems[0].getAttribute('Type') == 'RTC'){
	    var obj = selectedItems[0];
            dnd_setOutport(e, obj.path, obj.name);
          }
        }
        break;

      default:
        break;
    }
    e.preventDefault();
    e.stopPropagation();
    e.returnValue=false;
  }
  return e.returnValue;
}

if(!navigator.userAgent.match("iPad") &&
   !navigator.userAgent.match("Android")){
  document.onmousedown=onMouseDown;
  document.onmousemove=onMouseMove;
  document.onmouseup=onMouseUp;
  document.ondblclick=onDoubleTap;
  document.onkeydown=onKeyDown;
}else{
  document.ontouchstart=onTouchStart;
  document.ontouchmove=onTouchMove;
  document.ontouchend=onTouchEnd;
}

