myrelay
=======

copy fron http://code.taobao.org/p/myrelay/wiki/index/， and do some special changes


read client query (cli_query_cb(), change conn.state = STATE_WRITING_MYSQL)
       |
       V
send query to mysql (my_query_cb(), change conn.state = STATE_READ_MYSQL_WRITE_CLIENT)
       |
       V
read mysql result and send to client (cli_query_cb : my_answer_cb)

