
export function DFLOG(StringToLog: string, LogVerbosity: EDataFactoryLogVerbosity, bPrintToScreen: boolean, bPrintToLog: boolean, Duration: number, MaxStackDataDepth: number): void
{
    DataFactoryBPLibrary.DF_PrintString(undefined, StringToLog, LogVerbosity, bPrintToScreen, bPrintToLog, Duration, MaxStackDataDepth);

}

export function DFLOG_ToScreen(StringToLog: string,
    LogVerbosity: EDataFactoryLogVerbosity = EDataFactoryLogVerbosity.Log,
    bPrintToScreen: boolean = true,
    bPrintToLog: boolean = false,
    Duration: number = 2.0,
    MaxStackDataDepth: number = 1): void
{
    DataFactoryBPLibrary.DF_PrintString(undefined, StringToLog, LogVerbosity, bPrintToScreen, bPrintToLog, Duration, MaxStackDataDepth);

}

export function DFLOG_ToConsole(StringToLog: string,
    LogVerbosity: EDataFactoryLogVerbosity = EDataFactoryLogVerbosity.Log,
    bPrintToScreen: boolean = false,
    bPrintToLog: boolean = true,
    Duration: number = 0,
    MaxStackDataDepth: number = 1): void
{
    DataFactoryBPLibrary.DF_PrintString(undefined, StringToLog, LogVerbosity, bPrintToScreen, bPrintToLog, Duration, MaxStackDataDepth);

}
