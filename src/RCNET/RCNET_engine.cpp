#include "RCNET/RCNET.h"

// Dependencies Libraries
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

// Standard C Libraries
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <cstdlib>  // Required for : getenv

// ServerLoop
static bool serverIsRunning = true;

// TickRates
static int tickRate = 60;
static unsigned long tickDuration; // Duration of each tick in nanoseconds

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
 * \brief Obtient le temps actuel en nanosecondes.
 * 
 * Cette fonction utilise `clock_gettime` pour obtenir le temps actuel
 * depuis l'horloge monotone, ce qui est utile pour les calculs de temps
 * dans les boucles de jeu.
 * 
 * \return {unsigned long} - Le temps actuel en nanosecondes.
 */
static unsigned long rcnet_engine_getCurrentTimeNs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000000000 + spec.tv_nsec; // Convert to nanoseconds
}

// Set Callbacks Engine by User
static void rcnet_engine_setCallbacks(RCNET_Callbacks* callbacksUser) 
{
    // Game loop callbacks
    if (callbacksUser->rcnet_unload != NULL) callbacksServerEngine.rcnet_unload = callbacksUser->rcnet_unload;
    if (callbacksUser->rcnet_load != NULL) callbacksServerEngine.rcnet_load = callbacksUser->rcnet_load;
    if (callbacksUser->rcnet_update != NULL) callbacksServerEngine.rcnet_update = callbacksUser->rcnet_update;
}

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

    // Initialize tickDuration
    tickDuration = 1000000000 / tickRate;

	return true;
}

static void rcnet_engine_serverloop(unsigned long* last_time) 
{
    // Start Time ServerLoop
    unsigned long start_time = rcnet_engine_getCurrentTimeNs();
    
    // Calculate deltaTime
    rcnet_deltaTime = (start_time - *last_time) / 1000000000.0; // Convert ns to seconds
    *last_time = start_time;

    // Run Callbacks Update
    if (callbacksServerEngine.rcnet_update != NULL) 
    {
        callbacksServerEngine.rcnet_update(rcnet_deltaTime);
    }

    // End Time ServerLoop
    unsigned long end_time = rcnet_engine_getCurrentTimeNs();
    unsigned long elapsed_time = end_time - start_time;
    if (elapsed_time < tickDuration) 
    {
        unsigned long sleep_time = tickDuration - elapsed_time;
        struct timespec ts;
        ts.tv_sec = sleep_time / 1000000000;
        ts.tv_nsec = sleep_time % 1000000000;
        nanosleep(&ts, NULL);
    }
}

static bool rcnet_engine_init(void)
{
	// Init ServeEngine house
	if (!rcnet_engine())
    {
		return false;
    }
    else
    {
        return true;
    }
}

static void rcnet_engine_quit(void)
{    
    // Lib OpenSSL Deinitialize
    ERR_free_strings();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    SSL_COMP_free_compression_methods();
    SSL_CTX_free(NULL);
}

void rcnet_engine_eventQuit(void)
{
    serverIsRunning = false;
}

bool rcnet_engine_run(RCNET_Callbacks* callbacksUser, int tickRate)
{
    // Set Callbacks to Server Engine
    if (callbacksUser != NULL)
    {
        rcnet_engine_setCallbacks(callbacksUser);
    }

    // Set TickRate
    if (tickRate > 0)
    {
        tickRate = tickRate;
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
    unsigned long last_time = rcnet_engine_getCurrentTimeNs();
    while (serverIsRunning) 
    {
        rcnet_engine_serverloop(&last_time);
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