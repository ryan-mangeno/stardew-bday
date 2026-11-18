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

  ISteamNetworkingSockets *sockets = SteamNetworkingSockets();

  SteamNetworkingIPAddr host;
  const std::string local_addr = "127.0.0.1:27020";
  bool parsed = host.ParseString(local_addr.c_str());
  if (!parsed) {
    LOG_ERROR("Failed to parse local IP address: {}", local_addr);
  } else {
    LOG_INFO("Parsed local IP address: {}", local_addr);
  }

  HSteamListenSocket listener = sockets->CreateListenSocketIP(host, 0, nullptr);

  if (listener == k_HSteamListenSocket_Invalid) {
    LOG_FATAL("Failed to create listening socket at {}", local_addr);
  } else {
    LOG_INFO("Created listening socket at {}", local_addr);
  }

  GameNetworkingSockets_Kill();
  return EXIT_SUCCESS;
}
