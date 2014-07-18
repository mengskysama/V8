#pragma  once

/**
 * 初始相关     
 */
extern INT32 handle_init(INT32 taskid, const TaskInfo* taskinfo);

/**
 * HTTP 头被读取结束后     
 */
extern INT32 handle_headcomplete(INT32 taskid, const TaskInfo* taskinfo);

/**
 * 读取HTTP数据的过程     
 */
extern INT32 handle_process(INT32 taskid, const TaskInfo* taskinfo);

/**
 * 数据读取结束     
 */
extern INT32 handle_finish(INT32 taskid, const TaskInfo* taskinfo);

/**
 * 读取的时候发生了错误     
 */
extern INT32 handle_error(INT32 taskid, const TaskInfo* taskinfo);

extern off_t FileCompare(HTTPCONTEXT* pContext);