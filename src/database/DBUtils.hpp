#ifndef DBUTILS_H
#define DBUTILS_H
#include <mariadb/conncpp.hpp>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

/*
    Kind of User:
    0: Super user
    1: Common user
    2: Channel
*/
struct DBUser {
  std::string name;
  std::string pwd;
  int kind;
};

class DBUtils {
public:
  static void CreateUserTable(std::unique_ptr<sql::Connection> &conn) {
    try {
      std::unique_ptr<sql::Statement> stmt(conn->createStatement());
      stmt->executeUpdate("create table if not exists user_info(uid INT NOT "
                          "NULL PRIMARY KEY AUTO_INCREMENT,name VARCHAR(50) "
                          "NOT NULL,password VARCHAR(200) NOT NULL,kind INT)");
      spdlog::info("Created table user_info okay!");
    } catch (sql::SQLException &e) {
      spdlog::error("DBUtils: error: {}", e.what());
    }
  };

  static void InsertUser(std::unique_ptr<sql::Connection> &conn, DBUser user) {
    try {
      std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
          "insert into user_info(name,password,kind) values(?,?,?)"));
      stmt->setString(1, user.name);
      stmt->setString(2, user.pwd);
      stmt->setInt(3, user.kind);
      stmt->executeUpdate();
    } catch (sql::SQLException &e) {
      spdlog::info("DBUtils: error: {}", e.what());
    }
  }

  static std::optional<DBUser> GetUser(std::unique_ptr<sql::Connection> &conn,
                                       uint uid) {
    try {
      std::unique_ptr<sql::PreparedStatement> stmt(
          conn->prepareStatement("select * from user_info where uid=?"));
      auto *res = stmt->executeQuery();
      DBUser user;
      while (res->next()) {
        user.name = res->getString(2);
        user.pwd = res->getString(3);
        user.kind = res->getInt(4);
      }
      return {user};
    } catch (sql::SQLException &e) {
      spdlog::info("DBUtils: error: {}", e.what());
      return std::nullopt;
    }
  }
};

#endif // !DBUTILS_H
