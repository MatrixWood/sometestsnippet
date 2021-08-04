#ifndef _MYSQLMGR_MYSQLTASK_H_
#define _MYSQLMGR_MYSQLTASK_H_

enum EMysqlError {
  EME_ERROR = -1,
  EME_OK,
  EME_NOT_EXIST,
  EME_EXIST,
};

class CDatabaseMysql;

class IMysqlTask {
 public:
  IMysqlTask() {}
  virtual ~IMysqlTask() {}

 private:
  virtual void execute(CDatabaseMysql* conn) = 0;
  virtual void reply() = 0;
  virtual void release() { delete this; }
};

#endif