#include "logger.h"

#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

#include <cstdlib>
#include <string>

int main() {
  SteamNetworkingErrMsg error{};
  if (!GameNetworkingSockets_Init(nullptr, error)) {
    LOG_FATAL("GameNetworkingSockets initialization failed: {}", error);
    return EXIT_FAILURE;
  } else {
    LOG_INFO("GameNetworkingSockets initialized successfully.");
  }

  SteamNetworkingIPAddr host;
  const std::string local_addr = "127.0.0.1:27020";
  bool parsed = host.ParseString(local_addr.c_str());
  if (!parsed) {
    LOG_ERROR("Failed to parse host address: {}", local_addr.c_str());
  } else {
    LOG_INFO("Attemping to resolve host address: {}", local_addr.c_str());
  }

  GameNetworkingSockets_Kill();
  return EXIT_SUCCESS;
}
