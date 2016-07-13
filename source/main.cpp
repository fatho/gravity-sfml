#include "octo/game.hpp"
#include "fmtlog/fmtlog.hpp"

#include <SFML/Graphics.hpp>
#include <boost/type_index.hpp>

int main() {
  fmtlog::Log log("<main>");
  log.info("started");
  try {
    octo::Game game;
    game.run();
  } catch(const std::exception& ex) {
    log.fatal("unhandled exception of type %s: %s", boost::typeindex::type_id_runtime(ex).pretty_name(), ex.what());
  } catch(...) {
    log.fatal("unhandled unkown exception");
  }
  log.info("about to exit");
  return 0;
}
