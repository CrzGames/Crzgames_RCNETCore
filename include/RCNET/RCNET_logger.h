#ifndef RCNET_LOGGER_H
#define RCNET_LOGGER_H

#include <stdarg.h> // Required for : ... (va_list, va_start, va_end, vsnprintf)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Niveaux de journalisation pour RCNET.
 * 
 * Ces niveaux déterminent la sévérité des messages de journalisation.
 * 
 * \since Cette énumération est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_LogLevel {
    /**
     * \brief Niveau de journalisation pour les messages de débogage.
     */
    RCNET_LOG_DEBUG,

    /**
     * \brief Niveau de journalisation pour les messages d'information.
     */
    RCNET_LOG_INFO,

    /**
     * \brief Niveau de journalisation pour les messages d'avertissement.
     */
    RCNET_LOG_WARN,

    /**
     * \brief Niveau de journalisation pour les messages d'erreur.
     */
    RCNET_LOG_ERROR,

    /**
     * \brief Niveau de journalisation pour les messages critiques.
     */
    RCNET_LOG_CRITICAL   
} RCNET_LogLevel;

/**
 * \brief Définit le niveau de journalisation global pour RCNET.
 * 
 * Cette fonction permet de configurer le niveau de journalisation
 * pour contrôler quels messages seront effectivement journalisés.
 * 
 * \param {RCNET_LogLevel} logLevel - Le niveau de journalisation à définir.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
void rcnet_logger_setPriority(const RCNET_LogLevel logLevel);

/**
 * \brief Journalise un message avec un niveau de sévérité spécifique.
 * 
 * Cette fonction permet de journaliser des messages formatés selon le niveau
 * de sévérité spécifié. Elle utilise `vsnprintf` pour formater le message.
 * 
 * \param {RCNET_LogLevel} logLevel - Le niveau de journalisation du message.
 * \param {const char*} format - Le format du message (similaire à printf).
 * \param {...} - Les arguments à formater dans le message.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
void rcnet_logger_log(const RCNET_LogLevel logLevel, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif // RCNET_LOGGER_H
