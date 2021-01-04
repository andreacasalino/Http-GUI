#include <httplib.h>
#include <iostream>

using namespace httplib;
using namespace std;

int main(void) {

  Server svr;

  svr.Get("/hi", [](const Request& req, Response& res) {
    res.set_content("Hello World!", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
  });

  svr.Post("/ci", [](const Request& req, Response& res) {
    cout << "body: " << req.body << endl;

    res.set_content("Ciao a tutti!", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
  });

  svr.Get("/stop", [&](const Request& req, Response& res) {
    svr.stop();
  });

  svr.listen("localhost", 3000);

  return EXIT_FAILURE;
}
