/*
	[Discuz!] (C)2001-2007 Comsenz Inc.
	This is NOT a freeware, use is subject to license terms

	$RCSfile: common.js,v $
	$Revision: 1.103 $
	$Date: 2007/07/30 09:16:52 $
*/

var lang = new Array();
var userAgent = navigator.userAgent.toLowerCase();
var is_opera = userAgent.indexOf('opera') != -1 && opera.version();
var is_moz = (navigator.product == 'Gecko') && userAgent.substr(userAgent.indexOf('firefox') + 8, 3);
var is_ie = (userAgent.indexOf('msie') != -1 && !is_opera) && userAgent.substr(userAgent.indexOf('msie') + 5, 3);

function $(id) {
	return document.getElementById(id);
}

if(typeof Array.prototype.push === 'undefined') {
Array.prototype.push = function(value) {
this[this.length] = value;
return this.length;
}
}

function checkall(form, prefix, checkall) {
	var checkall = checkall ? checkall : 'chkall';
	for(var i = 0; i < form.elements.length; i++) {
		var e = form.elements[i];
		if(e.name && e.name != checkall && (!prefix || (prefix && e.name.match(prefix)))) {
			e.checked = form.elements[checkall].checked;
		}
	}
}

function doane(event) {
	e = event ? event : window.event;
	if(is_ie) {
		e.returnValue = false;
		e.cancelBubble = true;
	} else if(e) {
		e.stopPropagation();
		e.preventDefault();
	}
}

function fetchCheckbox(cbn) {
	return $(cbn) && $(cbn).checked == true ? 1 : 0;
}

function getcookie(name) {
	var cookie_start = document.cookie.indexOf(name);
	var cookie_end = document.cookie.indexOf(";", cookie_start);
	return cookie_start == -1 ? '' : unescape(document.cookie.substring(cookie_start + name.length + 1, (cookie_end > cookie_start ? cookie_end : document.cookie.length)));
}

function thumbImg(obj) {
	var zw = obj.width;
	var zh = obj.height;
	if(is_ie && zw == 0 && zh == 0) {
		var matches
		re = /width=(["']?)(\d+)(\1)/i
		matches = re.exec(obj.outerHTML);
		zw = matches[2];
		re = /height=(["']?)(\d+)(\1)/i
		matches = re.exec(obj.outerHTML);
		zh = matches[2];
	}
	obj.resized = true;
	obj.style.width = zw + 'px';
	obj.style.height = 'auto';
	if(obj.offsetHeight > zh) {
		obj.style.height = zh + 'px';
		obj.style.width = 'auto';
	}
	if(is_ie) {
		var imgid = 'img_' + Math.random();
		obj.id = imgid;
		setTimeout('try {if ($(\''+imgid+'\').offsetHeight > '+zh+') {$(\''+imgid+'\').style.height = \''+zh+'px\';$(\''+imgid+'\').style.width = \'auto\';}} catch(e){}', 1000);
	}
	obj.onload = null;
}

function imgzoom(obj) {}

function in_array(needle, haystack) {
	if(typeof needle == 'string' || typeof needle == 'number') {
		for(var i in haystack) {
			if(haystack[i] == needle) {
					return true;
			}
		}
	}
	return false;
}

function setcopy(text, alertmsg){
	if(is_ie) {
		clipboardData.setData('Text', text);
		alert(alertmsg);
	} else if(prompt('Press Ctrl+C Copy to Clipboard', text)) {
		alert(alertmsg);
	}
}

function isUndefined(variable) {
	return typeof variable == 'undefined' ? true : false;
}

function mb_strlen(str) {
	var len = 0;
	for(var i = 0; i < str.length; i++) {
		len += str.charCodeAt(i) < 0 || str.charCodeAt(i) > 255 ? (charset == 'utf-8' ? 3 : 2) : 1;
	}
	return len;
}

function setcookie(cookieName, cookieValue, seconds, path, domain, secure) {
	var expires = new Date();
	expires.setTime(expires.getTime() + seconds);
	document.cookie = escape(cookieName) + '=' + escape(cookieValue)
		+ (expires ? '; expires=' + expires.toGMTString() : '')
		+ (path ? '; path=' + path : '/')
		+ (domain ? '; domain=' + domain : '')
		+ (secure ? '; secure' : '');
}

function strlen(str) {
	return (is_ie && str.indexOf('\n') != -1) ? str.replace(/\r?\n/g, '_').length : str.length;
}

function updatestring(str1, str2, clear) {
	str2 = '_' + str2 + '_';
	return clear ? str1.replace(str2, '') : (str1.indexOf(str2) == -1 ? str1 + str2 : str1);
}

function toggle_collapse(objname, noimg) {
	var obj = $(objname);
	obj.style.display = obj.style.display == '' ? 'none' : '';
	if(!noimg) {
		var img = $(objname + '_img');
		img.src = img.src.indexOf('_yes.gif') == -1 ? img.src.replace(/_no\.gif/, '_yes\.gif') : img.src.replace(/_yes\.gif/, '_no\.gif')
	}
	var collapsed = getcookie('discuz_collapse');
	collapsed =  updatestring(collapsed, objname, !obj.style.display);
	setcookie('discuz_collapse', collapsed, (collapsed ? 86400 * 30 : -(86400 * 30 * 1000)));
}

function trim(str) {
	return (str + '').replace(/(\s+)$/g, '').replace(/^\s+/g, '');
}

function updateseccode() {
	type = seccodedata[2];
	var rand = Math.random();
	if(type == 2) {
		$('seccodeimage').innerHTML = '<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0" width="' + seccodedata[0] + '" height="' + seccodedata[1] + '" align="middle">'
			+ '<param name="allowScriptAccess" value="sameDomain" /><param name="movie" value="seccode.php?update=' + rand + '" /><param name="quality" value="high" /><param name="wmode" value="transparent" /><param name="bgcolor" value="#ffffff" />'
			+ '<embed src="seccode.php?update=' + rand + '" quality="high" wmode="transparent" bgcolor="#ffffff" width="' + seccodedata[0] + '" height="' + seccodedata[1] + '" align="middle" allowScriptAccess="sameDomain" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" /></object>';
	} else {
		$('seccodeimage').innerHTML = '<img id="seccode" onclick="updateseccode()" width="' + seccodedata[0] + '" height="' + seccodedata[1] + '" src="seccode.php?update=' + rand + '" class="absmiddle" alt="" />';
	}
}

function updatesecqaa() {
	var x = new Ajax();
	x.get('ajax.php?action=updatesecqaa&inajax=1', function(s) {
		$('secquestion').innerHTML = s;
	});
}

function _attachEvent(obj, evt, func) {
	if(obj.addEventListener) {
		obj.addEventListener(evt, func, false);
	} else if(obj.attachEvent) {
		obj.attachEvent("on" + evt, func);
	}
}

//-----popLayer
var sPop = null;
var postSubmited = false;

document.write("<div id='popLayer' style='position:absolute;z-index:1000;visibility: hidden;filter:progid:DXImageTransform.Microsoft.shadow(direction=135,color=#CCCCCC,strength=2)' class='popupmenu_popup'></div>");

function showPopupText(event) {
        if(event.srcElement) o = event.srcElement;
        else o = event.target;
        if(!o) return;
        MouseX = event.clientX;
        MouseY = event.clientY;
        if(o.alt != null && o.alt != '') {
                o.pop = o.alt;
                o.alt = '';
        }
        if(o.title != null && o.title != '') {
                o.pop = o.title;
                o.title = '';
        }
        if(o.pop != sPop) {
                sPop = o.pop;
                if(sPop == null || sPop == '') {
                        $('popLayer').style.visibility = "hidden";
                } else {
                        popStyle = o.dyclass != null ? o.dyclass : 'popupmenu_popup';
                        $('popLayer').style.visibility = "visible";
                        showIt();
                }
        }
}

function showIt() {
        $('popLayer').className = popStyle;
        $('popLayer').innerHTML = sPop.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\n/g,"<br>");

        var bodySL, bodyST;
        if(window.pageXOffset) {
                bodySL = window.pageXOffset;
        } else if(document.documentElement && document.documentElement.scrollLeft) {
                bodySL = document.documentElement.scrollLeft;
        } else if(document.body) {
                bodySL=document.body.scrollLeft;
        }

        if(window.pageYOffset) {
                bodyST = window.pageYOffset;
        } else if(document.documentElement && document.documentElement.scrollTop) {
                bodyST = document.documentElement.scrollTop;
        } else if(document.body) {
                bodyST = document.body.scrollTop;
        }

        var bodyCW, bodyCH;
        if(window.innerWidth) {
                bodyCW = window.innerWidth;
        } else if(document.documentElement && document.documentElement.clientWidth) {
                bodyCW = document.documentElement.clientWidth;
        } else if(document.body) {
                bodyCW = document.body.clientWidth;
        }

        if(window.innerHeight) {
                bodyCH = window.innerHeight;
        } else if(document.documentElement && document.documentElement.clientHeight) {
                bodyCH = document.documentElement.clientHeight;
        } else if(document.body) {
                bodyCH = document.body.clientHeight;
        }

        if($('popLayer').clientWidth > 300) {
                var popWidth = 300;
        } else {
                var popWidth = $('popLayer').clientWidth;
        }

        var popWidth = $('popLayer').clientWidth;
        var popHeight = $('popLayer').clientHeight;
        var popLeftAdjust = MouseX + 12 + popWidth > bodyCW ? -popWidth - 24 : 0;
        var popTopAdjust = MouseY + 12 + popHeight > bodyCH ? -popHeight - 24 : 0;
        $('popLayer').style.left = (MouseX + 12 + bodySL + popLeftAdjust) + 'px';
        $('popLayer').style.top = (MouseY + 12 + bodyST + popTopAdjust) + 'px';
}

if(!document.onmouseover) {
        document.onmouseover = function(e) {
                var event = e ? e : window.event;
                showPopupText(event);
        };
}
//-----popLayer