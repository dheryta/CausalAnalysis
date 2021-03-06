/*
 tshark -E separator=, -T fields -e frame.time_epoch -e wlan.fc.type_subtype -e prism.did.channel -e wlan.sa -e wlan.da -e wlan.bssid -e wlan.ta -e wlan.ra -e wlan.duration -e prism.did.rate -e wlan.fc.retry -r WR6.pcap > WR6.csv
2009
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Max_Frames 10000000
#define SIFS 0.000011

struct PCAP_Data
{
double frame_time_epoch;
char wlan_fc_type_subtype[5];
int prism_did_channel;
char wlan_sa[18];
char wlan_da[18];
char wlan_bssid[18];
char wlan_ta[18];
char wlan_ra[18];
double wlan_duration;
int frame_len;
int prism_did_rate;
int wlan_fc_retry;
int wlan_mgt_ds_current_channel;
char wlan_mgt_ssid[33];
}PCAP_DATA_VAL[Max_Frames];


unsigned long framesCount=0;

void LoadPCAPData(char *pcap);
void FindPreqPres();

void main( int argc, char *argv[] )
{
	if ( argc != 2 )
    	{        	
        	printf( "usage: %s filename\n", argv[0] );
    	}
	else
	{
		LoadPCAPData(argv[1]);
		FindPreqPres();
	}

}

//Load Pcap into Structure Array
void LoadPCAPData(char *pcap)
{
unsigned long frame=-1;
FILE *file = fopen(pcap, "r" );
		if (file == NULL)
		{
			printf("\n Unable to read file");
			return;
		}
		char buffer[4095];
		char *token;
		char * end;
		double temp;
		while(fgets(buffer, 4095, file) != NULL)
    		{
		    frame=frame+1;
		    token = strtok(buffer, ",");
		    if (token != NULL)
		    {
		    temp = strtod (token, & end);
			if (end == token)
			PCAP_DATA_VAL[frame].frame_time_epoch = -1;
			else
   		        PCAP_DATA_VAL[frame].frame_time_epoch =  temp;
                    }
   
		    token = strtok(NULL, ",");
		    if (token != NULL)
   		    strcpy(PCAP_DATA_VAL[frame].wlan_fc_type_subtype, token);

                    token = strtok(NULL, ",");
		    if (token != NULL)
   		    strcpy(PCAP_DATA_VAL[frame].wlan_sa, token);

		    token = strtok(NULL, ",");
		    if (token != NULL)
   		    strcpy(PCAP_DATA_VAL[frame].wlan_da, token);

		    token = strtok(NULL, ",");
		    if (token != NULL)
   		    PCAP_DATA_VAL[frame].frame_len= atoi(token);
		    
		    
		}
		framesCount=frame;
		fclose(file);

}

void FindPreqPres()
{
int eFrame=0;	
int nextFrame;
char *success;
int illegalFrames=0;
int totalFrames=0;
	while(eFrame<framesCount)
	{

	if(strcmp(PCAP_DATA_VAL[eFrame].wlan_fc_type_subtype,"0x04") == 0 && 
        strcmp(PCAP_DATA_VAL[eFrame+1].wlan_fc_type_subtype,"0x05") == 0)
		{		
		int nFrame = eFrame + 1;
		int probeResponse = 0;

		while((nFrame<framesCount)&&
		     strcmp(PCAP_DATA_VAL[nFrame].wlan_da,PCAP_DATA_VAL[eFrame].wlan_sa) == 0  && 
		     strcmp(PCAP_DATA_VAL[nFrame].wlan_fc_type_subtype,"0x05") == 0)
			{				
			probeResponse ++;				
			nFrame ++;			
			}
		if(probeResponse > 0)
		printf("%d\n",probeResponse);
		eFrame = nFrame;
		}
	else
		eFrame++;
	}
}

