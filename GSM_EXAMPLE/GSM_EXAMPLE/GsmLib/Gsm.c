#include "Gsm.h"
#include "GsmConfig.h"

osThreadId 		GsmTaskHandle;
osSemaphoreId GsmSemHandle;

Gsm_t	Gsm;

void	Gsm_InitValue(void)
{
	osDelay(1000);
	Gsm_SetPower(true);													//	turn on module
	osDelay(1000);
  Gsm_UssdCancel();
	#if (_GSM_DUAL_SIM_SUPPORT==1)
	Gsm_SetDefaultSim(1);							
	#endif
	Gsm_MsgSetStoredOnSim(false);								//	select message sored to mudule
	Gsm_MsgGetStatus();													//	read message stored capacity and more
	Gsm_MsgSetTextMode(true);										//	set to text mode
	Gsm_SetEnableShowCallerNumber(true);				//  set enable show caller number
	Gsm_SetConnectedLineIdentification(true);		//	set enable Connected Line Identification
	Gsm_MsgGetSmsServiceCenter();
	Gsm_MsgGetTextModeParameter();
	Gsm_MsgGetTeCharacterset();
	
	if(( Gsm.MsgTextModeFo != 17) || (Gsm.MsgTextModeVp != 167) || (Gsm.MsgTextModePid != 0) || (Gsm.MsgTextModeDcs != 0))
		Gsm_MsgSetTextModeParameter(17,167,0,0);
	#if (_GSM_DUAL_SIM_SUPPORT==1)
	Gsm_SetDefaultSim(2);
  Gsm_MsgSetTextMode(true);
	Gsm_MsgSetStoredOnSim(false);					//	select message sored to mudule
	Gsm_MsgGetSmsServiceCenterDS();
	Gsm_MsgGetTextModeParameter();
	if(( Gsm.MsgTextModeFoDS != 17) || (Gsm.MsgTextModeVpDS != 167) || (Gsm.MsgTextModePidDS != 0) || (Gsm.MsgTextModeDcsDS != 0))
		Gsm_MsgSetTextModeParameter(17,167,0,0);
	Gsm_SetDefaultSim(1);
	#endif	
}
//#########################################################################################################
bool	Gsm_SendRaw(uint8_t *data,uint16_t len)
{
	if(len <= _GSM_TX_SIZE)
	{
		memcpy(Gsm.TxBuffer,data,len);
		if(HAL_UART_Transmit(&_GSM_USART,data,len,100) == HAL_OK)
			return true;
		else
			return false;
	}
	else
		return false;
}
//#########################################################################################################
bool	Gsm_SendString(char *data)
{
	return Gsm_SendRaw((uint8_t*)data,strlen(data));
}
//#########################################################################################################
bool	Gsm_SendStringAndWait(char *data,uint16_t DelayMs)
{
	if(Gsm_SendRaw((uint8_t*)data,strlen(data))==false)
		return false;
	osDelay(DelayMs);
	return true;
}
//#########################################################################################################
bool	Gsm_WaitForString(uint32_t TimeOut_ms,uint8_t *result,uint8_t CountOfParameter,...)
{
	
	if(result == NULL)
		return false;
	if(CountOfParameter == 0)
		return false;
	*result=0;
  va_list tag;
	va_start (tag,CountOfParameter);
	char *arg[CountOfParameter];
	for(uint8_t i=0; i<CountOfParameter ; i++)
		arg[i] = va_arg (tag, char *);	
  va_end (tag);		
	//////////////////////////////////	
	for(uint32_t t=0 ; t<TimeOut_ms ; t+=50)
	{
		osDelay(50);
		for(uint8_t	mx=0 ; mx<CountOfParameter ; mx++)
		{			
			if(strstr((char*)Gsm.RxBuffer,arg[mx])!=NULL)
			{
				*result = mx+1;
				return true;
			}				
		}				
	}
	// timeout
	return false;	
}
//#########################################################################################################
bool	Gsm_ReturnString(char *result,uint8_t WantWhichOne,char *SplitterChars)
{
	if(result == NULL) 
		return false;
	if(WantWhichOne==0)
		return false;

	char *str = (char*)Gsm.RxBuffer;
	

	str = strtok (str,SplitterChars);
	if(str == NULL)
	{
		strcpy(result,"");
		return false;
	}
	while (str != NULL)
  {
    str = strtok (NULL,SplitterChars);
		if(str != NULL)
			WantWhichOne--;
		if(WantWhichOne==0)
		{
			strcpy(result,str);
			return true;
		}
  }
	strcpy(result,"");
	return false;	
}
//#########################################################################################################
bool	Gsm_ReturnStrings(char *InputString,char *SplitterChars,uint8_t CountOfParameter,...)
{
	if(CountOfParameter == 0)
		return false;
	va_list tag;
	va_start (tag,CountOfParameter);
	char *arg[CountOfParameter];
	for(uint8_t i=0; i<CountOfParameter ; i++)
		arg[i] = va_arg (tag, char *);	
  va_end (tag);
	
	char *str;
	str = strtok (InputString,SplitterChars);
	if(str == NULL)
		return false;
	uint8_t i=0;
	while (str != NULL)
  {
    str = strtok (NULL,SplitterChars);
		if(str != NULL)
			CountOfParameter--;
		strcpy(arg[i],str);
		i++;
		if(CountOfParameter==0)
		{
			return true;
		}
  }
	return false;		
}
//#########################################################################################################
bool	Gsm_ReturnInteger(int32_t	*result,uint8_t WantWhichOne,char *SplitterChars)
{
	if((char*)Gsm.RxBuffer == NULL)
		return false;
	if(Gsm_ReturnString((char*)Gsm.RxBuffer,WantWhichOne,SplitterChars)==false)
		return false;
	*result = atoi((char*)Gsm.RxBuffer);
	return true;
}
//#########################################################################################################

bool	Gsm_ReturnFloat(float	*result,uint8_t WantWhichOne,char *SplitterChars)
{	
	if((char*)Gsm.RxBuffer == NULL)
		return false;
	if(Gsm_ReturnString((char*)Gsm.RxBuffer,WantWhichOne,SplitterChars)==false)
		return false;
	*result = atof((char*)Gsm.RxBuffer);
	return true;
}
//#########################################################################################################
void Gsm_RemoveChar(char *str, char garbage)
{
	uint16_t	MaxBuffCounter=_GSM_RX_SIZE;
	char *src, *dst;
  for (src = dst = str; *src != '\0'; src++)
	{
		*dst = *src;
		if (*dst != garbage)
			dst++;
		MaxBuffCounter--;
		if(MaxBuffCounter==0)
			break;
  }
  *dst = '\0';
}
//#########################################################################################################
void	Gsm_RxClear(void)
{
	HAL_UART_Receive_IT(&_GSM_USART,&Gsm.usartBuff,1);
	memset(Gsm.RxBuffer,0,_GSM_RX_SIZE);
	Gsm.RxIndex=0;	
}
//#########################################################################################################
void	Gsm_TxClear(void)
{
	memset(Gsm.TxBuffer,0,_GSM_TX_SIZE);
}
//#########################################################################################################
void	Gsm_RxCallBack(void)
{
	Gsm.RxBuffer[Gsm.RxIndex] = Gsm.usartBuff;
	if(Gsm.RxIndex < _GSM_RX_SIZE)
		Gsm.RxIndex++;
	else
		Gsm.usartRxError=true;
	HAL_UART_Receive_IT(&_GSM_USART,&Gsm.usartBuff,1);
	Gsm.LastTimeRecieved = HAL_GetTick();
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
void GsmTask(void const * argument)
{
	uint8_t GsmResult;
	Gsm_InitValue();
	//#######################	

	while(1)
	{
		if(Gsm.MsgStoredUsed>0)	//	sms on memory
		{
			for(uint8_t i=1 ; i<=100 ; i++)
			{
				if(Gsm_MsgRead(i)==true)
				{
					Gsm_SmsReceiveProcess(Gsm.MsgNumber,Gsm.MsgMessage,Gsm.MsgDate);
					osDelay(100);
					if(Gsm_MsgDelete(i) == false)
					{
						osDelay(100);
						Gsm_MsgDelete(i);
					}
					osDelay(100);
				}
			}			
		}
		Gsm_RxClear();
		if(Gsm_WaitForString(5000,&GsmResult,2,"\r\nRING\r\n","\r\nRINGDS\r\n")==true)
		{
			if((GsmResult == 1) || (GsmResult == 2))		//	Recieve New Call
			{				
				if(Gsm_GetCallerNumber()==true)
				{
					Gsm_CallProcess(Gsm.CallerNumber);
					if(GsmResult==1)
					{
						#if (_GSM_DUAL_SIM_SUPPORT==1)
						Gsm_SetDefaultSim(1);
						#endif
						Gsm_DisconnectVoiceCall();
					}
					if(GsmResult==2)
					{
						#if (_GSM_DUAL_SIM_SUPPORT==1)
						Gsm_SetDefaultSim(2);
						#endif
						Gsm_DisconnectVoiceCall();
					}
				}
			}					
		}
		Gsm_MsgGetStatus();
		osDelay(100);
		Gsm_SignalQuality();
		osDelay(100);
		#if (_GSM_DUAL_SIM_SUPPORT==1)
		Gsm_SignalQualityDS();
		osDelay(100);
		#endif
		Gsm_UserProcess();
		osDelay(100);
		if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
		{
			osDelay(100);
			if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
			{
				Gsm.PowerState=false;
				Gsm_SetPower(true);
			}			
		}
		if(Gsm.SignalQuality == GsmSignalQuality_0)
		{
			Gsm.SignalQualityCounter++;
			if(Gsm.SignalQualityCounter>10)
			{
				Gsm_SetPower(false);
				osDelay(5000);	
				Gsm_InitValue();
				Gsm.SignalQualityCounter=0;				
			}				
		}
		else
			Gsm.SignalQualityCounter=0;
	}
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
void	Gsm_Init(osPriority	Priority)
{
	HAL_UART_Receive_IT(&_GSM_USART,&Gsm.usartBuff,1);
	Gsm_RxClear();
	Gsm_TxClear();
	osSemaphoreDef(GsmSemHandle);
  GsmSemHandle = osSemaphoreCreate(osSemaphore(GsmSemHandle), 1);
	osThreadDef(GsmTaskName, GsmTask, Priority, 0, _GSM_TASK_SIZE);
  GsmTaskHandle = osThreadCreate(osThread(GsmTaskName), NULL);	
}
//#########################################################################################################
bool	Gsm_SetPower(bool ON_or_OFF)
{
	if(ON_or_OFF==false)	// Power OFF
	{
		if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
		{
			Gsm.PowerState = true;
			return true;
		}
		else
		{
			HAL_GPIO_WritePin(_GSM_POWER_PORT,_GSM_POWER_PIN,GPIO_PIN_RESET);
			osDelay(1200);
			HAL_GPIO_WritePin(_GSM_POWER_PORT,_GSM_POWER_PIN,GPIO_PIN_SET);
			osDelay(200);
			if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
			{
				Gsm.PowerState = true;
				return true;
			}
			else
			{
				Gsm.PowerState = false;
				return false;			
			}
		}		
	}
	else								//	Power ON
	{
		if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
		{	
			HAL_GPIO_WritePin(_GSM_POWER_PORT,_GSM_POWER_PIN,GPIO_PIN_RESET);
			osDelay(1200);
			HAL_GPIO_WritePin(_GSM_POWER_PORT,_GSM_POWER_PIN,GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(_GSM_POWER_STATUS_PORT,_GSM_POWER_STATUS_PIN)==GPIO_PIN_RESET)
			{
				Gsm.PowerState = false;
				return false;
			}
			osDelay(3000);
			//	init AtCommands
			Gsm_SendStringAndWait("AT\r\n",1000);
			Gsm_SendStringAndWait("AT\r\n",1000);		
			Gsm_SendStringAndWait("AT+CFUN=1\r\n",1000);				
			Gsm.PowerState = true;
			uint8_t result;
			Gsm_WaitForString(_GSM_WAIT_TIME_VERYHIGH,&result,1,"Call Ready");
			return true;			
		}
		else
		{
			HAL_GPIO_WritePin(_GSM_POWER_PORT,_GSM_POWER_PIN,GPIO_PIN_SET);
			Gsm.PowerState = true;
			return true;			
		}
	}
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
bool	Gsm_SetDefaultProfile(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"ATZ0\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;
}
//#########################################################################################################
bool	Gsm_SetDefaultFactory(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT&F0\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_SaveConfig(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT&W\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_SignalQuality(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSQ\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		if(Gsm_ReturnInteger((int32_t*)&result,1,":")==false)
			break;
			
		if(result == 99)
			Gsm.SignalQuality = GsmSignalQuality_0;
		else if(result<8)
			Gsm.SignalQuality = GsmSignalQuality_25;
		else if((result>=8) && (result<16))
			Gsm.SignalQuality = GsmSignalQuality_50;
		else if((result>=16) && (result<24))
			Gsm.SignalQuality = GsmSignalQuality_75;
		else if((result>=24) && (result<32))
			Gsm.SignalQuality = GsmSignalQuality_100;

		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
#if (_GSM_DUAL_SIM_SUPPORT==1)
bool	Gsm_SignalQualityDS(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSQDS\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		if(Gsm_ReturnInteger((int32_t*)&result,1,":")==false)
			break;
			
		if(result == 99)
			Gsm.SignalQualityDS = GsmSignalQuality_0;
		else if(result<8)
			Gsm.SignalQualityDS = GsmSignalQuality_25;
		else if((result>=8) && (result<16))
			Gsm.SignalQualityDS = GsmSignalQuality_50;
		else if((result>=16) && (result<24))
			Gsm.SignalQualityDS = GsmSignalQuality_75;
		else if((result>=24) && (result<32))
			Gsm.SignalQualityDS = GsmSignalQuality_100;

		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
#endif
//#########################################################################################################
bool	Gsm_SetTime(uint8_t Year,uint8_t Month,uint8_t Day,uint8_t Hour,uint8_t Min,uint8_t Sec,int8_t GMT_inQuarter)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CCLK=\"%02d/%02d/%02d,%02d:%02d:%02d%+02d\"\r\n",Year,Month,Day,Hour,Min,Sec,GMT_inQuarter);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_GetTime(uint8_t *Year,uint8_t *Month,uint8_t *Day,uint8_t *Hour,uint8_t *Min,uint8_t *Sec)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CCLK?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;	
		sscanf((char*)Gsm.RxBuffer,"AT+CCLK?\r\r\n+CCLK: \"%d/%d/%d,%d:%d:%d\":",(int*)Year,(int*)Month,(int*)Day,(int*)Hour,(int*)Min,(int*)Sec);
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
#if (_GSM_DUAL_SIM_SUPPORT==1)
bool	Gsm_SetDefaultSim(uint8_t	SelectedSim_1_or_2)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CDSDS=%d\r\n",SelectedSim_1_or_2);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		Gsm.DefaultSim = SelectedSim_1_or_2;
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_GetDefaultSim(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CDSDS?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		if(strstr((char*)Gsm.RxBuffer,"SIM1")!=NULL)
			Gsm.DefaultSim = 1;
		if(strstr((char*)Gsm.RxBuffer,"SIM2")!=NULL)
			Gsm.DefaultSim = 2;
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
#endif
//#########################################################################################################
bool  Gsm_Ussd(char *SendUssd,char *Answer)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CUSD=1,\"%s\"\r\n",SendUssd);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_VERYHIGH,&result,2,"+CUSD:","ERROR")==false)
		{
      sprintf((char*)Gsm.TxBuffer,"AT+CUSD=2\r\n");
      Gsm_SendString((char*)Gsm.TxBuffer);
      Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR");
			break;
    }
		if(result == 2)
			break;
      
    Gsm_RemoveChar((char*)Gsm.RxBuffer,'"');
    char *s = strstr((char*)Gsm.RxBuffer,"+CUSD: ");
    if(s==NULL)
      break;
    s+=strlen("+CUSD: ");
    s+=2;
    strcpy(Answer,s);
    returnVal=true;	
	}while(0);
  sprintf((char*)Gsm.TxBuffer,"AT+CUSD=2\r\n");
  Gsm_SendString((char*)Gsm.TxBuffer);
  Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR");
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	  
}
//#########################################################################################################
bool  Gsm_UssdCancel(void)
{
  osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CUSD=2\r\n");
    Gsm_SendString((char*)Gsm.TxBuffer);
    if(Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR")==false)
      break;
    if(result == 2)
      break;
    returnVal=true;
  }while(0);  
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	  
  
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
bool	Gsm_Answer(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"ATA\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_WaitForDisconnectCall(uint16_t WaitSecond)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		if(Gsm_WaitForString(WaitSecond*1000,&result,1,"NO CARRIER")==false)
		{
			sprintf((char*)Gsm.TxBuffer,"AT+HVOIC\r\n");
			if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
				break;
			if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
				break;
			break;
		}		
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_Dial(char *DialNumber,uint8_t WaitForAnswer_Second)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		if(WaitForAnswer_Second>65)
			WaitForAnswer_Second=65;
		if(DialNumber==NULL)
			break;
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"ATD%s;\r\n",DialNumber);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(WaitForAnswer_Second*1000,&result,7,"ERROR","OK","NO DIALTONE","BUSY","NO CARRIER","NO ANSWER","CONNECT")==false)
		{
			sprintf((char*)Gsm.TxBuffer,"AT+HVOIC\r\n");
			Gsm_SendString((char*)Gsm.TxBuffer);
			break;
		}
		if(result == 1)
			break;
			
		switch (result)
		{
			case 2:
				Gsm.DialAnswer = GsmDial_Answer;
			break;
			case 3:
				Gsm.DialAnswer = GsmDial_NoDialTone;
			break;
			case 4:
				Gsm.DialAnswer = GsmDial_Busy;
			break;
			case 5:
				Gsm.DialAnswer = GsmDial_NoCarrier;
			break;
			case 6:
				Gsm.DialAnswer = GsmDial_NoAnswer;
			break;
			case 7:
				Gsm.DialAnswer = GsmDial_Data;
			break;	
		}
		returnVal=true;	
	}while(0);
	
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;
}

//#########################################################################################################
bool	Gsm_DisconnectAll(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"ATH\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_DisconnectVoiceCall(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+HVOIC\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_DtmfGereration(char *SendingNumber)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+VTS=%s,2\r\n",SendingNumber);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW+200*strlen(SendingNumber),&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_GetCallerNumber(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	bool		returnVal=false;
	do
	{
		if(Gsm.CallerID == true)
		{
			char *str = strstr((char*)Gsm.RxBuffer,"\r\n+CLIP");
			if(str == NULL)
				break;
			Gsm_RemoveChar(str,'"');
			if(Gsm_ReturnString(Gsm.CallerNumber,1,":,")==false)
				break;
			Gsm_RemoveChar(Gsm.CallerNumber,' ');
			returnVal=true;		
		}
		else
			returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_SetEnableShowCallerNumber(bool	Enable)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CLIP=%d\r\n",Enable);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
		Gsm.CallerID=Enable;
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_SetConnectedLineIdentification(bool Enable)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+COLP=%d\r\n",Enable);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
		Gsm.CallerID=Enable;
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
bool	Gsm_MsgSetTextMode(bool	TextMode)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CMGF=%d\r\n",TextMode);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;	
   	#if (_GSM_DUAL_SIM_SUPPORT==0)
		Gsm.MsgTextMode=TextMode;
    #else
    if(Gsm.DefaultSim==1)
      Gsm.MsgTextMode=TextMode;
    else
      Gsm.MsgTextModeDS=TextMode;
    #endif
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_MsgGetTextMode(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CMGF?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		if(Gsm_ReturnInteger((int32_t*)&result,1,":")==false)
			break;
		Gsm.MsgTextMode=(bool)result;
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_MsgDeleteAll(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CMGD=1,4\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_HIGH,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	Gsm_MsgGetStatus();
	return returnVal;
}
//#########################################################################################################
bool	Gsm_MsgDelete(uint8_t MsgIndex)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CMGD=%d,0\r\n",MsgIndex);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_HIGH,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	Gsm_MsgGetStatus();
	return returnVal;
}
//#########################################################################################################
bool	Gsm_MsgSetStoredOnSim(bool	OnSimCard)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		if(OnSimCard==false)
			sprintf((char*)Gsm.TxBuffer,"AT+CPMS=\"ME\",\"ME\",\"ME\"\r\n");
		else
			sprintf((char*)Gsm.TxBuffer,"AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;
}
//#########################################################################################################
bool	Gsm_MsgGetStatus(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
		
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CPMS?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
	
		char str[3];
		sscanf((char*)Gsm.RxBuffer,"AT+CPMS?\r\r\n+CPMS: \"%2s\",%d,%d,",str,(int*)&Gsm.MsgStoredUsed,(int*)&Gsm.MsgStoredCapacity);
		if(strstr(str,"SM")!=NULL)
			Gsm.MsgStoredOnSim = true;
		else
			Gsm.MsgStoredOnSim = false;
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_MsgRead(uint16_t MsgIndex)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	char *str;
	bool		returnVal=false;
	do
	{
		strcpy(Gsm.MsgMessage,"");
		strcpy(Gsm.MsgDate,"");
		strcpy(Gsm.MsgNumber,"");
		
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CMGR=%d\r\n",MsgIndex);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;	
	
		osDelay(100);
		Gsm_RemoveChar((char*)Gsm.RxBuffer,'"');
		str=strchr((char*)Gsm.RxBuffer,',');
		if(str == NULL)
			break;

		str++;
		uint16_t msglen;
		sscanf(str,"%[^,],,%[^+]%d\r\n%[^\r]",Gsm.MsgNumber,Gsm.MsgDate,(int*)&msglen,Gsm.MsgMessage);	
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;
}
//#########################################################################################################
bool	Gsm_MsgSend(char *Number,char *message)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	Gsm.MsgSendDone=false;
	do
	{
   	#if (_GSM_DUAL_SIM_SUPPORT==0) 
		if(Gsm.MsgTextMode==false)
		{


		}
    #else
    if((Gsm.DefaultSim == 1) && (Gsm.MsgTextMode==false))
		{
      

		}
    if((Gsm.DefaultSim == 2) && (Gsm.MsgTextModeDS==false))
		{
      

		}
    #endif
		else
		{
			Gsm_RxClear();
			sprintf((char*)Gsm.TxBuffer,"AT+CMGS=\"%s\"\r\n",Number);
			if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
				break;
			if(Gsm_WaitForString(_GSM_WAIT_TIME_VERYHIGH,&result,2,">","ERROR")==false)
				break;
			if(result == 2)
				break;
			Gsm_RxClear();
			sprintf((char*)Gsm.TxBuffer,"%s%c%c",message,26,0);
			if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
				break;
			if(Gsm_WaitForString(_GSM_WAIT_TIME_VERYHIGH,&result,2,"OK","ERROR")==false)
				break;
			if(result == 2)
				break;	
			Gsm.MsgSendDone=true;			
			returnVal=true;	
		}		
	}while(0);	
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//########################################################################################################
bool	Gsm_MsgSetTextModeParameter(uint8_t fo,uint8_t vp,uint8_t pid,uint8_t dcs)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSMP=%d,%d,%d,%d\r\n",fo,vp,pid,dcs);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
    #if (_GSM_DUAL_SIM_SUPPORT==0)
		Gsm.MsgTextModeFo = fo;
		Gsm.MsgTextModeVp = vp;
		Gsm.MsgTextModePid = pid;
		Gsm.MsgTextModeDcs = dcs;
    #else
    if(Gsm.DefaultSim==1)
    {
      Gsm.MsgTextModeFo = fo;
      Gsm.MsgTextModeVp = vp;
      Gsm.MsgTextModePid = pid;
      Gsm.MsgTextModeDcs = dcs;
    }
    else
    {
      Gsm.MsgTextModeFoDS = fo;
      Gsm.MsgTextModeVpDS = vp;
      Gsm.MsgTextModePidDS = pid;
      Gsm.MsgTextModeDcsDS = dcs;
    }
    #endif
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//########################################################################################################
bool	Gsm_MsgGetTextModeParameter(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSMP?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		
		char *str=strchr((char*)Gsm.RxBuffer,':');
		if(str == NULL)
			break;
		str++;
		#if (_GSM_DUAL_SIM_SUPPORT==0)
		sscanf(str,"%d,%d,%d,%d",(int*)&Gsm.MsgTextModeFo,(int*)&Gsm.MsgTextModeVp,(int*)&Gsm.MsgTextModePid,(int*)&Gsm.MsgTextModeDcs);	
		#else
			if(Gsm.DefaultSim == 1)
				sscanf(str,"%d,%d,%d,%d",(int*)&Gsm.MsgTextModeFo,(int*)&Gsm.MsgTextModeVp,(int*)&Gsm.MsgTextModePid,(int*)&Gsm.MsgTextModeDcs);	
			else
				sscanf(str,"%d,%d,%d,%d",(int*)&Gsm.MsgTextModeFoDS,(int*)&Gsm.MsgTextModeVpDS,(int*)&Gsm.MsgTextModePidDS,(int*)&Gsm.MsgTextModeDcsDS);	
		#endif
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_MsgGetSmsServiceCenter(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSCA?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		Gsm_RemoveChar((char*)Gsm.RxBuffer,'"');
		Gsm_RemoveChar((char*)Gsm.RxBuffer,' ');
		Gsm_ReturnString(Gsm.MsgServiceCenter,1,":,");
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
#if (_GSM_DUAL_SIM_SUPPORT==1)
bool	Gsm_MsgGetSmsServiceCenterDS(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSCA?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		Gsm_RemoveChar((char*)Gsm.RxBuffer,'"');
		Gsm_RemoveChar((char*)Gsm.RxBuffer,' ');
		Gsm_ReturnString(Gsm.MsgServiceCenterDS,1,":,");
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
#endif
//#########################################################################################################
bool	Gsm_MsgSetSmsServiceCenter(char *ServiceCenter)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSCA=\"%s\"\r\n",ServiceCenter);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_MED,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		sprintf(Gsm.MsgServiceCenter,"%s",ServiceCenter);
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_MsgSetTeCharacterset(char *TeCharacterSet)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSCS=\"%s\"\r\n",TeCharacterSet);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		sprintf(Gsm.MsgTeCharacterSet,"%s",TeCharacterSet);
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_MsgGetTeCharacterset(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSCS?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		Gsm_RemoveChar((char*)Gsm.RxBuffer,'"');
		Gsm_RemoveChar((char*)Gsm.RxBuffer,' ');
		Gsm_ReturnString(Gsm.MsgTeCharacterSet,2,":\r\n,");
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_MsgRestoreSettings(uint8_t selectProfile_0_to_3)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CRES=%d\r\n",selectProfile_0_to_3);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_MsgSaveSettings(uint8_t selectProfile_0_to_3)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{	
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CSAS=%d\r\n",selectProfile_0_to_3);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
bool	Gsm_SetWhiteNumberOff(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CWHITELIST=0\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;		
}
//#########################################################################################################
bool	Gsm_SetWhiteNumberEmpty(void)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CWHITELIST=3,1,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
}
//#########################################################################################################
bool	Gsm_SetWhiteNumber(uint8_t	Index_1_to_30,char *PhoneNumber)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CWHITELIST=3,%d,%s\r\n",Index_1_to_30,PhoneNumber);
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
			
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
	
}
//#########################################################################################################
bool	Gsm_GetWhiteNumber(uint8_t	Index_1_to_30,char *PhoneNumber)
{
	osSemaphoreWait(GsmSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
		
	do
	{
		if(PhoneNumber == NULL)
			break;
		strcpy(PhoneNumber,"");
		if((Index_1_to_30>30) || (Index_1_to_30==0))
			break;
		Gsm_RxClear();
		sprintf((char*)Gsm.TxBuffer,"AT+CWHITELIST?\r\n");
		if(Gsm_SendString((char*)Gsm.TxBuffer)==false)
			break;
		if(Gsm_WaitForString(_GSM_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;		
		if(Gsm_ReturnString(PhoneNumber,Index_1_to_30,",")==false)
			break;
	
		Gsm_RemoveChar(PhoneNumber,'"');
		returnVal=true;		
	}while(0);
	osSemaphoreRelease(GsmSemHandle);
	return returnVal;	
		
	
}
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################
//#########################################################################################################

