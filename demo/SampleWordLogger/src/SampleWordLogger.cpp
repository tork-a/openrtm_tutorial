// -*- C++ -*-
/*!
 * @file  SampleWordLogger.cpp
 * @brief SampleWordLogger image component
 * @date $Date$
 *
 * $Id$
 */

#include "SampleWordLogger.h"

#include <objbase.h>
#include <XmlLite.h>
#include <codecvt>


// Module specification
// <rtc-template block="module_spec">
static const char* SampleWordLogger_spec[] =
  {
    "implementation_id", "SampleWordLogger",
    "type_name",         "SampleWordLogger",
    "description",       "SampleWordLogger component",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.SampleWordLoggerFileName", "SampleWord.log",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SampleWordLogger::SampleWordLogger(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_resultIn("result", m_result)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SampleWordLogger::~SampleWordLogger()
{
}

RTC::ReturnCode_t SampleWordLogger::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("result", m_resultIn);
  
 
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("SampleWordLoggerFileName", m_SampleWordLoggerFileName, "SampleWord.log");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SampleWordLogger::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SampleWordLogger::onActivated(RTC::UniqueId ec_id)
{
  // Create a file for logging.
  std::cerr << "Opening Log File ... " << m_SampleWordLoggerFileName << std::endl;
  std::locale::global(std::locale(""));
  m_SampleWordFileStream.open(m_SampleWordLoggerFileName);
  if(!m_SampleWordFileStream) {
      std::cerr<<"Error opening file"<< std::endl; 
	  return RTC::RTC_ERROR;
  }

  m_SampleWordFileStream.imbue(std::locale(m_SampleWordFileStream.getloc(),new std::codecvt_utf8_utf16<wchar_t>));

  return RTC::RTC_OK;
}


RTC::ReturnCode_t SampleWordLogger::onDeactivated(RTC::UniqueId ec_id)
{
  // close file
  m_SampleWordFileStream.close();

  return RTC::RTC_OK;
}


RTC::ReturnCode_t SampleWordLogger::onExecute(RTC::UniqueId ec_id)
{
   // Check if the incoming data is new.
   if (m_resultIn.isNew()) {
     // Read in from InPort.
     m_resultIn.read();
 
	 SYSTEMTIME systime;
	 GetLocalTime(&systime);
	 static char sDate[128];
	 sprintf(sDate, "%04d/%02d/%02d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	 std::cerr << "Writing... " << std::endl; 
	 std::cerr << "    " << sDate << std::endl;
	 m_SampleWordFileStream << sDate << " ";

	 HRESULT hr = S_OK;
	 IXmlReader *pReader = NULL;
     XmlNodeType nodeType;
	 IStream *pStream;
	 CreateStreamOnHGlobal(NULL, TRUE, &pStream);
	 LARGE_INTEGER liBeggining = { 0 };
	 pStream->Seek(liBeggining, STREAM_SEEK_SET, NULL);
	 ULONG ulBytesWritten = 0;
	 ULONG ulSize = 0;
	 ULARGE_INTEGER uliSize = { 0 };
	 pStream -> SetSize (uliSize);
	 ulSize = (ULONG)strlen(m_result.data);
	 pStream->Write((void const*)m_result.data,(ULONG)ulSize, (ULONG*)&ulBytesWritten);
	 pStream->Seek(liBeggining, STREAM_SEEK_SET, NULL);

	 if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
	 {
        wprintf(L"Error creating xml reader, error is %08.8lx", hr);
		return RTC::RTC_ERROR;
	 }
	 
	 if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
	 {
        wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
		return RTC::RTC_ERROR;
	 }
	 
	 if (FAILED(hr = pReader->SetInput(pStream)))
	 {
        wprintf(L"Error setting input for reader, error is %08.8lx", hr);
        return RTC::RTC_ERROR;
	 }


	 bool firstText = true;
	 while (S_OK == (hr = pReader->Read(&nodeType)))
	 {
        LPCWSTR pwszPrefix = NULL;
        UINT cwchPrefix = 0;
        LPCWSTR pwszLocalName = NULL;
        LPCWSTR pwszValue = NULL;

		switch (nodeType)
		{
		case XmlNodeType_Element:
			if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			{
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				return RTC::RTC_ERROR;
			}
			if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			{
				 wprintf(L"Error getting local name, error is %08.8lx", hr);
				 return RTC::RTC_ERROR;
		    }
			if (cwchPrefix > 0)
				wprintf(L"Element: %s:%s\n", pwszPrefix, pwszLocalName);
			else
				wprintf(L"Element: %s\n", pwszLocalName);

			// Attributes
			const WCHAR* pwszPrefix;
			const WCHAR* pwszLocalName;
			const WCHAR* pwszValue;
			hr = pReader->MoveToFirstAttribute();
			while (TRUE)
			{
				if (!pReader->IsDefault())
				{
					UINT cwchPrefix;
					if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
					{
						wprintf(L"Error getting prefix, error is %08.8lx", hr);
						return RTC::RTC_ERROR;
					}
					if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
					{
						wprintf(L"Error getting local name, error is %08.8lx", hr);
						return RTC::RTC_ERROR;
					}
					if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return RTC::RTC_ERROR;
					}
					if (cwchPrefix > 0)
						wprintf(L"Attr: %s:%s=\"%s\" \n", pwszPrefix, pwszLocalName, pwszValue);
					else
						wprintf(L"Attr: %s=\"%s\" \n", pwszLocalName, pwszValue);
					
					if ( firstText && wcscmp(pwszLocalName, L"text") == 0 ) {
						m_SampleWordFileStream << pwszValue << std::endl;
						firstText = false;
					}
					//m_SampleWordFileStream << "  " <<  pwszLocalName << ":" << pwszValue << std::endl;
				}

				if (S_OK != pReader->MoveToNextAttribute())
					break;
			}

			if (pReader->IsEmptyElement() )
				wprintf(L" (empty)");
			 break;
		 case XmlNodeType_EndElement:
			 if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
			 {
				wprintf(L"Error getting prefix, error is %08.8lx", hr);
				return RTC::RTC_ERROR;
		     }
			 if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
			 {
				wprintf(L"Error getting local name, error is %08.8lx", hr);
				return RTC::RTC_ERROR;
			 }
			 if (cwchPrefix > 0)
				wprintf(L"End Element: %s:%s\n", pwszPrefix, pwszLocalName);
			 else
				wprintf(L"End Element: %s\n", pwszLocalName);
			 break;
		}
	 }       
	//
   }
   return RTC::RTC_OK;  
}

/*
RTC::ReturnCode_t SampleWordLogger::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SampleWordLogger::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{ 
  void SampleWordLoggerInit(RTC::Manager* manager)
  {
    coil::Properties profile(SampleWordLogger_spec);
    manager->registerFactory(profile,
                             RTC::Create<SampleWordLogger>,
                             RTC::Delete<SampleWordLogger>);
  }  
};


