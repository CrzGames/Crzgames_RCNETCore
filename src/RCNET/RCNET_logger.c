#include "RCNET/RCNET_logger.h"

// Standard C libraries
#include <stdio.h> // Required for printf, fprintf

static RCNET_LogLevel currentLogLevel = RCNET_LOG_DEBUG; // Default log level

/**
 * Définit le niveau de priorité des messages de log.
 *
 * Cette fonction ajuste le niveau de priorité global pour les messages de log,
 * permettant de filtrer les messages moins importants selon le niveau spécifié.
 * Les messages ayant un niveau de priorité inférieur au niveau spécifié seront ignorés.
 *
 * @param logLevel Le niveau de log à définir, déterminant quels messages doivent être affichés.
 */
void rcnet_logger_setPriority(const RCNET_LogLevel logLevel) 
{
    currentLogLevel = logLevel;
}

/**
 * Affiche un message de log selon le format et les arguments spécifiés.
 *
 * Cette fonction affiche un message de log, en utilisant le formatage printf,
 * si son niveau de priorité est supérieur ou égal au niveau de log actuel.
 *
 * @param logLevel Le niveau de priorité du message.
 * @param format Le format du message, suivant la syntaxe de printf.
 * @param ... Les arguments à insérer dans le format du message.
 */
void rcnet_logger_log(const RCNET_LogLevel logLevel, const char* format, ...) 
{
    if (logLevel < currentLogLevel) return;

    va_list args;
    va_start(args, format);

    FILE* output = (logLevel >= RCNET_LOG_ERROR) ? stderr : stdout;

    fprintf(output, "[%d] ", logLevel); // Affiche le niveau de log
    vfprintf(output, format, args); // Affiche le message formaté
    fprintf(output, "\n"); // Nouvelle ligne après le message

    va_end(args);
}
