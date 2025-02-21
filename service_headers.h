#ifndef ONVIFHEADERS_H
#define ONVIFHEADERS_H

const char* deviceHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\">";

const char* mediaHeader = 
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\">";

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

const char* allHeader =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
  "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" "
  "xmlns:tt=\"http://www.onvif.org/ver10/schema\" "
  "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
  "xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" "
  "xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" "
  "xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" "
  "xmlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\" "
  "xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" "
  "xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">";

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
