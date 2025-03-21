// Device Management
const char* getDeviceInfo = 
  "<s:Body>"
  "<tds:GetDeviceInformationResponse>"
  "<tds:Manufacturer>%s</tds:Manufacturer>"
  "<tds:Model>%s</tds:Model>"
  "<tds:FirmwareVersion>%s</tds:FirmwareVersion>"
  "<tds:SerialNumber>%s</tds:SerialNumber>"
  "<tds:HardwareId>%s</tds:HardwareId>"
  "</tds:GetDeviceInformationResponse>";

const char* getCapabilitiesStart = 
  "<s:Body>"
  "<tds:GetCapabilitiesResponse>"
  "<tds:Capabilities>";

const char* getCapabilitiesDevice = 
  "<tt:Device>"
  "<tt:XAddr>http://%s/onvif/device_service</tt:XAddr>"
  "<tt:Network>"
  "<tt:IPFilter>true</tt:IPFilter>"
  "<tt:ZeroConfiguration>true</tt:ZeroConfiguration>"
  "<tt:IPVersion6>false</tt:IPVersion6>"
  "<tt:DynDNS>false</tt:DynDNS>"
  "<tt:Extension>"
  "<tt:Dot11Configuration>false</tt:Dot11Configuration>"
  "</tt:Extension>"
  "</tt:Network>"
  "<tt:System>"
  "<tt:DiscoveryResolve>true</tt:DiscoveryResolve>"
  "<tt:DiscoveryBye>true</tt:DiscoveryBye>"
  "<tt:RemoteDiscovery>false</tt:RemoteDiscovery>"
  "<tt:SystemBackup>true</tt:SystemBackup>"
  "<tt:SystemLogging>true</tt:SystemLogging>"
  "<tt:FirmwareUpgrade>true</tt:FirmwareUpgrade>"
  "<tt:SupportedVersions>"
  "<tt:Major>23</tt:Major><tt:Minor>12</tt:Minor>"
  "</tt:SupportedVersions>"
  "<tt:Extension>"
  "<tt:HttpFirmwareUpgrade>true</tt:HttpFirmwareUpgrade>"
  "<tt:HttpSystemBackup>true</tt:HttpSystemBackup>"
  "<tt:HttpSystemLogging>true</tt:HttpSystemLogging>"
  "<tt:HttpSupportInformation>true</tt:HttpSupportInformation>"
  "</tt:Extension>"
  "</tt:System>"
  "<tt:IO>"
  "<tt:InputConnectors>0</tt:InputConnectors>"
  "<tt:RelayOutputs>0</tt:RelayOutputs>"
  "<tt:Extension>"
  "<tt:Auxiliary>false</tt:Auxiliary>"
  "<tt:Extension/>"
  "</tt:Extension>"
  "</tt:IO>"
  "<tt:Security>"
  "<tt:TLS1.1>false</tt:TLS1.1>"
  "<tt:TLS1.2>false</tt:TLS1.2>"
  "<tt:OnboardKeyGeneration>false</tt:OnboardKeyGeneration>"
  "<tt:AccessPolicyConfig>false</tt:AccessPolicyConfig>"
  "<tt:X.509Token>false</tt:X.509Token>"
  "<tt:SAMLToken>false</tt:SAMLToken>"
  "<tt:KerberosToken>false</tt:KerberosToken>"
  "<tt:RELToken>false</tt:RELToken>"
  "<tt:Extension>"
  "<tt:TLS1.0>false</tt:TLS1.0>"
  "<tt:Extension>"
  "<tt:Dot1X>false</tt:Dot1X>"
  "<tt:SupportedEAPMethod>0</tt:SupportedEAPMethod>"
  "<tt:RemoteUserHandling>true</tt:RemoteUserHandling>"
  "</tt:Extension>"
  "</tt:Extension>"
  "</tt:Security>"
  "</tt:Device>";

const char* getCapabilitiesEvents = 
  "<tt:Events>"
  "<tt:XAddr>http://%s/onvif/event_service</tt:XAddr>"
  "<tt:WSSubscriptionPolicySupport>true</tt:WSSubscriptionPolicySupport>"
  "<tt:WSPullPointSupport>true</tt:WSPullPointSupport>"
  "<tt:WSPausableSubscriptionManagerInterfaceSupport>false</tt:WSPausableSubscriptionManagerInterfaceSupport>"
  "</tt:Events>";

const char* getCapabilitiesImaging = 
  "<tt:Imaging>"
  "<tt:XAddr>http://%s/onvif/image_service</tt:XAddr>"
  "</tt:Imaging>";

const char* getCapabilitiesMedia = 
  "<tt:Media>"
  "<tt:XAddr>http://%s/onvif/media_service</tt:XAddr>"
  "<tt:StreamingCapabilities>"
  "<tt:RTPMulticast>true</tt:RTPMulticast>"
  "<tt:RTP_TCP>true</tt:RTP_TCP>"
  "<tt:RTP_RTSP_TCP>true</tt:RTP_RTSP_TCP>"
  "</tt:StreamingCapabilities>"
  "<tt:Extension>"
  "<tt:ProfileCapabilities>"
  "<tt:MaximumNumberOfProfiles>10</tt:MaximumNumberOfProfiles>"
  "</tt:ProfileCapabilities>"
  "</tt:Extension>"
  "</tt:Media>";

const char* getCapabilitiesExension = 
  "<tt:Extension>"
  "<tt:DeviceIO>"
  "<tt:XAddr>http://%s/onvif/deviceio_service</tt:XAddr>"
  "<tt:VideoSources>1</tt:VideoSources>"       // One camera
  "<tt:VideoOutputs>0</tt:VideoOutputs>"       // No video outputs
  "<tt:AudioSources>%s</tt:AudioSources>"       // One microphone
  "<tt:AudioOutputs>%s</tt:AudioOutputs>"       // One amplifier speaker
  "<tt:RelayOutputs>0</tt:RelayOutputs>"       // No relay outputs
  "</tt:DeviceIO>"
  // "<tt:Recording>"
  // "<tt:XAddr>http://%s/onvif/recording_service</tt:XAddr>"
  // "<tt:ReceiverSource>false</tt:ReceiverSource>"
  // "<tt:MediaProfileSource>true</tt:MediaProfileSource>"
  // "<tt:DynamicRecordings>true</tt:DynamicRecordings>"
  // "<tt:DynamicTracks>true</tt:DynamicTracks>"
  // "<tt:MaxStringLength>256</tt:MaxStringLength>"
  // "</tt:Recording>"
  // "<tt:Search>"
  // "<tt:XAddr>http://%s/onvif/search_service</tt:XAddr>"
  // "<tt:MetadataSearch>true</tt:MetadataSearch>"
  // "</tt:Search>"
  // "<tt:Replay>"
  // "<tt:XAddr>http://%s/onvif/replay_service</tt:XAddr>"
  // "</tt:Replay>"
  // "<tt:Receiver>"
  // "<tt:XAddr>http://%s/onvif/receiver_service</tt:XAddr>"
  // "<tt:RTP_Multicast>true</tt:RTP_Multicast>"
  // "<tt:RTP_TCP>true</tt:RTP_TCP>"
  // "<tt:RTP_RTSP_TCP>true</tt:RTP_RTSP_TCP>"
  // "<tt:SupportedReceivers>10</tt:SupportedReceivers>"
  // "<tt:MaximumRTSPURILength>256</tt:MaximumRTSPURILength>"
  // "</tt:Receiver>"
  "</tt:Extension>";

const char* getCapabilitiesPTZ = 
  "<tt:PTZ>"
  "<tt:XAddr>http://%s/onvif/ptz_service</tt:XAddr>"
  "</tt:PTZ>";

const char* getCapabilitiesEnd = 
  "</tds:Capabilities>"
  "</tds:GetCapabilitiesResponse>";


// Services
const char* getServicesStart = 
  "<s:Body>"
  "<tds:GetServicesResponse>";

const char* getServicesDevice= 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver10/device/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/device_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>23</tt:Major>"
  "<tt:Minor>12</tt:Minor>"
  "</tds:Version>"
  "</tds:Service>";
  
const char* getServicesEvent = 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver10/events/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/event_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>22</tt:Major>"
  "<tt:Minor>6</tt:Minor>"
  "</tds:Version>"
  "</tds:Service>";

const char* getServicesMedia2 = 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver20/media/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/media2_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>23</tt:Major>"
  "<tt:Minor>6</tt:Minor>"
  "</tds:Version>"
  "</tds:Service>";

const char* getServicesMedia = 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver10/media/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/media_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>22</tt:Major>"
  "<tt:Minor>12</tt:Minor>"
  "</tds:Version>"
  "</tds:Service>";

const char* getServicesPTZ = 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver20/ptz/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/ptz_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>23</tt:Major>"
  "<tt:Minor>6</tt:Minor>"
  "</tds:Version>"
  "</tds:Service>";

const char* getServicesImage = 
  "<tds:Service>"
  "<tds:Namespace>http://www.onvif.org/ver20/imaging/wsdl</tds:Namespace>"
  "<tds:XAddr>http://%s/onvif/image_service</tds:XAddr>"
  "<tds:Version>"
  "<tt:Major>22</tt:Major>"
  "<tt:Minor>6</tt:Minor>"
  "</tds:Version>"
 "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver20/analytics/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/analytics_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>23</tt:Major>"
  // "<tt:Minor>12</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/recording/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/recording_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>23</tt:Major>"
  // "<tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/search/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/search_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>22</tt:Major>"
  // "<tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/replay/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/replay_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>21</tt:Major>"
  // "<tt:Minor>12</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/accesscontrol/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/accesscontrol_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>21</tt:Major>"
  // "<tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/doorcontrol/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/doorcontrol_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>21</tt:Major>"
  // "<tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/deviceIO/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/deviceio_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>22</tt:Major>"
  // "<tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/thermal/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/thermal_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>22</tt:Major><tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/credential/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/credential_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>21</tt:Major><tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/accessrules/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/accessrules_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>19</tt:Major><tt:Minor>6</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/schedule/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/schedule_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>18</tt:Major><tt:Minor>12</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/receiver/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/receiver_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>21</tt:Major><tt:Minor>12</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  // "<tds:Service>"
  // "<tds:Namespace>http://www.onvif.org/ver10/provisioning/wsdl</tds:Namespace>"
  // "<tds:XAddr>http://%s/onvif/provisioning_service</tds:XAddr>"
  // "<tds:Version>"
  // "<tt:Major>18</tt:Major><tt:Minor>12</tt:Minor>"
  // "</tds:Version>"
  // "</tds:Service>"
  ;

const char* getServicesEnd = 
  "</tds:GetServicesResponse>";



const char* getEndpointReference = 
  "<s:Body>"
  "<tds:GetEndpointReferenceResponse>"
  "<tds:EndpointReference>"
  "<wsa:Address>urn:uuid:%s</wsa:Address>"
  "</tds:EndpointReference>"
  "</tds:GetEndpointReferenceResponse>";

//Scopes
const char* getScopesStart =
  "<s:Body>"
  "<tds:GetScopesResponse>";

const char* getScopesAudio =
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/audio_encoder</tt:ScopeItem>"
  "</tds:Scopes>";

const char* getScopesVideo =
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/video_encoder</tt:ScopeItem>"
  "</tds:Scopes>";

const char* getScopesPTZ =
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/ptz</tt:ScopeItem>"
  "</tds:Scopes>";

const char* getScopesProS =
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/Streaming</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/S</tt:ScopeItem>"
  "</tds:Scopes>";
  
const char* getScopesProT =
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/T</tt:ScopeItem>"
  "</tds:Scopes>";
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/G</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/C</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/A</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/M</tt:ScopeItem>"
  // "</tds:Scopes>"

const char* getScopesInfo =
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/location/%s</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/name/%s</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/hardware/%s</tt:ScopeItem>"
  "</tds:Scopes>";

const char* getScopesEnd =
  "</tds:GetScopesResponse>";

  const char* getScopes =
  "<s:Body>"
  "<tds:GetScopesResponse>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/audio_encoder</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/video_encoder</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/type/ptz</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/Streaming</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/S</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/Profile/T</tt:ScopeItem>"
  "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/G</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/C</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/A</tt:ScopeItem>"
  // "</tds:Scopes>"
  // "<tds:Scopes>"
  // "<tt:ScopeDef>Fixed</tt:ScopeDef>"
  // "<tt:ScopeItem>onvif://www.onvif.org/Profile/M</tt:ScopeItem>"
  // "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/location/%s</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/name/%s</tt:ScopeItem>"
  "</tds:Scopes>"
  "<tds:Scopes>"
  "<tt:ScopeDef>Configurable</tt:ScopeDef>"
  "<tt:ScopeItem>onvif://www.onvif.org/hardware/%s</tt:ScopeItem>"
  "</tds:Scopes>"
  "</tds:GetScopesResponse>";
  
const char* getZeroConfig = 
  "<s:Body>"
  "<tds:GetZeroConfigurationResponse>"
  "<tds:ZeroConfiguration>"
  "<tt:InterfaceToken>NetworkInterfaceToken_1</tt:InterfaceToken>"
  "<tt:Enabled>true</tt:Enabled>"
  "<tt:Addresses>"
  "<tt:IPv4Address>%s</tt:IPv4Address>"
  "</tt:Addresses>"
  "</tds:ZeroConfiguration>"
  "</tds:GetZeroConfigurationResponse>";

const char* getNetworkInterfaces =
  "<s:Body>"
  "<tds:GetNetworkInterfacesResponse>"
  "<tds:NetworkInterfaces token=\"NetworkInterfaceToken_1\">"
  "<tt:Enabled>true</tt:Enabled>"
  "<tt:Info>"
  "<tt:Name>WiFi</tt:Name>"
  "<tt:HwAddress>%s</tt:HwAddress>"
  "<tt:MTU>1500</tt:MTU>"
  "</tt:Info>"
  "<tt:IPv4>"
  "<tt:Enabled>true</tt:Enabled>"
  "<tt:Config>"
  "<tt:FromDHCP>"
  "<tt:Address>%s</tt:Address>"
  "<tt:PrefixLength>24</tt:PrefixLength>"
  "</tt:FromDHCP>"
  "<tt:DHCP>true</tt:DHCP>"
  "</tt:Config>"
  "</tt:IPv4>"
  "</tds:NetworkInterfaces>"
  "</tds:GetNetworkInterfacesResponse>";

  const char* getNTP = 
  "<s:Body>"
  "<tds:GetNTPResponse>"
  "<tds:NTPInformation>"
  "<tt:FromDHCP>false</tt:FromDHCP>"
  "<tt:NTPManual>"
  "<tt:Type>DNS</tt:Type>"
  "<tt:DNSname>time.windows.com</tt:DNSname>"
  "</tt:NTPManual>"
  "</tds:NTPInformation>"
  "</tds:GetNTPResponse>";
  
  const char* getHostname = 
  "<s:Body>"
  "<tds:GetHostnameResponse>"
  "<tds:HostnameInformation>"
  "<tt:FromDHCP>false</tt:FromDHCP>"
  "<tt:Name>SpecialKMeta</tt:Name>"
  "</tds:HostnameInformation>"
  "</tds:GetHostnameResponse>";
  
  const char* getNetworkDefaultGateway = 
  "<s:Body>"
  "<tds:GetNetworkDefaultGatewayResponse>"
  "<tds:NetworkGateway>"
  "<tt:IPv4Address>192.168.1.1</tt:IPv4Address>"
  "</tds:NetworkGateway>"
  "</tds:GetNetworkDefaultGatewayResponse>";
  
  const char* getNetworkProtocols = 
  "<s:Body>"
  "<tds:GetNetworkProtocolsResponse>"
  "<tds:NetworkProtocols>"
  "<tt:Name>HTTP</tt:Name>"
  "<tt:Enabled>true</tt:Enabled>"
  "<tt:Port>8000</tt:Port>"
  "</tds:NetworkProtocols>"
  "<tds:NetworkProtocols>"
  "<tt:Name>HTTPS</tt:Name>"
  "<tt:Enabled>false</tt:Enabled>"
  "<tt:Port>8443</tt:Port>"
  "</tds:NetworkProtocols>"
  "<tds:NetworkProtocols>"
  "<tt:Name>RTSP</tt:Name>"
  "<tt:Enabled>true</tt:Enabled>"
  "<tt:Port>554</tt:Port>"
  "</tds:NetworkProtocols>"
  "</tds:GetNetworkProtocolsResponse>";  

const char* getDiscoveryMode = 
  "<s:Body>"
  "<tds:GetDiscoveryModeResponse>"
  "<tds:DiscoveryMode>Discoverable</tds:DiscoveryMode>"
  "</tds:GetDiscoveryModeResponse>";

const char* setNetworkProtocols = 
  "<s:Body>"
  "<tds:SetNetworkProtocolsResponse>"
  "<tds:RebootNeeded>true</tds:RebootNeeded>"
  "</tds:SetNetworkProtocolsResponse>";

const char* getDNS = 
  "<s:Body>"
  "<tds:GetDNSResponse>"
  "<tds:DNSInformation>"
  "<tt:FromDHCP>false</tt:FromDHCP>"
  "<tt:DNSManual>"
  "<tt:Type>IPv4</tt:Type>"
  "<tt:IPv4Address>%s</tt:IPv4Address>"
  "</tt:DNSManual>"
  "</tds:DNSInformation>"
  "</tds:GetDNSResponse>";

const char* getSystemDateAndTime = 
  "<s:Body>"
  "<tds:GetSystemDateAndTimeResponse>"
  "<tds:SystemDateAndTime>"
  "<tt:DateTimeType>%s</tt:DateTimeType>"
  "<tt:DaylightSavings>%s</tt:DaylightSavings>"
  "<tt:TimeZone>"
  "<tt:TZ>%s</tt:TZ>"
  "</tt:TimeZone>"
  "<tt:UTCDateTime>"
  "<tt:Time>"
  "<tt:Hour>%02d</tt:Hour>"
  "<tt:Minute>%02d</tt:Minute>"
  "<tt:Second>%02d</tt:Second>"
  "</tt:Time>"
  "<tt:Date>"
  "<tt:Year>%04d</tt:Year>"
  "<tt:Month>%02d</tt:Month>"
  "<tt:Day>%02d</tt:Day>"
  "</tt:Date>"
  "</tt:UTCDateTime>"
  "</tds:SystemDateAndTime>"
  "</tds:GetSystemDateAndTimeResponse>";

const char* setSystemDateAndTime =
  "<s:Body>"
  "<tds:SetSystemDateAndTimeResponse/>";
