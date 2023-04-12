
export function DFLOG(StringToLog: string, LogVerbosity: EDataFactoryLogVerbosity, bPrintToScreen: boolean, bPrintToLog: boolean, Duration: number, MaxStackDataDepth: number): void
{
    DataFactoryBPLibrary.DF_PrintString(undefined, StringToLog, LogVerbosity, bPrintToScreen, bPrintToLog, Duration, MaxStackDataDepth);

}
