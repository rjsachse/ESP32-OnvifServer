const char* getProfilesMedia2 = 
  "<s:Body>"
  "<tr2:GetProfilesResponse>"
  "<tr2:Profiles token=\"ProfileToken_1\" fixed=\"true\">"
  "<tr2:Name>ProfileName_1</tr2:Name>"
  "<tr2:Configurations>"

  // "<tr2:VideoSourceConfiguration token=\"VideoSourceConfigurationToken_1\">"
  // "<tr2:Name>VideoSourceConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>2</tr2:UseCount>"
  // "<tr2:SourceToken>VideoSourceToken_1</tr2:SourceToken>"
  // "<tr2:Bounds x=\"0\" y=\"0\" width=\"1280\" height=\"720\"/>"
  // "</tr2:VideoSourceConfiguration>"

  // "<tr2:AudioSourceConfiguration token=\"AudioSourceConfigurationToken_1\">"
  // "<tr2:Name>AudioSourceConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>2</tr2:UseCount>"
  // "<tr2:SourceToken>AudioSourceToken_1</tr2:SourceToken>"
  // "</tr2:AudioSourceConfiguration>"

  // "<tr2:VideoEncoderConfiguration token=\"VideoEncoderConfigurationToken_1\">"
  // "<tr2:Name>VideoEncoderConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>1</tr2:UseCount>"
  // "<tr2:Encoding>JPEG</tr2:Encoding>"
  // "<tr2:Resolution>"
  // "<tr2:Width>1280</tr2:Width>"
  // "<tr2:Height>720</tr2:Height>"
  // "</tr2:Resolution>"
  // "<tr2:Quality>4</tr2:Quality>"
  // "<tr2:RateControl>"
  // "<tr2:FrameRateLimit>25</tr2:FrameRateLimit>"
  // "<tr2:EncodingInterval>1</tr2:EncodingInterval>"
  // "<tr2:BitrateLimit>2048</tr2:BitrateLimit>"
  // "</tr2:RateControl>"
  // "<tr2:SessionTimeout>PT10S</tr2:SessionTimeout>"
  // "</tr2:VideoEncoderConfiguration>"

  // "<tr2:AudioEncoderConfiguration token=\"AudioEncoderConfigurationToken_1\">"
  // "<tr2:Name>AudioEncoderConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>2</tr2:UseCount>"
  // "<tr2:Encoding>G726</tr2:Encoding>"
  // "<tr2:Bitrate>32000</tr2:Bitrate>"
  // "<tr2:SampleRate>16000</tr2:SampleRate>"
  // "</tr2:AudioEncoderConfiguration>"

  // "<tr2:AudioOutputConfiguration token=\"AudioOutputConfigurationToken_1\">"
  // "<tr2:Name>AudioOutputConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>2</tr2:UseCount>"
  // "<tr2:OutputToken>AudioOutputToken_1</tr2:OutputToken>"
  // "<tr2:SendPrimacy>Primary</tr2:SendPrimacy>"
  // "<tr2:OutputLevel>50</tr2:OutputLevel>"
  // "</tr2:AudioOutputConfiguration>"

  // "<tr2:PTZConfiguration token=\"PTZConfigurationToken_1\">"
  // "<tr2:Name>PTZConfigurationName_1</tr2:Name>"
  // "<tr2:UseCount>1</tr2:UseCount>"
  // "<tr2:NodeToken>PTZNodeToken_1</tr2:NodeToken>"
  // "<tr2:DefaultAbsolutePantTiltPositionSpace>http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace</tr2:DefaultAbsolutePantTiltPositionSpace>"
  // "<tr2:DefaultAbsoluteZoomPositionSpace>http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace</tr2:DefaultAbsoluteZoomPositionSpace>"
  // "<tr2:DefaultRelativePanTiltTranslationSpace>http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace</tr2:DefaultRelativePanTiltTranslationSpace>"
  // "<tr2:DefaultRelativeZoomTranslationSpace>http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace</tr2:DefaultRelativeZoomTranslationSpace>"
  // "<tr2:DefaultContinuousPanTiltVelocitySpace>http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace</tr2:DefaultContinuousPanTiltVelocitySpace>"
  // "<tr2:DefaultContinuousZoomVelocitySpace>http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace</tr2:DefaultContinuousZoomVelocitySpace>"
  // "<tr2:DefaultPTZTimeout>PT30S</tr2:DefaultPTZTimeout>"
  // "<tr2:PanTiltLimits>"
  // "<tr2:Range>"
  // "<tr2:URI>http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace</tr2:URI>"
  // "<tr2:XRange>"
  // "<tr2:Min>-1.0</tr2:Min>"
  // "<tr2:Max>1.0</tr2:Max>"
  // "</tr2:XRange>"
  // "<tr2:YRange>"
  // "<tr2:Min>-1.0</tr2:Min>"
  // "<tr2:Max>1.0</tr2:Max>"
  // "</tr2:YRange>"
  // "</tr2:Range>"
  // "</tr2:PanTiltLimits>"
  // "<tr2:ZoomLimits>"
  // "<tr2:Range>"
  // "<tr2:URI>http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace</tr2:URI>"
  // "<tr2:XRange>"
  // "<tr2:Min>0.0</tr2:Min>"
  // "<tr2:Max>1.0</tr2:Max>"
  // "</tr2:XRange>"
  // "</tr2:Range>"
  // "</tr2:ZoomLimits>"
  // "</tr2:PTZConfiguration>"

  "</tr2:Configurations>"
  "</tr2:Profiles>"
  "</tr2:GetProfilesResponse>";

const char* getStreamUri2 = 
  "<s:Body>"
  "<tr2:GetStreamUriResponse>"
  "<tr2:Uri>rtsp://%s:554</tr2:Uri>"
  "</tr2:GetStreamUriResponse>";

const char* getSnapshotUri2 = 
  "<s:Body>"
  "<tr2:GetSnapshotUriResponse>"
  "<tr2:Uri>http://%s/onvif/snapshot</tr2:Uri>"
  "</tr2:GetSnapshotUriResponse>";

const char* getAudioDecoderConfigurationOptions2 = 
  "<s:Body>"
  "<tr2:GetAudioDecoderConfigurationOptionsResponse>"
  "<tr2:Options>"
  "<tr2:AudioDecoderConfigurationOption>"
  "<tr2:Encoding>L16</tr2:Encoding>"
  "<tr2:BitrateList>"
  "<tr2:Items>16000</tr2:Items>"
  "</tr2:BitrateList>"
  "<tr2:SampleRateList>"
  "<tr2:Items>16000</tr2:Items>"
  "</tr2:SampleRateList>"
  "</tr2:AudioDecoderConfigurationOption>"
  "</tr2:Options>"
  "</tr2:GetAudioDecoderConfigurationOptionsResponse>";

const char* getAudioDecoderConfigurations2 = 
  "<s:Body>"
  "<tr2:GetAudioDecoderConfigurationsResponse>"
  "<tr2:Configurations token=\"AudioDecoderConfigurationToken_1\">"
  "<tr2:Name>AudioDecoderConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "</tr2:Configurations>"
  "</tr2:GetAudioDecoderConfigurationsResponse>";

const char* getAudioEncoderConfigurations2 = 
  "<s:Body>"
  "<tr2:GetAudioEncoderConfigurationsResponse>"
  "<tr2:Configurations token=\"AudioEncoderConfigurationToken_1\">"
  "<tr2:Name>AudioEncoderConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "<tr2:Encoding>L16</tr2:Encoding>"
  "<tr2:Bitrate>16000</tr2:Bitrate>"
  "<tr2:SampleRate>16000</tr2:SampleRate>"
  "</tr2:Configurations>"
  "</tr2:GetAudioEncoderConfigurationsResponse>";

const char* getAudioEncoderConfigurationOptions2 = 
  "<s:Body>"
  "<tr2:GetAudioEncoderConfigurationOptionsResponse>"
  "<tr2:Options>"
  "<tr2:Encoding>L16</tr2:Encoding>"
  "<tr2:BitrateList>"
  "<tr2:Items>16000</tr2:Items>"
  "</tr2:BitrateList>"
  "<tr2:SampleRateList>"
  "<tr2:Items>16000</tr2:Items>"
  "</tr2:SampleRateList>"
  "</tr2:Options>"
  "</tr2:GetAudioEncoderConfigurationOptionsResponse>";

const char* getAudioOutputConfigurationOptions2 = 
  "<s:Body>"
  "<tr2:GetAudioOutputConfigurationOptionsResponse>"
  "<tr2:Options>"
  "<tr2:OutputTokensAvailable>AudioOutputToken_1</tr2:OutputTokensAvailable>"
  "<tr2:OutputLevelRange>"
  "<tr2:Min>0</tr2:Min>"
  "<tr2:Max>100</tr2:Max>"
  "</tr2:OutputLevelRange>"
  "</tr2:Options>"
  "</tr2:GetAudioOutputConfigurationOptionsResponse>";

const char* getAudioOutputConfigurations2 = 
  "<s:Body>"
  "<tr2:GetAudioOutputConfigurationsResponse>"
  "<tr2:Configurations token=\"AudioOutputConfigurationToken_1\">"
  "<tr2:Name>AudioOutputConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "<tr2:OutputToken>AudioOutputToken_1</tr2:OutputToken>"
  "<tr2:OutputLevel>50</tr2:OutputLevel>"
  "</tr2:Configurations>"
  "</tr2:GetAudioOutputConfigurationsResponse>";

const char* getVideoSourceConfigurations2 = 
  "<s:Body>"
  "<tr2:GetVideoSourceConfigurationsResponse>"
  "<tr2:Configurations token=\"VideoSourceConfigurationToken_1\">"
  "<tr2:Name>VideoSourceConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>2</tr2:UseCount>"
  "<tr2:SourceToken>VideoSourceToken_1</tr2:SourceToken>"
  "<tr2:Bounds x=\"0\" y=\"0\" width=\"1280\" height=\"720\"/>"
  "</tr2:Configurations>"
  "</tr2:GetVideoSourceConfigurationsResponse>";

const char* getVideoEncoderConfigurations2 = 
  "<s:Body>"
  "<tr2:GetVideoEncoderConfigurationsResponse>"
  "<tr2:Configurations "
  "token=\"VideoEncoderConfigurationToken_1\" "
  "GovLength=\"25\" "
  "Profile=\"Main\" "
  "GuaranteedFrameRate=\"false\">"
  "<tr2:Name>VideoEncoderConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "<tr2:Encoding>JPEG</tr2:Encoding>"
  "<tr2:Resolution>"
  "<tr2:Width>1280</tr2:Width>"
  "<tr2:Height>720</tr2:Height>"
  "</tr2:Resolution>"
  "<tr2:RateControl>"
  "<tr2:FrameRateLimit>25</tr2:FrameRateLimit>"
  "<tr2:EncodingInterval>1</tr2:EncodingInterval>"
  "<tr2:BitrateLimit>2048</tr2:BitrateLimit>"
  "</tr2:RateControl>"
  "<tr2:Quality>4</tr2:Quality>"
  "</tr2:Configurations>"
  "</tr2:GetVideoEncoderConfigurationsResponse>";

const char* getAudioInputConfigurations2 = 
  "<s:Body>"
  "<tr2:GetAudioInputConfigurationsResponse>"
  "<tr2:Configurations token=\"AudioInputConfigurationToken_1\">"
  "<tr2:Name>AudioInputConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "<tr2:InputToken>AudioInputToken_1</tr2:InputToken>"
  "<tr2:SendPrimacy>http://www.onvif.org/ver20/HalfDuplex/Client</tr2:SendPrimacy>"
  "</tr2:Configurations>"
  "</tr2:GetAudioInputConfigurationsResponse>";

const char* setAudioInputConfiguration2 = 
  "<s:Body>"
  "<tr2:SetAudioInputConfigurationResponse>"
  "</tr2:SetAudioInputConfigurationResponse>";

const char* getAudioSourceConfigurations2 = 
  "<s:Body>"
  "<tr2:GetAudioSourceConfigurationsResponse>"
  "<tr2:Configurations token=\"AudioSourceConfigurationToken_1\">"
  "<tr2:Name>AudioSourceConfigurationName_1</tr2:Name>"
  "<tr2:UseCount>1</tr2:UseCount>"
  "<tr2:SourceToken>AudioSourceToken_1</tr2:SourceToken>"
  "</tr2:Configurations>"
  "</tr2:GetAudioSourceConfigurationsResponse>";
