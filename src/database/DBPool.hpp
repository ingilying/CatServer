#ifndef DBPOOL
#define DBPOOL
#include <mariadb/conncpp.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

struct DBProperty {
  std::string user;
  std::string pwd;
  std::string address;
  uint port;
  std::string db_name;
};

class DBPool {
private:
  DBProperty property;
  uint account;
  std::vector<std::unique_ptr<sql::Connection>> conntions;
  std::mutex _m_cons;
  bool error = false;

public:
  explicit DBPool(uint account, DBProperty property);
  ~DBPool();
  void Initialize();
  bool GetError();
  void Put(std::unique_ptr<sql::Connection> conn);
  std::unique_ptr<sql::Connection> Get();
};
#endif // DBPool
