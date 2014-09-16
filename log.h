/**
  *  Author : WANG FENG
  *    File : mylog.h
  *    Date : 2014年 09月 16日 星期二 15:29:53 CST
  *    Desc :
  * History :1.created on 2014年 09月 16日 星期二 15:29:53 CST by WANG FENG
  */
#ifndef __MYLOG_H__
#define __MYLOG_H__

#define INFO(fmt, args...) do{ fprintf(stdout, "%s %d INFO %s\n", __FILE__, __LINE__, ##args); }while(0);
#define ERROR(fmt, args...) do{ fprintf(stderr, "%s %d ERROR %s\n", __FILE__, __LINE__, ##args); }while(0);
#define WARN(fmt, args...) do{ fprintf(stdout, "%s %d WARN %s\n", __FILE__, __LINE__, ##args); }while(0);

#endif    // #ifndef __MYLOG_H__

