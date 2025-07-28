#ifndef RCNET_ENGINE_H
#define RCNET_ENGINE_H

#include <stdbool.h> // Required for : bool

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Structure représentant les fonctions de rappel pour l'API Engine.
 * @typedef {struct} RC2D_Callbacks
 * @property {function} rc2d_unload - Fonction appelée lors du déchargement du server de jeu.
 * @property {function} rc2d_load - Fonction appelée lors du chargement du server de jeu.
 * @property {function} rc2d_update - Fonction appelée pour mettre à jour le server de jeu.
 */
typedef struct RCNET_Callbacks {
    // Server Loop Callbacks
    void (*rcnet_unload)(void);
    void (*rcnet_load)(void);
    void (*rcnet_update)(double dt);
} RCNET_Callbacks;

/**
 * Fonction principale pour exécuter le server de jeu.
 * @param {RC2D_Callbacks} callbacks - Callbacks pour l'API Engine.
 * @param {int} tickRate - Taux de rafraîchissement du server de jeu.
 * @returns {bool} - Retourne true si le server de jeu s'exécute correctement, sinon false.
 */
bool rcnet_engine_run(RCNET_Callbacks* callbacks, int tickRate);

/**
 * Fonction pour quitter le server de jeu.
 */
void rcnet_engine_eventQuit(void);

#ifdef __cplusplus
}
#endif

#endif // RCNET_ENGINE_H
