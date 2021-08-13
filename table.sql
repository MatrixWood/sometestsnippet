CREATE TABLE IF NOT EXISTS  t_user  (
	f_id  bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID',
	f_user_id  bigint(20) NOT NULL COMMENT '用户ID',
	f_username  varchar(64) NOT NULL COMMENT '用户名',
	PRIMARY KEY ( f_user_id ),
	INDEX f_user_id (f_user_id),
	KEY  f_id  ( f_id )
) ENGINE=InnoDB DEFAULT CHARSET=utf8

insert into t_user values (0, 0, "Jack");