#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
int main()
{

    HANDLE hComm;
    BOOL   Status;
    DCB dcbSerialParams = { 0 };
    char ArduinoMessage[64] = { 0 };
    DWORD BytesWritten = 0;


    //Navnet COM3 kommer an på hvilken serial port arduionen er sat til
    //Serial monitoren og dette c program kan ikke køre samtidig,
    //da der kun kan være 1 enhed på serial monitoren på samme tid.
    //Nogle gange før at arduionen kan læse beskeden fra dette c program
    //skal man først sende en tilfældig besked igennem serial monitoren, er ikke helt sikker på hvorfor

    hComm = CreateFileA("\\\\.\\COM3",                //port name
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,// Open existing port only
                      0,            // Non Overlapped I/O
                      NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)

        //Hvis der ikke kan forbindes til serial port
        printf("Cannot connect to serial port, either wrong serial port name, or the serial monitor is open");
    else
        //Hvis der bliver forbundet til serial port
        printf("Welcome");
        printf("\nCommands:");
        printf("\nTo change the sensor limits type 'set:humidityvalue:tempvalue:lightvalue' example 'set:60:25:700'");
        printf("\nTo change the time limits type 'timeset:min:max' example 'timeset:5:8'");
        printf("\nTo go into manual mode type 'control' and to exit manual mode type 'controloff'");
        printf("\nWhen in manual mode, you can control the lights with the following commands'");
        printf("\nhumion and humioff to change the humidity LED");
        printf("\ntempon and tempoff to change the temperature LED");
        printf("\nlighton and lightoff to change the light LED");
        printf("\nTo update the time, type 'time'");
        printf("\nType 'quit' to quit\n");


    while(TRUE)
    {
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        Status = GetCommState(hComm, &dcbSerialParams);


    //Skriver tiden til en char
    time_t current_time;
    struct tm * time_info;
    char timeString[9];

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);


    scanf_s("%s", ArduinoMessage, (unsigned)_countof(ArduinoMessage));

    if (strcmp("time", ArduinoMessage) == 0)
    {
        strcat(ArduinoMessage, ":");
        strcat(ArduinoMessage, timeString);
    }
    if (strcmp("quit", ArduinoMessage) == 0)
    {
        break;
    }

    //Besked som bliver sendt til arduino
    Status = WriteFile(hComm,
    ArduinoMessage,
    sizeof(ArduinoMessage),
    &BytesWritten,
    NULL);

    }

    CloseHandle(hComm);//Closing the Serial Port

    return 0;
}
