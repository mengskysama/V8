
var SKIN_ON_CLICK_CLOSE = 1;
var SKIN_ON_CLICK_SEND = 2;
var SKIN_ON_CLICK_COLOR = 3;
var SKIN_ON_CLICK_INSERTROOM = 4;
var SKIN_ON_CLICK_EMOTION = 5;
var SKIN_ON_CLICK_WISH = 6;


Window.AttachEvent("OnInit", OnInit);
Window.AttachEvent("OnSize", OnSize);
Window.AttachEvent("OnNotify", OnNotify);

btnSysClose.AttachEvent("OnClick", btnCloseOnClick);
btnColor.AttachEvent("OnClick", btnColorOnClick);
btnInsertRoom.AttachEvent("OnClick", btnInsertRoomClick);
btnEmotion.AttachEvent("OnClick", btnEmotionClick);
//btnWish.AttachEvent("OnClick", btnWishClick);

op_send.AttachEvent("OnClick", OnClickSend);
op_cancel.AttachEvent("OnClick", btnCloseOnClick);


function btnCloseOnClick()
{
	Window.PostSkinMessage(SKIN_ON_CLICK_CLOSE, 0);
}

function OnClickSend()
{
	Window.PostSkinMessage(SKIN_ON_CLICK_SEND, 0);
}

function btnColorOnClick()
{
	Window.PostSkinMessage(SKIN_ON_CLICK_COLOR, 0);
}

function btnInsertRoomClick()
{
	Window.PostSkinMessage(SKIN_ON_CLICK_INSERTROOM, 0);
}

function btnEmotionClick()
{
	Window.PostSkinMessage(SKIN_ON_CLICK_EMOTION, 0);
}

// function btnWishClick()
// {
	// Window.PostSkinMessage(SKIN_ON_CLICK_WISH, 0);
// }



function OnInit()
{
	textTitle.text = "小喇叭";
    textTitle.visible = false;

	OnSize(Window.width, Window.height);
}

function OnSize(cx, cy)
{	
    imgTopBk.left = 2;
	imgTopBk.top = 2;
	imgTopBk.width = cx - 4;
	imgTopBk.height = 32;

	imgMidBk.left = 2;
	imgMidBk.top = imgTopBk.top + imgTopBk.height;
	imgMidBk.width = cx - 4;
	imgMidBk.height = cy - 2 - imgMidBk.top;
	
	textTheTitle.left = 10;
	textTheTitle.top = 8;
	textTheTitle.width = textTheTitle.textWidth;
	textTheTitle.height = textTheTitle.textHeight;
	
	op_cancel.visible = true;
	//op_cancel.width = 79;
	//op_cancel.height = 35;
	op_cancel.left = cx - 10 - op_cancel.width;
	op_cancel.top = cy - 38;
	
	op_send.visible = true;
	//op_send.width = op_cancel.width;
	//op_send.height = op_cancel.height;
	op_send.left = op_cancel.left - 20 - op_send.width;
	op_send.top = op_cancel.top;
	/////////////////////////////////////////////////
	textTip.visible = true;
	textTip.left = textTheTitle.left;
	textTip.top = 43;
	textTip.width = textTip.textWidth;
	textTip.height = textTip.textHeight;
	
	radioSmallSpeaker.visible = true;
	radioSmallSpeaker.left =  40;
	radioSmallSpeaker.top = 60;
	radioSmallSpeaker.width = 60;
	radioSmallSpeaker.height = 16;
	
	radioBigSpeaker.visible = true;
	radioBigSpeaker.left = radioSmallSpeaker.left + radioSmallSpeaker.width + 73;
	radioBigSpeaker.top = radioSmallSpeaker.top;
	radioBigSpeaker.width = radioSmallSpeaker.width;
	radioBigSpeaker.height = radioSmallSpeaker.height;
	
	radioSuperSpeaker.visible = true;
	radioSuperSpeaker.left = radioBigSpeaker.left + radioBigSpeaker.width + 67;
	radioSuperSpeaker.top = radioBigSpeaker.top;
	radioSuperSpeaker.width = 180;
	radioSuperSpeaker.height = radioBigSpeaker.height;
	
	imgTip.visible = true;
	imgTip.left =  8;
	imgTip.top = radioSmallSpeaker.top + radioSmallSpeaker.height + 6;
	imgTip.width = 403;
	imgTip.height = 33;
	
	imgTipMid.left = imgTip.left;
	imgTipMid.top = imgTip.top;
	imgTipMid.width = imgTip.width;
	imgTipMid.height = imgTip.height;
	
	imgTipRight.left = imgTipMid.left;
	imgTipRight.top = imgTipMid.top;
	imgTipRight.width = imgTipMid.width;
	imgTipRight.height = imgTipMid.height;
	
	textTip1.visible = true;
	textTip1.width = imgTip.width - 40;
	textTip1.height = 17;
	textTip1.left = imgTip.left + 20;
	textTip1.top = imgTip.top + (imgTip.height - textTip1.height)/2 + 2;
	
	// textTip2.visible = false;
	// textTip2.width = textTip2.textWidth;
	// textTip2.height = textTip2.textHeight;
	// textTip2.left = imgTip.left + imgTip.width - 85 - textTip2.width;
	// textTip2.top = textTip1.top;
	
	imgInputBound.visible = true;
	imgInputBound.left = imgTip.left;
	imgInputBound.top = imgTip.top + imgTip.height + 8;
	imgInputBound.width = imgTip.width + 1;
	imgInputBound.height = 142;
	
	imgBtnBk.visible = true;
	imgBtnBk.left = imgInputBound.left + 1;
	imgBtnBk.top = imgInputBound.top + 1;
	imgBtnBk.width = imgInputBound.width - 2;
	imgBtnBk.height = 34;
	
	btnEmotion.visible = true;
	btnEmotion.left = imgBtnBk.left;
	btnEmotion.top = imgBtnBk.top;
	btnEmotion.width = 134;
	btnEmotion.height = imgBtnBk.height;
	
	// btnWish.visible = true;
	// btnWish.left = btnEmotion.left + btnEmotion.width;
	// btnWish.top = btnEmotion.top;
	// btnWish.width = btnEmotion.width;
	// btnWish.height = btnEmotion.height;
	
	btnColor.visible = true;
	btnColor.left = btnEmotion.left + btnEmotion.width;
	btnColor.top = btnEmotion.top;
	btnColor.width = 132;
	btnColor.height = btnEmotion.height;
	
	btnInsertRoom.visible = true;
	btnInsertRoom.left = btnColor.left + btnColor.width;
	btnInsertRoom.top = btnColor.top;
	btnInsertRoom.width = 136;
	btnInsertRoom.height = btnColor.height;
    
	itemHtmlSpeakerInput.visible = true;
	itemHtmlSpeakerInput.left = imgBtnBk.left;
	itemHtmlSpeakerInput.top = imgBtnBk.top + imgBtnBk.height + 1;
	itemHtmlSpeakerInput.width = imgBtnBk.width;
	itemHtmlSpeakerInput.height = 100;
	
	textPrompt.visible = true;
	textPrompt.left = imgInputBound.left + 20;
	textPrompt.top = imgInputBound.top + imgInputBound.height + 2;
	textPrompt.width = textPrompt.textWidth;
	textPrompt.height = textPrompt.textHeight;
	
	//textPrompt2.visible = true;
	//textPrompt2.left = textPrompt1.left + textPrompt1.width;
	//textPrompt2.top = textPrompt1.top;
	//textPrompt2.width = textPrompt2.textWidth;
	//textPrompt2.height = textPrompt2.textHeight;
	
	//textPrompt3.visible = true;
	//textPrompt3.left = textPrompt2.left + textPrompt2.width;
	//textPrompt3.top = textPrompt2.top;
	//textPrompt3.width = textPrompt3.textWidth;
	//textPrompt3.height = textPrompt3.textHeight;
	
	textEmotionPrompt.visible = false;
	textEmotionPrompt.left = textPrompt.left;
	textEmotionPrompt.top = textPrompt.top + 20;
	textEmotionPrompt.width = textEmotionPrompt.textWidth;
	textEmotionPrompt.height = textEmotionPrompt.textHeight;	

}

function OnNotify(code, code1)
{
	
}
