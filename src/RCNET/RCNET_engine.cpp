#include "RCNET/RCNET.h"

// Dependencies Libraries OpenSSL
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

// Standard C/C++ Libraries
#include <stdbool.h> // Required for : bool
#include <cstdlib>  // Required for : getenv
#include <time.h> // Required for : nanosleep
#include <chrono> // Required for : std::chrono
using namespace std::chrono;

// ServerLoop
static bool serverIsRunning = true;

// TickRates
static int tickRate = 60;
static unsigned long tickDuration; // Duration of each tick in nanoseconds
static double fixedDeltaTime = 0.0; // Delta fixe pour updates

// Delta time
static double rcnet_deltaTime = 0;

// Callbacks API Server Engine
RCNET_Callbacks callbacksServerEngine = { 
    NULL, // rcnet_unload
    NULL, // rcnet_load
    NULL, // rcnet_update
};

/**
 * \brief Initialise la bibliothèque OpenSSL avec options de log.
 * 
 * Cette fonction appelle OPENSSL_init_ssl() avec les options standards de chargement
 * des chaînes d’erreur et d’algorithmes. Elle loggue et quitte le programme si 
 * l’initialisation échoue.
 * 
 * \return {bool} - true si l'initialisation réussit, false sinon
 *
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static bool rcnet_engine_initOpenssl(void) 
{
    if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL) == 0)
    {
        rcnet_logger_log(RCNET_LOG_ERROR, "Erreur lors de l'initialisation d'OpenSSL : %s", ERR_error_string(ERR_get_error(), NULL));
        return false;
    }
    else 
    {
        rcnet_logger_log(RCNET_LOG_INFO, "OpenSSL initialisé avec succès.");
        return true;
    }
}

/**
 * \brief Initialise le client JWT avec la clé publique et l'émetteur.
 * 
 * Cette fonction récupère la clé publique JWT depuis une variable d'environnement,
 * la décode en PEM, puis initialise le client JWT. Elle loggue les erreurs et
 * retourne false si l'initialisation échoue.
 * 
 * \return {bool} - true si l'initialisation réussit, false sinon
 *
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static bool rcnet_engine_initClientJWT(void)
{
    const char* keyJWTBase64 = getenv("SEATYRANTS_PUBLIC_KEY_JWT_BASE64");
    if (!keyJWTBase64) 
    {
        rcnet_logger_log(RCNET_LOG_ERROR, "Variable d environnement SEATYRANTS_PUBLIC_KEY_JWT_BASE64 manquante.");
        return false;
    }

    char* pemKey = rcnet_jwt_base64Decode(keyJWTBase64);
    if (!pemKey) 
    {
        rcnet_logger_log(RCNET_LOG_ERROR, "Erreur: décodage base64 de la cle publique (SEATYRANTS_PUBLIC_KEY_JWT_BASE64) echoue.");
        return false;
    }

    if (!rcnet_jwt_clientInit(pemKey, "SeaTyrants_WebSite_BackEnd")) 
    {
        rcnet_logger_log(RCNET_LOG_ERROR, "Erreur: initialisation du client JWT.");
        free(pemKey);
        return false;
    }

    free(pemKey);
    rcnet_logger_log(RCNET_LOG_INFO, "Client JWT initialiser avec succes.");
    return true;
}

/**
 * \brief Obtient le temps actuel en nanosecondes depuis l'époque.
 * 
 * Cette fonction utilise chrono pour obtenir le temps actuel en nanosecondes.
 * 
 * \return {uint64_t} - Temps actuel en nanosecondes.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static uint64_t rcnet_engine_getCurrentTimeNs(void)
{
    auto now = steady_clock::now();
    return static_cast<uint64_t>(duration_cast<nanoseconds>(now.time_since_epoch()).count());
}


/**
 * \brief Définit les callbacks du moteur de jeu.
 * 
 * Cette fonction permet de définir les callbacks pour le moteur de jeu.
 * 
 * \param callbacksUser {RCNET_Callbacks*} - Pointeur vers la structure des callbacks à définir.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static void rcnet_engine_setCallbacks(RCNET_Callbacks* callbacksUser) 
{
    // Game loop callbacks
    if (callbacksUser->rcnet_unload != NULL) callbacksServerEngine.rcnet_unload = callbacksUser->rcnet_unload;
    if (callbacksUser->rcnet_load != NULL) callbacksServerEngine.rcnet_load = callbacksUser->rcnet_load;
    if (callbacksUser->rcnet_update != NULL) callbacksServerEngine.rcnet_update = callbacksUser->rcnet_update;
}

/**
 * \brief Initialise le moteur de jeu RCNET.
 * 
 * Cette fonction initialise OpenSSL, le client JWT et la durée des ticks.
 * 
 * \return {bool} - true si l'initialisation réussit, false sinon
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static bool rcnet_engine(void)
{
    // Initialize OpenSSL
    if (!rcnet_engine_initOpenssl())
    {
        return false;
    }

    // Initialize JWT Client
    if (!rcnet_engine_initClientJWT())
    {
        return false;
    }

    // Initialize tickDuration & fixedDeltaTime
    tickDuration = 1000000000 / tickRate;
    fixedDeltaTime = 1.0 / static_cast<double>(tickRate);

	return true;
}

/**
 * \brief Boucle principale du serveur.
 * 
 * Cette fonction gère la boucle principale du serveur, en appelant les callbacks
 * d'update à chaque tick et en gérant le timing pour respecter le tickRate.
 * 
 * \param next_tick_time {unsigned long*} - Pointeur vers le temps du prochain tick.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static void rcnet_engine_serverloop(unsigned long* next_tick_time)
{
    unsigned long now = rcnet_engine_getCurrentTimeNs();

    // Si on est en avance, on dort jusqu'au tick prévu
    if (now < *next_tick_time) 
    {
        unsigned long sleep_time = *next_tick_time - now;
        struct timespec ts;
        ts.tv_sec = sleep_time / 1000000000;
        ts.tv_nsec = sleep_time % 1000000000;
        nanosleep(&ts, NULL);
        now = rcnet_engine_getCurrentTimeNs(); // recheck après sleep
    }

    // deltaTime réel (en secondes)
    rcnet_deltaTime = (now - (*next_tick_time - tickDuration)) / 1e9;

    // Appeler la fonction d’update du serveur
    if (callbacksServerEngine.rcnet_update != NULL)
    {
        callbacksServerEngine.rcnet_update(fixedDeltaTime);
    }

    // Planifier le prochain tick
    *next_tick_time += tickDuration;

    // Si on a pris du retard, corriger
    if (*next_tick_time < rcnet_engine_getCurrentTimeNs())
    {
        rcnet_logger_log(RCNET_LOG_WARN, "Le serveur est en retard d’un tick !");
        *next_tick_time = rcnet_engine_getCurrentTimeNs(); // reset sur maintenant
    }
}

/**
 * \brief Initialise le moteur de jeu RCNET.
 * 
 * Cette fonction initialise le moteur de jeu RCNET.
 * 
 * \return {bool} - true si l'initialisation réussit, false sinon
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static bool rcnet_engine_init(void)
{
	// Init ServeEngine house
	return rcnet_engine();
}

/**
 * \brief Quitte le moteur de jeu RCNET.
 * 
 * Cette fonction libère les ressources utilisées par le moteur de jeu RCNET.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
static void rcnet_engine_quit(void)
{    
    // Lib OpenSSL Deinitialize
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    SSL_COMP_free_compression_methods();
}

/**
 * \brief Événement de sortie du moteur de jeu RCNET.
 * 
 * Cette fonction est appelée lorsque le moteur de jeu RCNET doit quitter.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
void rcnet_engine_eventQuit(void)
{
    serverIsRunning = false;
}

/**
 * \brief Démarre le moteur de jeu RCNET avec les callbacks et le tick rate spécifiés.
 * 
 * Cette fonction configure les callbacks du moteur de jeu, initialise le moteur,
 * et lance la boucle principale du serveur.
 * 
 * \param callbacksUser {RCNET_Callbacks*} - Pointeur vers les callbacks à utiliser.
 * \param tickRate {int} - Taux de ticks par seconde (par défaut 60).
 * 
 * \return {bool} - true si le moteur démarre avec succès, false sinon.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
bool rcnet_engine_run(RCNET_Callbacks* callbacksUser, int tickRateArg)
{
    // Set Callbacks to Server Engine
    if (callbacksUser != NULL)
    {
        rcnet_engine_setCallbacks(callbacksUser);
    }

    // Set TickRate
    if (tickRateArg > 0)
    {
        ::tickRate = tickRateArg;
        tickDuration = 1000000000UL / ::tickRate;
        fixedDeltaTime = 1.0 / ::tickRate;
    }

    // Init GameEngine RCNET
	if(!rcnet_engine_init())
    {
		rcnet_engine_quit();
        return false;
    }

    // First call the callback to load the server loop
    if (callbacksServerEngine.rcnet_load != NULL) 
    {
        callbacksServerEngine.rcnet_load();
    }

    // ServerLoop - Main thread
    unsigned long next_tick_time = rcnet_engine_getCurrentTimeNs() + tickDuration;
    while (serverIsRunning) 
    {
        rcnet_engine_serverloop(&next_tick_time);
    }

    // Last call the callback to unload the server loop (free memory, etc.)
    if (callbacksServerEngine.rcnet_unload != NULL)
    {
        callbacksServerEngine.rcnet_unload();
    }

    // FreeMemory ServerEngine and Quit Program.
    rcnet_engine_quit();

	return true;
}