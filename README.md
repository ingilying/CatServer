# CatServer  
CatServer 是CatTalk的服务端  
构建工具: xmake  
网络框架: workflow  
数据库: mariadb  
日志库: spdlog  
  
# 编译  
安装[xmake](https://xmake.io)  
```shell
xmake f -m release
xmake
```

xmake会自动检测并安装工具链并安装依赖  
编译产物位于: build/(编译平台)/(编译架构)/release/

# 协议 
底层基于TCP/IP协议  
内容为了简便使用JSON协议
拆包策略: 四字节长度(uint) + 对应长度内容  
说明:
    Request: 请求(一般为客户端发送)  
    Response: 回复(一般由服务端发送)  
  
账号类型:
0: 超级用户即服务器管理员
1: 普通用户
2: 频道用户(可由1创建，不可登陆，由频道管理员账户发送消息)
登陆:  
Request:  
```json
{
    "tag": "login",
    "content": {
        "uid": 10001, //UID即用户标识符
        "pwd": "mima"   //密码
    }
}
```
Response:
```json
{
    "tag": "login",
    "content": {
        "name": "test",//登陆成功返回用户名和类型，否则content为空
        "kind": 0 //账户类型
    }
}
```

