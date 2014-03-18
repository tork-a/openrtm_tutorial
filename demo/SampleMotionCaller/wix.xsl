<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
   xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
   xmlns:wix="http://schemas.microsoft.com/wix/2006/wi">
  <xsl:output indent="yes" method="xml"/>

  <xsl:template match="/wix:Wix">
    <Wix xmlns="http://schemas.microsoft.com/wix/2006/wi">
      <Product xmlns="http://schemas.microsoft.com/wix/2006/wi" Id="45F1B513-2389-4E01-AE24-5514743C033B" Name="SampleMotionCaller 1.0.0" Language="1041" Codepage="932" Version="1.0.0" Manufacturer="AIST" UpgradeCode="386ACBB1-9E92-47FB-924C-9C48114B5BA5">
        <Package InstallerVersion="300" Compressed="yes" Languages='1041' SummaryCodepage='932' />
        <Media Id="1" Cabinet="SampleMotionCaller.cab" EmbedCab="yes" />
        <Directory Id="TARGETDIR" Name="SourceDir" >
            <Directory Id="ProgramFilesFolder" Name="PFILE" >
                <Directory Id="OPENRTM_DIR" Name="OpenRTM-aist" >
                    <Directory Id="INSTALLLOCATION" Name="1.1" />
                </Directory>
            </Directory>
        </Directory>

        <Feature Id="APPLICATIONS" Title="Applications" Level="1" Description="RT-Component applications" >
          <!-- Start Ripping through the xml -->
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\SampleMotionCaller.dll')]" />
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\SampleMotionCaller.lib')]" />
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\SampleMotionCallerComp.exe')]" />
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\RTC.xml')]" />
        </Feature>

        <Feature Id="DOCUMENTS" Title="Documents" Level="1" Absent="allow" Description="RT-Component documents" >
          <!-- Start Ripping through the xml -->
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\doc')]" />
        </Feature>

        <Feature Id="SOURCES" Title="Source files" Level="1" Absent="allow" Description="RT-Component source files" >
          <!-- Start Ripping through the xml -->
          <xsl:apply-templates select="wix:Fragment/wix:DirectoryRef/wix:Component[contains(wix:File/@Source, 'samplemotioncaller1.0.0\src')]" />
        </Feature>

        <!--Tack on your specific wix options-->
        <UIRef Id="WixUI_FeatureTree" />
        <UIRef Id="WixUI_ErrorProgressText" />
        <WixVariable Id="WixUILicenseRtf" Value="C:/Users/n130s/data/ExistingRsc_RtmUSB_201312/demo/SampleMotionCaller/cpack_resources/License.rtf"/>
        <!-- TODO: Add Wix Specific Dialogs and features. -->
        <!-- TODO: Add artwork  -->
        <!-- TODO: Add ... -->

      </Product>

      <!--Output the fragment info which heat generates-->
      <xsl:copy-of select="wix:Fragment[wix:DirectoryRef/wix:Component]" />
      <xsl:apply-templates select="wix:Fragment[wix:DirectoryRef/@Id!='TARGETDIR' and wix:DirectoryRef/wix:Directory]" />

    </Wix>
  </xsl:template>

  <xsl:template match="wix:Fragment[wix:DirectoryRef/wix:Directory]" >
    <xsl:copy>
      <xsl:apply-templates select="wix:DirectoryRef" />
    </xsl:copy>
  </xsl:template>

  <xsl:template match="wix:DirectoryRef" >
    <xsl:copy>
      <xsl:choose>
        <xsl:when test="wix:Directory[@Name='components']" >
          <xsl:attribute name="Id">INSTALLLOCATION</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="Id"><xsl:value-of select="@Id" /></xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates />
    </xsl:copy>
  </xsl:template>

  <xsl:template match="wix:Directory" >
    <xsl:copy>
      <xsl:attribute name="Id"><xsl:value-of select="@Id" /></xsl:attribute>
      <xsl:attribute name="Name"><xsl:value-of select="@Name" /></xsl:attribute>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="wix:Component">
    <xsl:element name="ComponentRef" xmlns="http://schemas.microsoft.com/wix/2006/wi" >
      <xsl:attribute name="Id">
        <xsl:value-of select="@Id" />
      </xsl:attribute>
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
