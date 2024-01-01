#pragma once

////////////////////////////////////////////////////////////
// certificate for test make.com

const char* remote_host = "hook.us1.make.com"; // change it according to the webhook address given by make.com
const uint16_t remote_port = 443;

// How to export root cert of the server: please refer to https://randomnerdtutorials.com/esp8266-nodemcu-https-requests/
const char cert_Root_CA [] PROGMEM = R"CERT(
<<Insert you Root certificate here>>
)CERT";

// end of certificate chain for make.com
////////////////////////////////////////////////////////////
