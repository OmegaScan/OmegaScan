#include "parser.hh"
using namespace std;

int main(int argc, char const *argv[]) {
  // (端口解析与ip解析类同)

  // 传入符合规范的ip范围，返回一个两地址范围内的地址列表
  string case1 = "221.2.31.3-221.2.32.4";

  // 传入一个ip地址，返回一个仅有该地址的列表
  string case2 = "232.23.1.44";

  // 传入以'-'结尾的ip地址，返回该地址至最后一位地址之间的地址列表，即
  // 221.2.31.3 ~ 255.255.255.255
  string case3 = "221.2.31.3-";

  // 传入数值不合法的ip地址，返回空列表
  string case4 = "221.2.31.3-221.2.3223424234.4";

  // 传入起址大于终址的范围，返回空列表
  string case5 = "221.2.31.3-0.2.32.4";

  vector<string> vs = parse_host(case1);
  for (string s : vs) {
    cout << s << endl;
  }
  return 0;
}
