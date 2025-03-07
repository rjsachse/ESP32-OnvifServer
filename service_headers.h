#ifndef ONVIFHEADERS_H
#define ONVIFHEADERS_H

const char* discoverNS = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
  "xmlns:enc=\"http://www.w3.org/2003/05/soap-encoding\" "
  "xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" "
  "xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" "
  "xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" "
  "xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\">";

// ! ! ! Copilot code NOT TESTED ! ! ! Use maxHeader until each gets tested.
// Discovery Service // https://www.onvif.org/specs/srv/discovery/ONVIF-Discovery-Service-Spec.pdf
const char* discoveryHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Device Service // https://www.onvif.org/specs/core/ONVIF-Core-Specification.pdf
const char* deviceHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Media Service // https://www.onvif.org/specs/srv/media/ONVIF-Media-Service-Spec.pdf
const char* mediaHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\">"
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\">"
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Media2 Service // https://www.onvif.org/specs/srv/media/ONVIF-Media2-Service-Spec.pdf
const char* media2Header = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\">"
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tr2=\"http://www.onvif.org/ver20/media/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// PTZ Service // https://www.onvif.org/specs/srv/ptz/ONVIF-PTZ-Service-Spec-v1706.pdf
const char* ptzHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Imaging Service // https://www.onvif.org/specs/srv/img/ONVIF-Imaging-Service-Spec.pdf
const char* imagingHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Events Service // https://www.onvif.org/specs/srv/events/ONVIF-Events-Service-Spec.pdf
const char* eventsHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Analytics Service // https://www.onvif.org/specs/srv/analytics/ONVIF-Analytics-Service-Spec.pdf
const char* analyticsHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Recording Service // https://www.onvif.org/specs/srv/recording/ONVIF-Recording-Service-Spec.pdf
const char* recordingHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Search Service // https://www.onvif.org/specs/srv/search/ONVIF-Search-Service-Spec.pdf
const char* searchHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tse=\"http://www.onvif.org/ver10/search/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Replay Service // https://www.onvif.org/specs/srv/replay/ONVIF-Replay-Service-Spec.pdf
const char* replayHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trp=\"http://www.onvif.org/ver10/replay/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Receiver Service // https://www.onvif.org/specs/srv/receiver/ONVIF-Receiver-Service-Spec.pdf
const char* receiverHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trv=\"http://www.onvif.org/ver10/receiver/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Display Service // https://www.onvif.org/specs/srv/display/ONVIF-Display-Service-Spec.pdf
const char* displayHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tdisp=\"http://www.onvif.org/ver10/display/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// DeviceIO Service // https://www.onvif.org/specs/srv/deviceIO/ONVIF-DeviceIO-Service-Spec.pdf
const char* deviceIOHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tio=\"http://www.onvif.org/ver10/deviceIO/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Access Control Service // https://www.onvif.org/specs/srv/accesscontrol/ONVIF-AccessControl-Service-Spec.pdf
const char* accessControlHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tacs=\"http://www.onvif.org/ver10/accesscontrol/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Credential Service // https://www.onvif.org/specs/srv/credential/ONVIF-Credential-Service-Spec.pdf
const char* credentialHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tcred=\"http://www.onvif.org/ver10/credential/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Provisioning Service // https://www.onvif.org/specs/srv/provisioning/ONVIF-Provisioning-Service-Spec.pdf
const char* provisioningHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tpv=\"http://www.onvif.org/ver10/provisioning/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// Door Control Service // https://www.onvif.org/specs/srv/door/ONVIF-DoorControl-Service-Spec.pdf
const char* doorControlHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tdc=\"http://www.onvif.org/ver10/doorcontrol/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";


// Thermal Service // https://www.onvif.org/specs/srv/thermal/ONVIF-Thermal-Service-Spec.pdf
const char* thermalHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tth=\"http://www.onvif.org/ver10/thermal/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";


// Storage Service // https://www.onvif.org/specs/srv/storage/ONVIF-Storage-Service-Spec.pdf
const char* storageHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tst=\"http://www.onvif.org/ver10/storage/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";


// Network Service // https://www.onvif.org/specs/srv/network/ONVIF-Network-Service-Spec.pdf
const char* networkHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

// PACS Service // https://www.onvif.org/specs/srv/pacs/ONVIF-PACS-Service-Spec.pdf
const char* pacsHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:pt=\"http://www.onvif.org/ver10/pacs\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\">";

const char* maxHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:e=\"http://www.w3.org/2003/05/soap-encoding\" "
  "xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" "
  "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
  "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
  "xmlns:wsaw=\"http://www.w3.org/2006/05/addressing/wsdl\" "
  "xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" "
  "xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" "
  "xmlns:wsntw=\"http://docs.oasis-open.org/wsn/bw-2\" "
  "xmlns:wsrf-rw=\"http://docs.oasis-open.org/wsrf/rw-2\" "
  "xmlns:wsrf-r=\"http://www.onvif.org/ver10/schema/wsrf-r-2\" "
  "xmlns:wsrf-bf=\"http://docs.oasis-open.org/wsrf/bf-2\" "
  "xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl\" "
  "xmlns:wsoap12=\"http://schemas.xmlsoap.org/wsdl/soap12\" "
  "xmlns:http=\"http://schemas.xmlsoap.org/wsdl/http\" "
  "xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" "
  "xmlns:wsadis=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tns1=\"http://www.onvif.org/ver10/topics\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
  "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
  "xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" "
  "xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" "
  "xmlns:tst=\"http://www.onvif.org/ver10/storage/wsdl\" "
  "xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" "
  "xmlns:pt=\"http://www.onvif.org/ver10/pacs\" "
  "xmlns:tr2=\"http://www.onvif.org/ver20/media/wsdl\" "
  "xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" "
  "xmlns:tan=\"http://www.onvif.org/ver20/analytics/wsdl\" "
  "xmlns:axt=\"http://www.onvif.org/ver20/analytics\" "
  "xmlns:trp=\"http://www.onvif.org/ver10/replay/wsdl\" "
  "xmlns:tse=\"http://www.onvif.org/ver10/search/wsdl\" "
  "xmlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" "
  "xmlns:tac=\"http://www.onvif.org/ver10/accesscontrol/wsdl\" "
  "xmlns:tdc=\"http://www.onvif.org/ver10/doorcontrol/wsdl\" "
  "xmlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" "
  "xmlns:tth=\"http://www.onvif.org/ver10/thermal/wsdl\" "
  "xmlns:tcr=\"http://www.onvif.org/ver10/credential/wsdl\" "
  "xmlns:tar=\"http://www.onvif.org/ver10/accessrules/wsdl\" "
  "xmlns:tsc=\"http://www.onvif.org/ver10/schedule/wsdl\" "
  "xmlns:trv=\"http://www.onvif.org/ver10/receiver/wsdl\" "
  "xmlns:tpv=\"http://www.onvif.org/ver10/provisioning/wsdl\" "
  "xmlns:ter=\"http://www.onvif.org/ver10/error\">";

const char* footer = 
  "</s:Body></s:Envelope>";

#endif // ONVIF_H
