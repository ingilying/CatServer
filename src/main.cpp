#include "database/DBPool.hpp"
#include "database/DBUtils.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  cout << "hello world!" << endl;
  DBPool pool(2, {"ingil", "a1a2a4a5", "127.0.0.1", 3306, "catserver"});
  pool.Initialize();
  if (pool.GetError()) {
    return -1;
  }
  auto con1 = pool.Get();
  pool.Put(std::move(con1));
  auto con2 = pool.Get();
  pool.Put(std::move(con2));
  return 0;
}
