//当前登录用户uin
var vodVarUin;

//当前登录用户是否是歌手
var vodVarIsArtist;

//当前选中麦序
var vodVarOrder;

//主播信息
var vodVarArtist;

//点歌单
var vodVarSonglist = {};

//已点歌单
var vodVarOrderList = {};

//评价歌曲
var vodVarEvaluateList;

//主播选择新增歌曲
var vodVarToAddSong;

//已点歌曲总数
var vodVarSongNum;

//歌单是否改动
var vodVarSonglistStat = 0;

//当前页码
var vodVarPage = 1;

/**
 * 当前房间全部主播列表
 * @param uid	int 当前登录用户uid
 * @param order	int 当前用户选择主播麦序
 * @param data	json 主播列表
		array(
		 	array(
				'order'		=> 主播麦序
				'RoleID'	=> 主播id,
				'RoleName'	=> 主播昵称,
				'thumb' 	=> 主播头像,
			)
		)
 * @param price	int 点歌价格，单位白兔币
 * @param renew	int 更新方式 0:更新全部，1:只更新data
 */
function vodArtist(uid, order, data, price, renew)
{
	if ('0' == renew) {
		vodVarUin = parseInt(uid);
		vodVarOrder = parseInt(order);
		if (price) {
			$('.font1.fb').text(price);
		}
	}
	vodVarArtist = eval(data);
    vodReloadArtistPage();
}

/**
 * 更换主播
 * @param order	int 麦序
 */
function vodChangeArtist(order)
{
	vodVarOrder = parseInt(order);
	vodVarPage = 1;
    vodReloadArtistPage();
    vodReloadSongListPage(0);
}

//刷新主播页面
function vodReloadArtistPage()
{
	page = '';
	for (order in vodVarArtist) {
		list = vodVarArtist[order];
		if (order == vodVarOrder) {
            page += '<li><a href="#" class="on">';
		} else {
            page += '<li><a href="javascript:;" onclick="vodChangeArtist(' + order + ');">';
        }
		page += '<img src="' + list['thumb'] + '" width="76" height="76" border="0" /><em class="nickname tc">' + list['RoleName'] + '</em></a></li>';
	}
	$('.zb_list').html(page);
}

/**
 * 返回一个主播对应歌单
 * @param order		int 主播麦序
 * @param RoleID	int 主播id
 * @param songList	json 歌单
			array(
				array(
					'song'	=> 歌曲名称,
					'hot'	=> 是否热推 0:否 1:是,
				)
			)
 * @param refresh	int 是否刷新页面，0:否，1:是
 * @param sort		int 排序方式，0:首页，1:尾页
 */
function vodSongList(order, RoleID, songList, refresh, sort)
{
	order = parseInt(order);
	RoleID = parseInt(RoleID);
    vodVarSonglist[order] = {'RoleID':RoleID, 'songList':eval(songList)};
    if ('1' == refresh) {
        //刷新页面
        vodReloadSongListPage(sort);
    }
}

/**
 * 刷新点歌单列表页面
 * @param sort	int	排序方式，0:首页，1:尾页
 */
function vodReloadSongListPage(sort)
{
    //等待页面
    $('.wrap_right').html('<div class="textPrompt"><p class="f16 fb c3">正在获取点歌单……</p></div><i class="arr' + (vodVarOrder + 1) + '"></i>');
    //更新页面
    RoleID = vodVarSonglist[vodVarOrder]['RoleID'];
    list = vodVarSonglist[vodVarOrder]['songList'];
    total = list.length;
    if (total) {
        if (RoleID == vodVarUin) {
            //主播管理
            page = '<div class="MusicList_height"><ul class="MusicList"></ul></div>';
            page += '<div class="w300 fl"><div class="pageView"></div></div><p class="fr mr30">';
			page += '<a class="btn_save_no tc f14">保存</a>';
            page += ' <a href="javascript:;" class="btn_clear tc f14" onclick="vodDropSongList()">清空</a></p><div class="bottom_music">';
            page += '<p class="ml20 mt15"><em class="f16 fb c3">添加歌曲</em> ';
			if (50 <= total) {
	            page += '<input type="text" class="input fcc" disabled/> ';
	            page += '<a class="btn_no tc fb f14">添加</a>';
			} else {
	            page += '<input type="text" maxlength="40" id="addSong" value="请输入歌曲名" class="input fcc" onFocus="vodInputOnFocus(1)" onBlur="vodInputOnBlur(1)"/> ';
	            page += '<a href="javascript:;" class="btn tc fb f14" onclick="vodAddSong()">添加</a>';
        	}
            page += '</p></div>';
            $('.wrap_right').html(page);
			if ('1' == sort) {
				//跳至尾页
				vodVarPage = 99;
			}
            vodReloadSongListAdminPage();
        } else {
            //用户点歌
            page = '<div class="MusicList_height"><ul class="MusicList"></ul></div>';
            page += '<div class="pageView"></div>';
            page += '<div class="bottom_music"><p class="ml20 mt15"><em class="f15 fb c3">自选歌曲</em> ';
  			page += '<input type="text" maxlength="40" id="addSong" value="请输入您要点的歌曲名" class="input fcc" onFocus="vodInputOnFocus(2)" onBlur="vodInputOnBlur(2)"/> <a href="javascript:;" class="btn_music tc fb f14" onclick="vodOrderSong(0)">点歌</a> <a href="javascript:;" class="btn_chabo tc fb f14" onclick="vodOrderSong(1)">插播</a></p></div>';
            $('.wrap_right').html(page);
            vodReloadSongListUserPage();
        }
    } else {
        //无数据
        if (RoleID == vodVarUin) {
            //主播添加
            if (vodVarToAddSong) {
                page = '<div class="addMusic"><p class="title fb f16 c3">添加歌曲</p><input type="text" maxlength="40" id="addSong" value="请输入歌曲名" class="input fcc" onFocus="vodInputOnFocus(1)" onBlur="vodInputOnBlur(1)"/> <a href="javascript:;" class="btn tc fb f14" onclick="vodAddSong()">添加</a></div>';
            } else {
                page = '<div class="textPrompt"><p class="f16 fb c3">您还没有添加任何歌曲！</p><p class="f14 fb c3">点击这里 <a href="javascript:;" onclick="vodToAddSong()">新增歌曲</a></p></div>';
            }
        } else {
            page = '<div class="addMusic"><p class="title fb f16 c3">TA还没有编辑歌单~</p><input type="text" maxlength="40" id="addSong" value="请输入您要点的歌曲名" class="input fcc" onFocus="vodInputOnFocus(2)" onBlur="vodInputOnBlur(2)"/> <a href="javascript:;" class="btn_music tc fb f14" onclick="vodOrderSong(0)">点歌</a> <a href="javascript:;" class="btn_chabo tc fb f14" onclick="vodOrderSong(1)">插播</a></div>';
        }
        $('.wrap_right').html(page);
    }
    //当前选中标记
    $('.wrap_right').append('<i class="arr' + (vodVarOrder + 1) + '"></i>');
}

//输入框选中效果
function vodInputOnFocus(type)
{
	if (1 == type) {
		tip = '请输入歌曲名';
	} else {
		tip = '请输入您要点的歌曲名';
	}
	if (tip == $('#addSong').val()) {
		$('#addSong').val('');
		$('#addSong').attr('class', 'input');
	}
}

//输入框移出效果
function vodInputOnBlur(type)
{
	if (1 == type) {
		tip = '请输入歌曲名';
	} else {
		tip = '请输入您要点的歌曲名';
	}
	song = $('#addSong').val();
	if ('' == song || tip == song) {
		$('#addSong').val(tip);
		$('#addSong').attr('class', 'input fcc');
	}
}

//刷新主播管理点歌单列表页面
function vodReloadSongListAdminPage()
{
    list = vodVarSonglist[vodVarOrder]['songList'];
    index = vodSongListSplitData();
    page = '';
    //最多可热推5首歌 (当已推荐5首歌，其余推荐按钮均变为不可编辑状态）
    totalHot = 0;
    end = list.length - 1;
    for (i in list) {
    	if (list[i]['hot']) {
    		totalHot++;
    	}
    }
    for (i = index['begin']; i <= index['end']; i++) {
        page += '<li id="song_' + i + '"><span class="fl musicName_width">';
        if (9 > parseInt(i)) page += '0';
        page += (1 + parseInt(i)) + '. ' + list[i]['song'] + '</span><span class="fr vm">';
        page += '<input type="checkbox" onclick="vodSongListHot(' + i + ')"';
        if (list[i]['hot']) {
            page += ' checked';
        } else if (4 < totalHot) {
        	page += ' disabled';
        }
        page += '/> 热推';
        if (0 == i) {
        	page += '<a class="ml10 mr10 dtop_no">置顶</a>';
        	page += '<a class="icon_up_no"></a>';
	    } else {
        	page += '<a href="javascript:;" class="ml10 mr10 dtop" onclick="vodSongListTop(' + i + ')">置顶</a>';
        	page += '<a href="javascript:;" class="icon_up" onclick="vodSongListUp(' + i + ')"></a>';
	    }
	    if (end == i) {
	    	page += '<a class="icon_down_no"></a>';
	    } else {
        	page += '<a href="javascript:;" class="icon_down" onclick="vodSongListDown(' + i + ')"></a>';
        }
        if (0 == end) {
        	page += '<a class="icon_del_no"></a>';
        } else {
        	page += '<a href="javascript:;" class="icon_del" onclick="vodSongListDel(' + i + ')"></a>';
        }
        page += '</span></li>';
    }
    $('.MusicList').html(page);
}

//刷新用户点歌单列表页面
function vodReloadSongListUserPage()
{
    list = vodVarSonglist[vodVarOrder]['songList'];
    index = vodSongListSplitData();
    page = '';
    for (i = index['begin']; i <= index['end']; i++) {
        page += '<li><span class="fl musicName_width">';
        if (9 > parseInt(i)) page += '0';
        page += (1 + parseInt(i)) + '. ' + list[i]['song'];
        if (list[i]['hot']) {
        	page += ' <em class="hot vm"></em>';
        }
        page += '</span><span class="fr vm"><a href="javascript:;" class="hill-seeding mr10" onclick="vodOrderSong(0, ' + i + ')"></a><a href="javascript:;" class="hill-insert" onclick="vodOrderSong(1, ' + i + ')"></a></span></li>';
    }
    $('.MusicList').html(page);
}

/**
 * 返回分页数据索引并更新分页页面
* @return json
	array(
		'begin'	=> 起始索引
		'end'	=> 末尾索引
	)
 */
function vodSongListSplitData()
{
	//每页8条
	perPage = 8;
	if (1 > vodVarPage) {
		vodVarPage = 1;
	}
	total = vodVarSonglist[vodVarOrder]['songList'].length;
	totalPage = Math.ceil(total / perPage);
	if (totalPage < vodVarPage) {
		vodVarPage = totalPage;
	}
	begin = (vodVarPage - 1) * perPage;
	end = begin + perPage - 1;
	if (total - 1 < end) {
		end = total - 1;
	}
	if (perPage < total) {
		//更新分页页面
		splitPage = vodAjaxPage('vodSongListTurnPage', total, vodVarPage, perPage);
		$(".pageView").html(splitPage);
	}
	return {'begin': begin, 'end':end};
}

//翻页响应
function vodSongListTurnPage(page)
{
	vodVarPage = page;
    if (vodVarSonglist[vodVarOrder]['RoleID'] == vodVarUin) {
        vodReloadSongListAdminPage();
    } else {
        vodReloadSongListUserPage();
    }
}

/**
 * 主播删除一首歌
 * @param index	int	歌曲索引
 */
function vodSongListDel(index)
{
	if (1 == vodVarSonglist[vodVarOrder]['songList'].length) {
		return false;
	}
    vodVarSonglist[vodVarOrder]['songList'].splice(index, 1);
    vodReloadSongListAdminPage();
    vodSongListStatChange();
    $(".pageView").html('');
    vodSongListSplitData();
}

/**
 * 主播上移一首歌
 * @param index	int	歌曲索引
 */
function vodSongListUp(index)
{
    if (0 == index) {
        return false;
    }
    cur = vodVarSonglist[vodVarOrder]['songList'][index];
    vodVarSonglist[vodVarOrder]['songList'][index] = vodVarSonglist[vodVarOrder]['songList'][index - 1];
    vodVarSonglist[vodVarOrder]['songList'][index - 1] = cur;
    vodReloadSongListAdminPage();
    vodSongListStatChange();
}

/**
 * 主播下移一首歌
 * @param index	int	歌曲索引
 */
function vodSongListDown(index)
{
    if (vodVarSonglist[vodVarOrder]['songList'].length - 1 == index) {
        return false;
    }
    cur = vodVarSonglist[vodVarOrder]['songList'][index];
    vodVarSonglist[vodVarOrder]['songList'][index] = vodVarSonglist[vodVarOrder]['songList'][index + 1];
    vodVarSonglist[vodVarOrder]['songList'][index + 1] = cur;
    vodReloadSongListAdminPage();
    vodSongListStatChange();
}

/**
 * 主播置顶一首歌
 * @param index	int	歌曲索引
 */
function vodSongListTop(index)
{
    if (0 == index) {
        return false;
    }
    tmp = vodVarSonglist[vodVarOrder]['songList'];
    vodVarSonglist[vodVarOrder]['songList'] = [];
    vodVarSonglist[vodVarOrder]['songList'].push(tmp[index]);
    for (i in tmp) {
        if (i != index) {
            vodVarSonglist[vodVarOrder]['songList'].push(tmp[i]);
        }
    }
    vodReloadSongListAdminPage();
    vodSongListStatChange();
}

/**
 * 主播热推一首歌
 * @param index	int	歌曲索引
 */
function vodSongListHot(index)
{
    if (1 == vodVarSonglist[vodVarOrder]['songList'][index]['hot']) {
        vodVarSonglist[vodVarOrder]['songList'][index]['hot'] = 0;
    } else {
        vodVarSonglist[vodVarOrder]['songList'][index]['hot'] = 1;
    }
    vodReloadSongListAdminPage();
    vodSongListStatChange();
}

//主播选择新增歌曲
function vodToAddSong()
{
    vodVarToAddSong = 1;
    vodReloadSongListPage();
}

//保存歌单
function vodSaveSongList()
{
	window.external.vod_saveSongList(vodVarSonglist[vodVarOrder]['RoleID'], $.toJSON(vodVarSonglist[vodVarOrder]['songList']));
}

//主播添加歌曲
function vodAddSong()
{
	if ('input fcc' == $('#addSong').attr('class')) {
		song = '';
	} else {
		song = $('#addSong').val();
	}
    if ('' == song) {
        alert('歌曲名不能为空');
        exit;
    } else if (20 < song.length) {
    	tmp = song;
    	if (40 < tmp.replace(/[^\x00-\xff]/g, "**").length) {
    		alert('歌名过长！');
    		exit;
    	}
    }
	window.external.vod_addSong(vodVarSonglist[vodVarOrder]['RoleID'], song);
}

//清空歌单
function vodDropSongList()
{
	window.external.vod_dropSongList(vodVarSonglist[vodVarOrder]['RoleID']);
}

/**
 * 用户点播歌曲
 * @option param type int 操作方式，0:点播，1:插播
 * @option param index int 歌曲索引，缺省取输入歌名
 */
function vodOrderSong(type, index)
{
    if (-1 < index) {
        song = vodVarSonglist[vodVarOrder]['songList'][index]['song'];
    } else {
		if ('input fcc' == $('#addSong').attr('class')) {
			song = '';
		} else {
			song = $('#addSong').val();
		}
    }
    if ('' == song) {
        alert('歌曲名不能为空');
        exit;
    } else if (20 < song.length) {
    	tmp = song;
    	if (40 < tmp.replace(/[^\x00-\xff]/g, "**").length) {
    		alert('歌名过长！');
    		exit;
    	}
    }
    if (1 == type) {
		window.external.vod_orderPriority(vodVarSonglist[vodVarOrder]['RoleID'], song);
	} else {
		window.external.vod_orderSong(vodVarSonglist[vodVarOrder]['RoleID'], song);
	}
}

/**
 * 歌单是否有改动
 * @call param stat int 0:未改动，1:有改动
 */
function vodSongListChange()
{
	return vodVarSonglistStat;
}

//改动歌单状态
function vodSongListStatChange()
{
	$('.btn_save_no').replaceWith('<a href="javascript:;" class="btn_save tc f14" onclick="vodSaveSongList()">保存</a>');
	vodVarSonglistStat = 1;
}

//重置歌单改动状态
function vodSongListStatClear()
{
	vodVarSonglistStat = 0;
}

/**
 * 已点歌曲主屏
 *
 * @param uid    当前登录用户uid
 * @param artist 是否为歌手, 0：普通用户, 1：歌手
 * @param num    当前房间已点歌曲总数
 * @param data   歌曲列表（json格式）
 *       array(
 *			array(
 *              'orderID'	=> 订单id,
 *				'RoleID'	=> 主播id,
 *				'RoleName'	=> 主播昵称,
 *				'UserID'	=> 点播粉丝id，
 *				'UserName'	=> 点播粉丝昵称，
 *				'time'		=> 时间,
 *				'song'		=> 歌曲名称,
 *				'wish'		=> 点播赠言,
 *              'priority'  => 插播费,
 *	     		'stat'		=> 状态，0：待处理，1：已同意，2：已拒绝,
 *              'evaluate'	=> 评价，0：待评，1：弃评，2：差评，3：好评, 4：为空,
 *			)
 *  	)
 * @return null
 */
function vodOrderedList(uid, artist, num, data) {
	vodVarUin       = parseInt(uid);
	vodVarIsArtist  = parseInt(artist);
	vodVarSongNum   = parseInt(num);
	vodVarOrderList = eval(data);

	showOrderListInfo(0);
}

/**
 * 主播处理已点歌曲，操作成功后回调
 *
 * @param orderID 订单id
 * @param stat   处理办法，0：拒绝，1：同意
 * @return null
 */
function vodDecideSongResult(orderID, stat) {
	switch( parseInt(stat) ){
		case 0:
			$("#orderId_"+orderID).html('<em class="btn_yes">已拒绝</em>');
			break;
		case 1:
			$("#orderId_"+orderID).html('<em class="btn_yes">已同意</em>');
			break;
		default :
			break;
	}
}

/**
 * 主播处理已点歌曲
 *
 * @param orderID 订单id
 * @param stat   处理办法，0：拒绝，1：同意
 * @return null
 */
function vodDecideSong(orderID, stat) {
	window.external.vod_decideSong(orderID, stat);
}

/**
 * 歌曲评价
 *
 * @param num    当前需要评价的歌曲总数
 * @param data   歌曲列表（json格式）
 *       array(
 *			array(
 *              'orderID'	=> 订单id,
 *				'RoleID'	=> 主播id,
 *				'RoleName'	=> 主播昵称,
 *				'time'		=> 时间,
 *				'song'		=> 歌曲名称,
 *			)
 *  	)
 * @return null
 */
function vodEvaluateList(num, data) {
	vodVarSongNum = parseInt(num);
	if(vodVarSongNum > 0){
		vodVarEvaluateList = eval(data);
		showEvaluateListInfo(0);
	}else{
		showEmptyOfEvaluate();
	}
}

/**
 * 点播者评价歌曲
 *
 * @param orderID 订单id
 * @param stat   处理办法，1：弃评，2：差评，3：好评
 * @return null
 */
function vodDecideEvaluate(orderID, stat) {
	window.external.vod_decideEvaluate(orderID, stat);
}

// 切换到点歌页面
function vodOpenOrderSong() {
	window.external.vod_openOrderSong();
}

/**
 * 返回分页导航
 *
 * @param ajaxMethod ajax方法时，传的是function名
 * @param totalRows  总记录的条数
 * @param curPage    当前的页码
 * @param perPage    每页的条数
 * @return string
 */
function vodAjaxPage(ajaxMethod, totalRows, curPage, perPage) {

	var allPageNum = Math.ceil(totalRows / perPage);
	if(allPageNum < 1){
		return '';
	}

	var pageHtml = '<ul class="pageList f14">';

	if (curPage > 1) {
		pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ (curPage - 1) +')" class="first"></a></li>';
	} else {
		pageHtml += '<li><a href="javascript:void(0);" class="first"></a></li>';
	}

	if (allPageNum <= 5) {

		for (var i = 1; i <= allPageNum; i++) {
			if( i == curPage ){
				pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
			}else{
				pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +')" class="num">'+ i +'</a></li>';
			}
		}

	} else {

		if (curPage == 1 || curPage == 2) {

			for (var i = 1; i < 4; i++) {
				if(i == curPage){
					pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
				}else{
					pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +')" class="num">'+ i +'</a></li>';
				}
			}

			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ allPageNum +');" class="num">'+ allPageNum +'</a></li>';

		} else if (curPage >= allPageNum || curPage == (allPageNum - 1)) {

			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'(1);" class="num">1</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';

			for (var i = allPageNum - 2; i <= allPageNum; i++) {
				if(i == curPage){
					pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
				}else{
					pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +');" class="num">'+ i +'</a></li>';
				}
			}

		} else if (curPage == 3) {

			for (var i = 1; i <= 4; i++) {
				if(i == curPage){
					pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
				}else{
					pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +');" class="num">'+ i +'</a></li>';
				}
			}

			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ allPageNum +');" class="num">'+ allPageNum +'</a></li>';

		} else if (curPage == (allPageNum - 2)) {

			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'(1);" class="num">1</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';

			for (var i = allPageNum - 3; i <= allPageNum; i++) {
				if(i == curPage){
					pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
				}else{
					pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +');" class="num">'+ i +'</a></li>';
				}
			}

		} else {

			var from = curPage - 1;
			var get = curPage + 1;

			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'(1);" class="num">1</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';

			for (var i = from; i <= get; i++) {
				if(i == curPage){
					pageHtml += '<li><a href="javascript:void(0);" class="num-on">'+ i +'</a></li>';
				}else{
					pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ i +');" class="num">'+ i +'</a></li>';
				}
			}

			pageHtml += '<li><a href="javascript:void(0);" class="num">...</a></li>';
			pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ allPageNum +');" class="num">'+ allPageNum +'</a></li>';

		}

	}

	if(curPage < allPageNum){
		pageHtml += '<li><a href="javascript:void(0);" onclick="'+ ajaxMethod +'('+ (curPage + 1) +');" class="last"></a></li>';
	}else{
		pageHtml += '<li><a href="javascript:void(0);" class="last"></a></li>';
	}

	pageHtml += '</ul>';

	return pageHtml;
}

//禁用鼠标右键、键盘刷新等
$(document).bind('contextmenu', function(e){return false;});

document.ondragstart = function(e){
	window.event.returnValue=false;
}
document.onkeydown = function(e){
    e = window.event || e;
    var keycode = e.keyCode || e.which;
    if (e.ctrlKey && (67 == keycode || 86 == keycode || 88 == keycode)) {
    	return true;
	} else if (e.ctrlKey || e.altKey || e.shiftKey || keycode >= 112 && keycode <= 123){
        if(window.event){// ie
            try{e.keyCode = 0;}catch(e){}
            e.returnValue = false;
        }else{// ff
            e.preventDefault();
        }
    }
}