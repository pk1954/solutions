// EvolutionConsole.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "EvolutionCore.h"

// scripting and tracing

#include "script.h"

int main( int argc, char *argv [ ], char *envp [ ] )
{
    Config::InitializeConfig( );
    EvolutionCore::InitClass( );
    EvolutionCore::CreateCore( );

    ProcessScript( L"std_configuration.in" );

    for ( int iCount = 0; iCount < argc; iCount++ )
    {
        std::string const strCmdLine( argv[ iCount ] );

        if ( strCmdLine.compare( "/nohist" ) == 0 )
            Config::SetConfigValue( Config::tId::maxGeneration, 0 );
    }

    ProcessScript( L"std_script.in" );

    return 0;
}

