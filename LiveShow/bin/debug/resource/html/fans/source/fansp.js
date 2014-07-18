function switchTab(ProTag, ProBox) {
	for (i = 1; i < 4; i++) {
		if ("tab" + i == ProTag) {
			document.getElementById(ProTag).getElementsByTagName("a")[0].className = "on";
		} else {
			document.getElementById("tab" + i).getElementsByTagName("a")[0].className = "";
		}
		if ("con" + i == ProBox) {
			document.getElementById(ProBox).style.display = "";
		} else {
			document.getElementById("con" + i).style.display = "none";
		}
	}
}

function AnchorFansToday(tipmsg, ranklist){
	setAnchorFansInfo(tipmsg, ranklist, "dayFans");
}

function AnchorFansMonth(tipmsg, ranklist){
	setAnchorFansInfo(tipmsg, ranklist, "monthFans");
}

function AnchorFansSuper(tipmsg, ranklist){
	setAnchorFansInfo(tipmsg, ranklist, "superFans");
}

function setAnchorFansInfo(tipmsg, ranklist, toId){
	var classNameStr = "";
	var htmlStr = "";
	if(tipmsg){
		htmlStr = '<div class="go">'+tipmsg+'</div>';
	}else{
		ranklist = eval(ranklist);
		
		for(var item in ranklist){
			htmlStr += '<ul class="list">';
			htmlStr += '<li class="i1"></li>';
			htmlStr += '<li class="i2">'+ranklist[item].name+'</li>';
			htmlStr += '<li class="i3">'+ranklist[item].value+'</li>';
			htmlStr += '</ul>';
		}
		
		htmlStr = '<div class="data">'+ htmlStr +'</div>';
	}
	document.getElementById(toId).innerHTML = htmlStr;
}

document.oncontextmenu = function(e){
	window.event.returnValue=false;
}
document.ondragstart = function(e){
	window.event.returnValue=false;
}
document.onkeydown = function(e){
    e = window.event || e;
    var keycode = e.keyCode || e.which;
    if(e.ctrlKey || e.altKey || e.shiftKey
        || keycode >= 112 && keycode <= 123){
        if(window.event){// ie
            try{e.keyCode = 0;}catch(e){}
            e.returnValue = false;
        }else{// ff
            e.preventDefault();
        }
    }
}