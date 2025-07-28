#ifndef JWT_H
#define JWT_H

#include <stdbool.h> // Required for : bool

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialise le client JWT avec la clé publique (décodée depuis base64) et l’issuer attendu.
 * Doit être appelé une seule fois au démarrage.
 * 
 * \param {const char*} public_key_pem - Clé publique au format PEM (base64)
 * \param {const char*} issuer - Issuer attendu (ex: "SeaTyrants_WebSite_BackEnd")
 * \return {bool} - true en cas de succès, false sinon
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
bool rcnet_jwt_clientInit(const char* public_key_pem, const char* issuer);

/**
 * Vérifie si le token JWT est valide (signature + issuer + expiration).
 * 
 * \param {const char*} token - Token JWT à vérifier
 * \return {bool} - true si le token est valide, false sinon
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
bool rcnet_jwt_clientVerify(const char* token);

/**
 * \brief Récupère un claim sous forme de string/double/bool depuis un token JWT.
 * Retourne NULL si le claim n’existe pas ou est d’un type non supporté.
 *
 * \param {const char*} token - Token JWT à analyser
 * \param {const char*} claim_name - Nom du champ à récupérer
 * \return {char*} - Valeur du claim sous forme de chaîne JSON (allouée dynamiquement), ou NULL
 * 
 * \warning La chaîne retournée doit être libérée par l'appelant avec `free()`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
char* rcnet_jwt_clientGetClaim(const char* token, const char* claim_name);

/**
 * \brief Décode une chaîne base64 standard. Cette fonction est utilisée pour 
 * décoder la clé publique PEM.
 * 
 * \param {const char*} input - Chaîne base64 (sans headers PEM)
 * \return {char*} - Chaîne binaire allouée dynamiquement, ou NULL
 * 
 * \warning La chaîne retournée doit être libérée par l'appelant avec `free()`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
char* rcnet_jwt_base64Decode(const char* input);

#ifdef __cplusplus
}
#endif

#endif // JWT_H