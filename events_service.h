//Events
const char* createPullPointSubscriptionResponse =
  "<s:Envelope>"
  "<s:Header>"
  "<wsa:MessageID>urn:uuid:%s</wsa:MessageID>"
  "<wsa:To>http://www.w3.org/2005/08/addressing/anonymous</wsa:To>"
  "<wsa:Action>http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse</wsa:Action>"
  "</s:Header>"
  "<s:Body>"
  "<tev:CreatePullPointSubscriptionResponse>"
  "<tev:SubscriptionReference>"
  "<wsa:Address>http://%s/event_service/0</wsa:Address>"
  "</tev:SubscriptionReference>"
  "<wsnt:CurrentTime>2024-10-26T03:04:29Z</wsnt:CurrentTime>"
  "<wsnt:TerminationTime>2024-10-26T03:05:29Z</wsnt:TerminationTime>"
  "</tev:CreatePullPointSubscriptionResponse>"
  "</s:Body>"
  "</s:Envelope>";
