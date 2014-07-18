
Window.AttachEvent("OnInit", OnInit);
Window.AttachEvent("OnSize", OnSize);
Window.AttachEvent("OnNotify", OnNotify);


function OnInit()
{
	OnSize(Window.width, Window.height);
}


function OnSize(cx, cy)
{
	ShowMyRoomDlgBk.visible = true;
	ShowMyRoomDlgBk.left = 0;
	ShowMyRoomDlgBk.top = 0;
	ShowMyRoomDlgBk.width = cx;
	ShowMyRoomDlgBk.height = cy;
	
	itemHtmlMyRoom.visible = true;
	itemHtmlMyRoom.left = 0;
	itemHtmlMyRoom.top = 0;
	itemHtmlMyRoom.width = cx;
	itemHtmlMyRoom.height = cy;
	
	imgApplyEffect.left = 0;
	imgApplyEffect.top = 0;
	imgApplyEffect.width = cx;
	imgApplyEffect.height = cy;
	
}

function OnNotify(code, code1)
{
  switch(code)
  {
  default:
    break;
  }
}
