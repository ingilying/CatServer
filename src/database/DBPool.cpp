#include "DBPool.hpp"
#include <algorithm>
#include <chrono>
#include <spdlog/spdlog.h>
#include <thread>
using namespace std::chrono_literals;

DBPool::DBPool(uint account, DBProperty property) {
  this->property = std::move(property);
  this->account = account;
}

DBPool::~DBPool() {
  std::lock_guard<std::mutex> lock(_m_cons);
  for (auto &conn : conntions) {
    conn->close();
    spdlog::info("DataBasePool: close a connection");
  }
  conntions.clear();
}

void DBPool::Initialize() {
  if (conntions.empty()) {
    auto driver = sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://" + this->property.address + ":" +
                       std::to_string(this->property.port) + "/" +
                       this->property.db_name);
    sql::Properties properties({{"user", property.user},
                                {"password", property.pwd},
                                {"autoReconnect", "true"}});
    for (int i = 0; i < account; i++) {
      std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));
      if (!conn) {
        spdlog::info("The fucking connection is error!");
        this->error = true;
        return;
      } else {
        conntions.push_back(std::move(conn));
        spdlog::info("DataBasePool: a new connection");
      }
    }
  }
}

bool DBPool::GetError() { return this->error; }

void DBPool::Put(std::unique_ptr<sql::Connection> conn) {
  std::lock_guard<std::mutex> lock(_m_cons);
  conntions.push_back(std::move(conn));
}

std::unique_ptr<sql::Connection> DBPool::Get() {
  bool empty;
  {
    std::lock_guard<std::mutex> lock(_m_cons);
    empty = conntions.empty();
  }
  if (empty) {
    while (true) {
      std::this_thread::sleep_for(100ms);
      std::lock_guard<std::mutex> lock(_m_cons);
      if (!conntions.empty()) {
        break;
      }
    }
  }
  std::lock_guard<std::mutex> lock(_m_cons);
  auto conn = std::move(conntions.back());
  conntions.pop_back();
  spdlog::info("Get a connection");
  return conn;
}
